/** @file cli.c
 * Command line parsing and early checks for litebuild.
 * @author Frank Wu
 */
#include "cli.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lexer.h"
#include "../utils/logger.h"

/**
 * Print the required usage message to stderr and exit.
 * This is used when the command line format is invalid.
 */
static void print_usage_and_exit(void)
{
    fprintf(stderr,
            "Usage: " PROJECT_NAME " [--file filename] [--simulate] "
            "[var=value]... [target]...\n");
    exit(EXIT_FAILURE);
}

/**
 * Creates a CommandLineOptions struct with default values.
 *
 * @return A CommandLineOptions struct with default initial values.
 */
static CommandLineOptions make_default_options(void)
{
    CommandLineOptions options;

    options.makefileName = DEFAULT_MAKEFILE;
    options.simulate = false;
    strvec_init(&options.cliVarDefs);
    strvec_init(&options.requestedTargets);

    return options;
}

/**
 * Processes an option argument and updates command line options.
 *
 * @param arg current argument string.
 * @param argv full argument array.
 * @param index pointer to current argument index.
 * @param options command line options to update.
 * @return This function does not return a value.
 */
static void handle_option(
        char* arg, char* argv[], int* index,
        CommandLineOptions* options, bool* seenFileOption,
        bool* seenSimulateOption)
{
    if (strcmp(arg, "--file") == 0) {
        if (*seenFileOption) {
            print_usage_and_exit();
        }
        *seenFileOption = true;
        (*index)++;
        if (argv[*index] == NULL || strlen(argv[*index]) == 0) {
            print_usage_and_exit();
        }
        options->makefileName = argv[*index];
    } else if (strcmp(arg, "--simulate") == 0) {
        if (*seenSimulateOption) {
            print_usage_and_exit();
        }
        *seenSimulateOption = true;
        options->simulate = true;
    } else {
        print_usage_and_exit();
    }
}

/**
 * Classifies a non-option argument as a variable definition or target.
 *
 * @param arg argument string to classify.
 * @param options command line options to update.
 * @return This function does not return a value.
 */
static void handle_non_option(
        char* arg, CommandLineOptions* options)
{
    if (strchr(arg, '=') != NULL) {
        strvec_push(&options->cliVarDefs, arg);
    } else {
        strvec_push(&options->requestedTargets, arg);
    }
}

/**
 * Parses and validates command line arguments.
 *
 * @param argc number of command line arguments.
 * @param argv argument vector.
 * @return Parsed command line options.
 */
CommandLineOptions parse_command_line(int argc, char* argv[])
{
    CommandLineOptions options = make_default_options();
    bool seenNonOption = false;
    bool seenFileOption = false;
    bool seenSimulateOption = false;

    // Read arguments in order and stop option parsing after the first target.
    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];

        if (strlen(arg) == 0) {
            print_usage_and_exit();
        }

        if (!seenNonOption && strncmp(arg, "--", 2) == 0) {
            handle_option(arg, argv, &i, &options, &seenFileOption,
                    &seenSimulateOption);
        } else {
            seenNonOption = true;
            handle_non_option(arg, &options);
        }
    }

    return options;
}

/**
 * Validates command line variable definitions.
 *
 * @param options pointer to command line options.
 * @return true if all variable definitions are valid, otherwise false.
 */
bool validate_cli_variable_definitions(const CommandLineOptions* options)
{
    bool hasError = false;

    // Check each command line variable definition before continuing.
    for (int i = 0; i < options->cliVarDefs.count; i++) {
        char* definition = options->cliVarDefs.data[i];
        unsigned int tokenCount = 0;
        char** tokens = lexer_tokenize_line(definition, &tokenCount);

        if (!lexer_is_valid_variable_definition(tokens, tokenCount)) {
            log_error("invalid variable definition: \"%s\"\n", definition);
            hasError = true;
        }

        lexer_free_tokens(tokens, tokenCount);
    }

    return !hasError;
}

/**
 * Frees memory associated with CommandLineOptions.
 *
 * @param options pointer to the options struct to free.
 * @return This function does not return a value.
 */
void free_command_line_options(CommandLineOptions* options)
{
    strvec_free(&options->cliVarDefs);
    strvec_free(&options->requestedTargets);
}
