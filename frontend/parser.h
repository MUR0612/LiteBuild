#ifndef FRONTEND_PARSER_H
#define FRONTEND_PARSER_H

#include <stdbool.h>
#include <stdio.h>

#include "../core/types.h"

bool parse_makefile(FILE* fp, const char* filename,
        UQMakefileModel* outModel);
bool check_requested_targets_exist(const UQMakefileModel* model,
        const CommandLineOptions* options, const char* filename);
Target* find_target_by_name(UQMakefileModel* model, const char* name);
void free_uqmakefile_model(UQMakefileModel* model);

#endif /* FRONTEND_PARSER_H */
