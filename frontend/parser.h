#ifndef FRONTEND_PARSER_H
#define FRONTEND_PARSER_H

#include <stdbool.h>
#include <stdio.h>

#include "../core/types.h"

bool parse_makefile(FILE* fp, const char* filename,
        BuildFileModel* outModel);
bool check_requested_targets_exist(const BuildFileModel* model,
        const CommandLineOptions* options, const char* filename);
Target* find_target_by_name(BuildFileModel* model, const char* name);
void free_build_file_model(BuildFileModel* model);

#endif /* FRONTEND_PARSER_H */
