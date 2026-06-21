#ifndef FRONTEND_CLI_H
#define FRONTEND_CLI_H

#include <stdbool.h>

#include "../core/types.h"

CommandLineOptions parse_command_line(int argc, char* argv[]);
bool validate_cli_variable_definitions(const CommandLineOptions* options);
void free_command_line_options(CommandLineOptions* options);

#endif /* FRONTEND_CLI_H */
