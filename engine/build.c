/** @file build.c
 * Build engine for litebuild.
 * @author Frank Wu
 */
#include "build.h"

#include "exec.h"
#include "../frontend/parser.h"
#include "../utils/logger.h"
#include "../utils/memory.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define MIN_OBJECT_TARGET_LENGTH 3

static BuildResult build_target_recursive(BuildFileModel* model,
        VariableTable* vars, const char* targetName, bool isTopLevel,
        const CommandLineOptions* options, RuntimeState* runtime);
static bool is_object_file_target(const char* name);
static char* make_matching_c_filename(const char* objName);
static BuildResult handle_implicit_object_rule(BuildFileModel* model,
        VariableTable* vars, const char* targetName, bool isTopLevel,
        const CommandLineOptions* options, RuntimeState* runtime);
static bool should_rebuild_file_target(const char* targetName,
        const BuildResult* depResults, int depCount, bool targetExists,
        struct timespec targetMtime);
static StringVector make_compile_command(
        const VariableTable* vars, const char* sourceName);
static StringVector make_link_command(BuildFileModel* model,
        const VariableTable* vars, const char* targetName,
        char** dependencies, int dependencyCount);
static void expand_file_target_dependencies_if_needed(
        FileTarget* target, VariableTable* vars);
static void expand_phony_pipeline_if_needed(
        PhonyTarget* target, VariableTable* vars);

/* ------------------------------------------------------------------------- */
/* Local helper functions                                                    */
/* ------------------------------------------------------------------------- */
/**
 * Stops execution if an interrupt signal has been received.
 *
 * @param runtime runtime state containing interrupt flag.
 *
 */
static void stop_if_interrupted(RuntimeState* runtime)
{
    if (runtime != NULL && runtime->interrupted) {
        handle_interrupt_shutdown(runtime);
    }
}

/**
 * Appends variable values into argument list.
 *
 * @param argv argument list.
 * @param argc argument count.
 * @param vars variable table.
 * @param varName variable name.
 *
 */
static void append_variable_values(
        StringVector* cmd, const VariableTable* vars, const char* varName)
{
    Variable* variable;
    int i;

    variable = lookup_variable((VariableTable*)vars, varName);
    if (variable == NULL) {
        return;
    }

    for (i = 0; i < variable->values.count; i++) {
        strvec_push(cmd, variable->values.data[i]);
    }
}

/**
 * Gets file modification time.
 *
 * @param filename file name.
 * @param mtimeOut output time.
 * @return 1 if file exists, 0 if missing, -1 on stat error.
 *
 */
static int get_file_mtime(const char* filename, struct timespec* mtimeOut)
{
    struct stat fileInfo;

    if (stat(filename, &fileInfo) != 0) {
        if (errno == ENOENT) {
            return 0;
        }
        log_error("failed to stat file \"%s\": %s\n", filename,
                strerror(errno));
        return -1;
    }

    if (mtimeOut != NULL) {
        *mtimeOut = fileInfo.st_mtim;
    }
    return 1;
}

/**
 * Compares two timespec values.
 *
 * @param a first time.
 * @param b second time.
 * @return comparison result.
 *
 */
static int compare_timespec(struct timespec a, struct timespec b)
{
    if (a.tv_sec > b.tv_sec) {
        return 1;
    }
    if (a.tv_sec < b.tv_sec) {
        return -1;
    }
    if (a.tv_nsec > b.tv_nsec) {
        return 1;
    }
    if (a.tv_nsec < b.tv_nsec) {
        return -1;
    }
    return 0;
}

/**
 * Creates default build result.
 *
 * @return initialized BuildResult.
 *
 */
static BuildResult make_default_build_result(void)
{
    BuildResult result;

    result.success = true;
    result.builtSomething = false;
    result.targetExistsAsFile = false;
    result.isPhony = false;
    result.mtime.tv_sec = 0;
    result.mtime.tv_nsec = 0;
    return result;
}

static BuildResult make_failed_build_result(void)
{
    BuildResult result = make_default_build_result();

    result.success = false;
    return result;
}

/**
 * Prints missing file error and returns a failed build result.
 *
 * @param targetName file name.
 *
 */
static BuildResult report_missing_file(const char* targetName)
{
    log_error("cannot find file \"%s\" - aborting\n", targetName);
    return make_failed_build_result();
}

/**
 * Checks if target is phony.
 *
 * @param model BuildFile model.
 * @param name target name.
 * @return true if phony.
 *
 */
static bool is_phony_target_name(const BuildFileModel* model, const char* name)
{
    Target* target;

    target = find_target_by_name((BuildFileModel*)model, name);
    return target != NULL && target->type == TARGET_PHONY;
}

/**
 * Frees commands created by splitting a command pipeline.
 *
 */
static void free_split_pipeline(char*** commands, int* argcList, int cmdCount)
{
    int i;

    if (commands != NULL) {
        for (i = 0; i < cmdCount; i++) {
            free(commands[i]);
        }
        free(commands);
    }

    free(argcList);
}

/**
 * Builds a phony target by expanding and executing its command pipeline.
 *
 * @param target phony target to build.
 * @param vars variable table.
 * @param options command line options.
 * @param runtime runtime state.
 * @return build result.
 *
 */
static BuildResult build_phony_target(PhonyTarget* target, VariableTable* vars,
        const CommandLineOptions* options, RuntimeState* runtime)
{
    BuildResult result;
    char*** commands;
    int* argcList;
    int cmdCount;
    int pipelineResult;

    stop_if_interrupted(runtime);

    // Expand the pipeline only when this target is actually built.
    expand_phony_pipeline_if_needed(target, vars);

    // A pipeline can become invalid after variable expansion.
    if (!is_valid_pipeline_after_expansion(target->expandedPipeline.data,
                target->expandedPipeline.count)) {
        log_error("invalid command pipeline\n");
        return make_failed_build_result();
    }

    commands = split_pipeline_into_commands(target->expandedPipeline.data,
            target->expandedPipeline.count, &argcList, &cmdCount);

    stop_if_interrupted(runtime);

    // Run or simulate the whole pipeline, then free the split command arrays.
    pipelineResult = execute_pipeline(
            commands, argcList, cmdCount, runtime, options->simulate);

    free_split_pipeline(commands, argcList, cmdCount);

    if (pipelineResult != 0) {
        log_error("command pipeline failed\n");
        return make_failed_build_result();
    }

    result = make_default_build_result();
    result.builtSomething = true;
    result.isPhony = true;
    return result;
}

/**
 * Handles a target that already exists as a normal file.
 *
 * @param targetName target name.
 * @param isTopLevel whether this is a top-level build request.
 * @return build result.
 *
 * for retrieving file modification time and reporting no-action cases.
 */
static BuildResult handle_existing_file_target(
        const char* targetName, bool isTopLevel)
{
    BuildResult result = make_default_build_result();
    int fileStatus;

    fileStatus = get_file_mtime(targetName, &result.mtime);
    if (fileStatus == 0) {
        return report_missing_file(targetName);
    }
    if (fileStatus < 0) {
        return make_failed_build_result();
    }

    result.targetExistsAsFile = true;

    if (isTopLevel) {
        log_info("Target \"%s\" - no action required\n", targetName);
    }

    return result;
}
/**
 * Builds all dependencies for one file target.
 *
 * @param model BuildFile model.
 * @param vars variable table.
 * @param fileTarget file target being built.
 * @param options CLI options.
 * @param runtime runtime state.
 * @return dependency build results.
 *
 * build_explicit_file_target.
 */
static BuildResult* build_file_dependencies(BuildFileModel* model,
        VariableTable* vars, FileTarget* fileTarget,
        const CommandLineOptions* options, RuntimeState* runtime)
{
    BuildResult* depResults;
    int i;

    depResults = xmalloc(sizeof(BuildResult) * fileTarget->expandedDeps.count);

    for (i = 0; i < fileTarget->expandedDeps.count; i++) {
        stop_if_interrupted(runtime);
        depResults[i] = build_target_recursive(model, vars,
                fileTarget->expandedDeps.data[i], false, options, runtime);
    }

    return depResults;
}

static bool build_results_succeeded(const BuildResult* results, int count)
{
    for (int i = 0; i < count; i++) {
        if (!results[i].success) {
            return false;
        }
    }
    return true;
}

/**
 * Runs the compile or link command for an explicit file target.
 *
 * @param model BuildFile model.
 * @param vars variable table.
 * @param targetName target name.
 * @param fileTarget file target data.
 * @param options CLI options.
 * @param runtime runtime state.
 *
 * build_explicit_file_target.
 */
static bool run_explicit_file_command(BuildFileModel* model,
        VariableTable* vars, const char* targetName, FileTarget* fileTarget,
        const CommandLineOptions* options, RuntimeState* runtime)
{
    StringVector cmd;
    bool success;

    // Choose compile or link command depending on target type.
    if (is_object_file_target(targetName)) {
        char* sourceName = make_matching_c_filename(targetName);

        cmd = make_compile_command(vars, sourceName);
        stop_if_interrupted(runtime);

        // Execute compile command for .o target.
        success = execute_command_or_abort(
                cmd.data, runtime, options->simulate, targetName);

        free(sourceName);
    } else {
        cmd = make_link_command(model, vars, targetName,
                fileTarget->expandedDeps.data, fileTarget->expandedDeps.count);
        stop_if_interrupted(runtime);

        // Execute link command for final executable.
        success = execute_command_or_abort(
                cmd.data, runtime, options->simulate, targetName);
    }

    // Free dynamically created command argument list.
    strvec_free(&cmd);
    return success;
}

/**
 * Creates a build result after a target has been rebuilt.
 *
 * @param targetName target name.
 * @param options CLI options.
 * @return build result.
 *
 */
static BuildResult make_rebuilt_file_result(
        const char* targetName, const CommandLineOptions* options)
{
    BuildResult result = make_default_build_result();
    int fileStatus;

    result.builtSomething = true;
    result.targetExistsAsFile = true;

    if (options->simulate) {
        result.mtime.tv_sec = 0;
        result.mtime.tv_nsec = 0;
    } else {
        fileStatus = get_file_mtime(targetName, &result.mtime);
        if (fileStatus == 0) {
            return report_missing_file(targetName);
        }
        if (fileStatus < 0) {
            return make_failed_build_result();
        }
    }

    return result;
}

/**
 * Creates a build result when no rebuild was needed.
 *
 * @param targetName target name.
 * @param isTopLevel top-level flag.
 * @param targetMtime target modification time.
 * @return build result.
 *
 * functions.
 */
static BuildResult make_no_action_file_result(const char* targetName,
        bool isTopLevel, struct timespec targetMtime)
{
    BuildResult result = make_default_build_result();

    result.targetExistsAsFile = true;
    result.mtime = targetMtime;

    if (isTopLevel) {
        log_info("Target \"%s\" - no action required\n", targetName);
    }

    return result;
}
/**
 * Builds an explicit file target after its dependencies have been handled.
 *
 * @param model BuildFile model.
 * @param vars variable table.
 * @param targetName target name.
 * @param fileTarget file target data.
 * @param isTopLevel top-level flag.
 * @param options CLI options.
 * @param runtime runtime state.
 * @return build result.
 *
 * helper functions.
 */
static BuildResult build_explicit_file_target(BuildFileModel* model,
        VariableTable* vars, const char* targetName, FileTarget* fileTarget,
        bool isTopLevel, const CommandLineOptions* options,
        RuntimeState* runtime)
{
    BuildResult result;
    BuildResult* depResults;
    bool targetExists;
    int fileStatus;
    struct timespec targetMtime;

    stop_if_interrupted(runtime);

    // Expand dependencies once before deciding how this target should build.
    expand_file_target_dependencies_if_needed(fileTarget, vars);

    // Empty dependency list means this is either an implicit .o rule or a file.
    if (fileTarget->expandedDeps.count == 0) {
        if (is_object_file_target(targetName)) {
            return handle_implicit_object_rule(
                    model, vars, targetName, isTopLevel, options, runtime);
        }
        return handle_existing_file_target(targetName, isTopLevel);
    }

    // Build dependencies first so their results can guide rebuild decisions.
    depResults = build_file_dependencies(
            model, vars, fileTarget, options, runtime);
    stop_if_interrupted(runtime);

    if (!build_results_succeeded(depResults, fileTarget->expandedDeps.count)) {
        free(depResults);
        return make_failed_build_result();
    }

    fileStatus = get_file_mtime(targetName, &targetMtime);
    if (fileStatus < 0) {
        free(depResults);
        return make_failed_build_result();
    }
    targetExists = fileStatus == 1;

    // Rebuild only when the target is missing or dependencies require it.
    if (should_rebuild_file_target(targetName, depResults,
                fileTarget->expandedDeps.count, targetExists, targetMtime)) {
        if (!run_explicit_file_command(
                    model, vars, targetName, fileTarget, options, runtime)) {
            free(depResults);
            return make_failed_build_result();
        }
        result = make_rebuilt_file_result(targetName, options);
    } else {
        result = make_no_action_file_result(
                targetName, isTopLevel, targetMtime);
    }

    free(depResults);
    return result;
}

/**
 * Recursively builds a target.
 *
 * @param model parsed model.
 * @param vars variable table.
 * @param targetName target name.
 * @param isTopLevel top-level flag.
 * @param options CLI options.
 * @param runtime runtime state.
 * @return build result.
 *
 */
static BuildResult build_target_recursive(BuildFileModel* model, VariableTable* vars,
        const char* targetName, bool isTopLevel,
        const CommandLineOptions* options, RuntimeState* runtime)
{
    Target* target;

    stop_if_interrupted(runtime);

    // Look up the target in the parsed BuildFile model.
    target = find_target_by_name(model, targetName);

    // If it is a phony target, build it using its pipeline.
    if (target != NULL && target->type == TARGET_PHONY) {
        return build_phony_target(
                &target->data.phonyTarget, vars, options, runtime);
    }

    // If target is not defined, handle implicit rules or existing files.
    if (target == NULL) {
        if (is_object_file_target(targetName)) {
            return handle_implicit_object_rule(
                    model, vars, targetName, isTopLevel, options, runtime);
        }
        return handle_existing_file_target(targetName, isTopLevel);
    }

    // Otherwise, build a normal explicit file target.
    return build_explicit_file_target(model, vars, targetName,
            &target->data.fileTarget, isTopLevel, options, runtime);
}

/* ------------------------------------------------------------------------- */
/* Required build.c functions                                                */
/* ------------------------------------------------------------------------- */
/**
 * Builds requested targets.
 *
 * @param model BuildFile model.
 * @param vars variable table.
 * @param options CLI options.
 * @param runtime runtime state.
 *
 */
bool build_requested_targets(BuildFileModel* model,
        VariableTable* vars, const CommandLineOptions* options,
        RuntimeState* runtime)
{
    int i;
    BuildResult result;

    stop_if_interrupted(runtime);

    if (options->requestedTargets.count > 0) {
        for (i = 0; i < options->requestedTargets.count; i++) {
            stop_if_interrupted(runtime);
            result = build_target_recursive(model, vars,
                    options->requestedTargets.data[i], true, options,
                    runtime);
            if (!result.success) {
                return false;
            }
        }
    } else {
        result = build_target_recursive(
                model, vars, model->firstTargetName, true, options, runtime);
        if (!result.success) {
            return false;
        }
    }

    return true;
}

/**
 * Checks if name is .o file.
 *
 * @param name target name.
 * @return true if object file.
 *
 */
static bool is_object_file_target(const char* name)
{
    size_t len;

    if (name == NULL) {
        return false;
    }

    len = strlen(name);
    if (len < MIN_OBJECT_TARGET_LENGTH) {
        return false;
    }

    return strcmp(name + len - 2, ".o") == 0;
}

/**
 * Converts .o to .c filename.
 *
 * @param objName object file.
 * @return matching .c file.
 *
 */
static char* make_matching_c_filename(const char* objName)
{
    char* sourceName;
    size_t len;

    if (!is_object_file_target(objName)) {
        return NULL;
    }

    len = strlen(objName);
    sourceName = xstrdup(objName);
    sourceName[len - 1] = 'c';
    return sourceName;
}

/**
 * Checks if an implicit object target should be rebuilt.
 *
 * @param sourceResult build result of the matching source file.
 * @param targetExists whether the object target exists.
 * @param targetMtime object target modification time.
 * @return true if the object target should be rebuilt.
 *
 * handle_implicit_object_rule.
 */
static bool should_rebuild_implicit_object(BuildResult sourceResult,
        bool targetExists, struct timespec targetMtime)
{
    if (!targetExists) {
        return true;
    }
    if (sourceResult.builtSomething || sourceResult.isPhony) {
        return true;
    }
    return sourceResult.targetExistsAsFile
            && compare_timespec(sourceResult.mtime, targetMtime) > 0;
}

/**
 * Runs the compile command for an implicit object target.
 *
 * @param vars variable table.
 * @param sourceName source file name.
 * @param targetName object target name.
 * @param options CLI options.
 * @param runtime runtime state.
 *
 * handle_implicit_object_rule.
 */
static bool run_implicit_object_command(VariableTable* vars,
        const char* sourceName, const char* targetName,
        const CommandLineOptions* options, RuntimeState* runtime)
{
    StringVector cmd;
    bool success;

    cmd = make_compile_command(vars, sourceName);
    stop_if_interrupted(runtime);
    success = execute_command_or_abort(
            cmd.data, runtime, options->simulate, targetName);
    strvec_free(&cmd);
    return success;
}
/**
 * Handles implicit object build rule.
 *
 * @param model BuildFile model.
 * @param vars variable table.
 * @param targetName target name.
 * @param isTopLevel top-level flag.
 * @param options CLI options.
 * @param runtime runtime state.
 * @return build result.
 *
 * helper functions.
 */
static BuildResult handle_implicit_object_rule(BuildFileModel* model,
        VariableTable* vars, const char* targetName, bool isTopLevel,
        const CommandLineOptions* options, RuntimeState* runtime)
{
    BuildResult result;
    BuildResult sourceResult;
    bool targetExists;
    int fileStatus;
    struct timespec targetMtime;
    char* sourceName;

    stop_if_interrupted(runtime);

    // Convert the object target name to the matching source file name.
    sourceName = make_matching_c_filename(targetName);
    if (sourceName == NULL) {
        return handle_existing_file_target(targetName, isTopLevel);
    }

    // Build the source file first because the object file depends on it.
    sourceResult = build_target_recursive(
            model, vars, sourceName, false, options, runtime);
    stop_if_interrupted(runtime);
    if (!sourceResult.success) {
        free(sourceName);
        return make_failed_build_result();
    }

    fileStatus = get_file_mtime(targetName, &targetMtime);
    if (fileStatus < 0) {
        free(sourceName);
        return make_failed_build_result();
    }
    targetExists = fileStatus == 1;

    // Compile the object file only when it is missing or out of date.
    if (should_rebuild_implicit_object(sourceResult, targetExists,
                targetMtime)) {
        if (!run_implicit_object_command(
                    vars, sourceName, targetName, options, runtime)) {
            free(sourceName);
            return make_failed_build_result();
        }
        result = make_rebuilt_file_result(targetName, options);
    } else {
        result = make_no_action_file_result(
                targetName, isTopLevel, targetMtime);
    }

    free(sourceName);
    return result;
}

/**
 * Checks if rebuild is needed.
 *
 * @param targetName target.
 * @param depResults dependencies.
 * @param depCount count.
 * @param targetExists existence.
 * @param targetMtime time.
 * @return true if rebuild needed.
 *
 */
static bool should_rebuild_file_target(const char* targetName,
        const BuildResult* depResults, int depCount, bool targetExists,
        struct timespec targetMtime)
{
    int i;

    (void)targetName;

    // If the target file does not exist, it must be rebuilt.
    if (!targetExists) {
        return true;
    }

    // Check each dependency to determine if a rebuild is required.
    for (i = 0; i < depCount; i++) {
        if (depResults[i].builtSomething) {
            return true;
        }
        if (depResults[i].isPhony) {
            return true;
        }
        if (depResults[i].targetExistsAsFile
                && compare_timespec(depResults[i].mtime, targetMtime) > 0) {
            return true;
        }
    }

    // Otherwise, no rebuild is needed.
    return false;
}

/**
 * Creates compile command.
 *
 * @param vars variable table.
 * @param sourceName source file.
 * @param argcOut output count.
 * @return argv.
 *
 */
static StringVector make_compile_command(
        const VariableTable* vars, const char* sourceName)
{
    StringVector cmd;

    strvec_init(&cmd);
    append_variable_values(&cmd, vars, "COMPILER");
    strvec_push(&cmd, "-c");
    append_variable_values(&cmd, vars, "COMPILEFLAGS");
    strvec_push(&cmd, sourceName);

    return cmd;
}

/**
 * Creates link command.
 *
 * @param model BuildFile model.
 * @param vars variable table.
 * @param targetName output name.
 * @param dependencies deps.
 * @param dependencyCount count.
 * @param argcOut output count.
 * @return argv.
 *
 */
static StringVector make_link_command(BuildFileModel* model, const VariableTable* vars,
        const char* targetName, char** dependencies, int dependencyCount)
{
    StringVector cmd;
    int i;

    strvec_init(&cmd);

    // Add linker and flags, and specify output file.
    append_variable_values(&cmd, vars, "LINKER");
    append_variable_values(&cmd, vars, "COMPILEFLAGS");
    strvec_push(&cmd, "-o");
    strvec_push(&cmd, targetName);

    // Add all non-phony dependencies as input files.
    for (i = 0; i < dependencyCount; i++) {
        if (!is_phony_target_name(model, dependencies[i])) {
            strvec_push(&cmd, dependencies[i]);
        }
    }

    // Add linker-specific flags at the end.
    append_variable_values(&cmd, vars, "LINKFLAGS");
    return cmd;
}

/**
 * Expands file dependencies once.
 *
 * @param target file target.
 * @param vars variable table.
 *
 */
static void expand_file_target_dependencies_if_needed(
        FileTarget* target, VariableTable* vars)
{
    if (target->depsExpanded) {
        return;
    }

    strvec_free(&target->expandedDeps);
    target->expandedDeps = expand_tokens_once(vars, &target->rawDeps);
    target->depsExpanded = true;
}

/**
 * Expands phony pipeline once.
 *
 * @param target phony target.
 * @param vars variable table.
 *
 */
static void expand_phony_pipeline_if_needed(PhonyTarget* target, VariableTable* vars)
{
    if (target->pipelineExpanded) {
        return;
    }

    strvec_free(&target->expandedPipeline);
    target->expandedPipeline = expand_tokens_once(vars, &target->rawPipeline);
    target->pipelineExpanded = true;
}
