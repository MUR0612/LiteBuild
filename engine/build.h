#ifndef ENGINE_BUILD_H
#define ENGINE_BUILD_H

#include <stdbool.h>

#include "../core/types.h"
#include "../core/variable.h"

bool build_requested_targets(BuildFileModel* model, VariableTable* vars,
        const CommandLineOptions* options, RuntimeState* runtime);

#endif /* ENGINE_BUILD_H */
