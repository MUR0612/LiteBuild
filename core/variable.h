#ifndef CORE_VARIABLE_H
#define CORE_VARIABLE_H

#include "types.h"

VariableTable build_variable_table(
        const UQMakefileModel* model, const CommandLineOptions* options);
void set_variable(VariableTable* table, const char* name, StringVector values);
Variable* lookup_variable(VariableTable* table, const char* name);
StringVector expand_tokens_once(const VariableTable* table,
        const StringVector* rawTokens);
void free_variable_table(VariableTable* table);

#endif /* CORE_VARIABLE_H */
