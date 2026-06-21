#ifndef ENGINE_EXEC_H
#define ENGINE_EXEC_H

#include <stdbool.h>

#include "../core/types.h"

RuntimeState init_runtime_state(void);
void install_signal_handlers(RuntimeState* runtime);
void free_runtime_state(RuntimeState* runtime);
void handle_interrupt_shutdown(RuntimeState* runtime);

bool execute_command_or_abort(char** argv, RuntimeState* runtime,
        bool simulate, const char* targetName);
int execute_pipeline(char*** commands, int* argcList, int cmdCount,
        RuntimeState* runtime, bool simulate);

bool is_valid_pipeline_after_expansion(char** tokens, int tokenCount);
char*** split_pipeline_into_commands(char** tokens, int tokenCount,
        int** argcListOut, int* cmdCountOut);

#endif /* ENGINE_EXEC_H */
