/** @file main.c
 * @author Frank WU
 * @brief Main program entry point for litebuild.
 *
 * litebuild is a lightweight, POSIX-compliant build automation tool
 * designed to parse custom makefiles, resolve dependency graphs,
 * and execute build commands via multi-stage pipelines.
 */

#include "core/types.h"
#include "core/variable.h"
#include "frontend/cli.h"
#include "frontend/parser.h"
#include "engine/build.h"
#include "engine/exec.h"
#include "utils/logger.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Starts litebuild and controls the main program flow.
 *
 * @param argc number of command line arguments.
 * @param argv command line argument strings.
 * @return 0 if the program finishes successfully.
 */
int main(int argc, char* argv[])
{
    CommandLineOptions options = parse_command_line(argc, argv);
    FILE* fp = fopen(options.makefileName, "r");
    BuildFileModel model;

    if (fp == NULL) {
        log_error("unable to open makefile \"%s\" for reading\n",
                options.makefileName);
        free_command_line_options(&options);
        return EXIT_FAILURE;
    }

    if (!validate_cli_variable_definitions(&options)) {
        fclose(fp);
        free_command_line_options(&options);
        return EXIT_FAILURE;
    }

    // Parse the BuildFile before checking requested targets.
    if (!parse_makefile(fp, options.makefileName, &model)) {
        fclose(fp);
        free_command_line_options(&options);
        return EXIT_FAILURE;
    }
    fclose(fp);

    if (!check_requested_targets_exist(&model, &options,
                options.makefileName)) {
        free_build_file_model(&model);
        free_command_line_options(&options);
        return EXIT_FAILURE;
    }

    // Build the final variable table before running any target.
    VariableTable vars = build_variable_table(&model, &options);
    RuntimeState runtime = init_runtime_state();

    install_signal_handlers(&runtime);
    if (!build_requested_targets(&model, &vars, &options, &runtime)) {
        free_runtime_state(&runtime);
        free_variable_table(&vars);
        free_build_file_model(&model);
        free_command_line_options(&options);
        return EXIT_FAILURE;
    }

    free_runtime_state(&runtime);
    free_variable_table(&vars);
    free_build_file_model(&model);
    free_command_line_options(&options);
    return EXIT_SUCCESS;
}
