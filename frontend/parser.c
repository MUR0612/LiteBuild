/** @file parser.c
 * @author Frank WU
 *
 * Parser functions for reading a makefile and storing its contents.
 */
#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../lexer.h"
#include "../utils/logger.h"
#include "../utils/memory.h"

static bool handle_variable_line(UQMakefileModel* model, char** tokens,
        unsigned int tokenCount, int lineNumber);
static bool handle_dependency_line(UQMakefileModel* model, char** tokens,
        unsigned int tokenCount, int lineNumber);
static bool handle_phony_line(UQMakefileModel* model, char** tokens,
        unsigned int tokenCount, int lineNumber);
static bool target_name_already_seen(const HashMap* seenTargetNames,
        const char* name);

static void print_variable_line_error(const char* filename, int lineNumber)
{
    log_error("file \"%s\", line %d: variable definition not valid\n",
            filename, lineNumber);
}

static void print_dependency_line_error(const char* filename, int lineNumber)
{
    log_error("file \"%s\", line %d: invalid dependency definition\n",
            filename, lineNumber);
}

static void print_phony_line_error(const char* filename, int lineNumber)
{
    log_error("file \"%s\", line %d: phony target definition not valid\n",
            filename, lineNumber);
}

static void print_repeated_target_error(const char* filename, int lineNumber)
{
    log_error("file \"%s\", line %d: target name is repeated\n",
            filename, lineNumber);
}

static void print_line_not_valid_error(const char* filename, int lineNumber)
{
    log_error("file \"%s\", line %d: line not valid\n", filename,
            lineNumber);
}

static bool append_variable_def(UQMakefileModel* model, VariableDef def)
{
    VariableDef* resized = xrealloc(model->variableDefs,
            sizeof(VariableDef) * (model->variableDefCount + 1));

    model->variableDefs = resized;
    model->variableDefs[model->variableDefCount] = def;
    model->variableDefCount++;
    return true;
}

static bool append_target(UQMakefileModel* model, Target target)
{
    char* firstTargetName = NULL;
    Target* resized;

    if (model->firstTargetName == NULL) {
        firstTargetName = xstrdup(target.name);
    }

    resized = xrealloc(model->targets,
            sizeof(Target) * (model->targetCount + 1));

    model->targets = resized;
    model->targets[model->targetCount] = target;
    model->targetCount++;

    if (model->firstTargetName == NULL) {
        model->firstTargetName = firstTargetName;
    }

    return true;
}

static bool parse_variable_tokens(UQMakefileModel* model, char** tokens,
        unsigned int tokenCount, const char* filename, int lineNumber)
{
    if (!lexer_is_valid_variable_definition(tokens, tokenCount)) {
        print_variable_line_error(filename, lineNumber);
        return true;
    }

    return !handle_variable_line(model, tokens, tokenCount, lineNumber);
}

static bool parse_dependency_tokens(UQMakefileModel* model, char** tokens,
        unsigned int tokenCount, const char* filename, int lineNumber,
        HashMap* seenTargetNames)
{
    if (!lexer_is_valid_dependency_definition(tokens, tokenCount)) {
        print_dependency_line_error(filename, lineNumber);
        return true;
    }

    if (target_name_already_seen(seenTargetNames, tokens[0])) {
        print_repeated_target_error(filename, lineNumber);
        return true;
    }

    if (!handle_dependency_line(model, tokens, tokenCount, lineNumber)) {
        return true;
    }
    hashmap_put(seenTargetNames, tokens[0], seenTargetNames);
    return false;
}

static bool parse_phony_tokens(UQMakefileModel* model, char** tokens,
        unsigned int tokenCount, const char* filename, int lineNumber,
        HashMap* seenTargetNames)
{
    if (!lexer_is_valid_phony_target_definition(tokens, tokenCount)) {
        print_phony_line_error(filename, lineNumber);
        return true;
    }

    if (target_name_already_seen(seenTargetNames, tokens[0])) {
        print_repeated_target_error(filename, lineNumber);
        return true;
    }

    if (!handle_phony_line(model, tokens, tokenCount, lineNumber)) {
        return true;
    }
    hashmap_put(seenTargetNames, tokens[0], seenTargetNames);
    return false;
}

static bool parse_line_tokens(UQMakefileModel* model, char** tokens,
        unsigned int tokenCount, const char* filename, int lineNumber,
        HashMap* seenTargetNames)
{
    if (tokenCount >= 2 && strcmp(tokens[1], "=") == 0) {
        return parse_variable_tokens(
                model, tokens, tokenCount, filename, lineNumber);
    }

    if (tokenCount >= 2 && strcmp(tokens[1], ":") == 0) {
        return parse_dependency_tokens(
                model, tokens, tokenCount, filename, lineNumber,
                seenTargetNames);
    }

    if (tokenCount >= 2 && strcmp(tokens[1], "~") == 0) {
        return parse_phony_tokens(
                model, tokens, tokenCount, filename, lineNumber,
                seenTargetNames);
    }

    print_line_not_valid_error(filename, lineNumber);
    return true;
}

bool parse_makefile(FILE* fp, const char* filename,
        UQMakefileModel* outModel)
{
    char* line = NULL;
    size_t lineCapacity = 0;
    int lineNumber = 0;
    bool foundError = false;
    HashMap* seenTargetNames;

    memset(outModel, 0, sizeof(UQMakefileModel));
    seenTargetNames = hashmap_create(64);

    while (getline(&line, &lineCapacity, fp) != -1) {
        unsigned int tokenCount = 0;
        char** tokens = lexer_tokenize_line(line, &tokenCount);

        lineNumber++;
        if (tokenCount != 0 && tokens[0][0] != '#') {
            foundError |= parse_line_tokens(
                    outModel, tokens, tokenCount, filename, lineNumber,
                    seenTargetNames);
        }
        lexer_free_tokens(tokens, tokenCount);
    }

    free(line);
    hashmap_free(seenTargetNames, NULL);

    if (foundError) {
        free_uqmakefile_model(outModel);
        return false;
    }

    if (outModel->targetCount == 0) {
        log_error("no targets were found in file \"%s\"\n", filename);
        free_uqmakefile_model(outModel);
        return false;
    }

    outModel->targetMap = hashmap_create((size_t)outModel->targetCount * 2);
    for (int i = 0; i < outModel->targetCount; i++) {
        hashmap_put(outModel->targetMap, outModel->targets[i].name,
                &outModel->targets[i]);
    }

    return true;
}

static bool handle_variable_line(UQMakefileModel* model, char** tokens,
        unsigned int tokenCount, int lineNumber)
{
    VariableDef def = {0};

    def.name = xstrdup(tokens[0]);
    strvec_init(&def.rawTokens);
    for (unsigned int i = 2; i < tokenCount; i++) {
        strvec_push(&def.rawTokens, tokens[i]);
    }
    def.lineNumber = lineNumber;

    if (!append_variable_def(model, def)) {
        free(def.name);
        strvec_free(&def.rawTokens);
        return false;
    }

    return true;
}

static bool handle_dependency_line(UQMakefileModel* model, char** tokens,
        unsigned int tokenCount, int lineNumber)
{
    Target target = {0};

    target.type = TARGET_FILE;
    target.name = xstrdup(tokens[0]);
    strvec_init(&target.data.fileTarget.rawDeps);
    strvec_init(&target.data.fileTarget.expandedDeps);
    for (unsigned int i = 2; i < tokenCount; i++) {
        strvec_push(&target.data.fileTarget.rawDeps, tokens[i]);
    }
    target.data.fileTarget.depsExpanded = false;
    target.data.fileTarget.lineNumber = lineNumber;

    if (!append_target(model, target)) {
        free(target.name);
        strvec_free(&target.data.fileTarget.rawDeps);
        strvec_free(&target.data.fileTarget.expandedDeps);
        return false;
    }

    return true;
}

static bool handle_phony_line(UQMakefileModel* model, char** tokens,
        unsigned int tokenCount, int lineNumber)
{
    Target target = {0};

    target.type = TARGET_PHONY;
    target.name = xstrdup(tokens[0]);
    strvec_init(&target.data.phonyTarget.rawPipeline);
    strvec_init(&target.data.phonyTarget.expandedPipeline);
    for (unsigned int i = 2; i < tokenCount; i++) {
        strvec_push(&target.data.phonyTarget.rawPipeline, tokens[i]);
    }
    target.data.phonyTarget.pipelineExpanded = false;
    target.data.phonyTarget.lineNumber = lineNumber;

    if (!append_target(model, target)) {
        free(target.name);
        strvec_free(&target.data.phonyTarget.rawPipeline);
        strvec_free(&target.data.phonyTarget.expandedPipeline);
        return false;
    }

    return true;
}

static bool target_name_already_seen(const HashMap* seenTargetNames,
        const char* name)
{
    return hashmap_get(seenTargetNames, name) != NULL;
}

Target* find_target_by_name(UQMakefileModel* model, const char* name)
{
    return (Target*)hashmap_get(model->targetMap, name);
}

bool check_requested_targets_exist(const UQMakefileModel* model,
        const CommandLineOptions* options, const char* filename)
{
    bool foundMissing = false;

    for (int i = 0; i < options->requestedTargets.count; i++) {
        const char* targetName = options->requestedTargets.data[i];

        if (find_target_by_name((UQMakefileModel*)model, targetName) == NULL) {
            log_error("unable to find target \"%s\" in file \"%s\"\n",
                    targetName, filename);
            foundMissing = true;
        }
    }

    return !foundMissing;
}

static void free_variable_defs(UQMakefileModel* model)
{
    for (int i = 0; i < model->variableDefCount; i++) {
        VariableDef* def = &model->variableDefs[i];

        free(def->name);
        strvec_free(&def->rawTokens);
    }

    free(model->variableDefs);
}

static void free_file_target(FileTarget* fileTarget)
{
    strvec_free(&fileTarget->rawDeps);
    strvec_free(&fileTarget->expandedDeps);
}

static void free_phony_target(PhonyTarget* phonyTarget)
{
    strvec_free(&phonyTarget->rawPipeline);
    strvec_free(&phonyTarget->expandedPipeline);
}

static void free_targets(UQMakefileModel* model)
{
    for (int i = 0; i < model->targetCount; i++) {
        Target* target = &model->targets[i];

	free(target->name);

        if (target->type == TARGET_FILE) {
            free_file_target(&target->data.fileTarget);
        } else {
            free_phony_target(&target->data.phonyTarget);
        }
    }

    free(model->targets);
}

static void reset_model(UQMakefileModel* model)
{
    model->variableDefs = NULL;
    model->variableDefCount = 0;
    model->targets = NULL;
    model->targetCount = 0;
    model->firstTargetName = NULL;
    model->targetMap = NULL;
}

void free_uqmakefile_model(UQMakefileModel* model)
{
    if (model == NULL) {
        return;
    }

    free_variable_defs(model);
    free_targets(model);
    hashmap_free(model->targetMap, NULL);
    free(model->firstTargetName);

    reset_model(model);
}
