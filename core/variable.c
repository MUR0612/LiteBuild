/** @file variable.c
 * @author Frank WU
 * @brief Variable table and expansion helpers for litebuild.
 */
#include "variable.h"

#include <stdlib.h>
#include <string.h>

#include "../lexer.h"
#include "../utils/logger.h"
#include "../utils/memory.h"

static void apply_variable_definition(
        VariableTable* table, const char* definitionText);
static void free_variable_cb(void* val);

static StringVector make_vector_from_values(const char* const* values,
        int count)
{
    StringVector vector;

    strvec_init(&vector);
    for (int i = 0; i < count; i++) {
        strvec_push(&vector, values[i]);
    }
    return vector;
}

VariableTable build_variable_table(
        const UQMakefileModel* model, const CommandLineOptions* options)
{
    VariableTable table;
    const char* compiler[] = {"gcc"};
    const char* linker[] = {"gcc"};
    const char* compileFlags[] = {"-Wall", "-Wextra", "-pedantic"};

    table.map = hashmap_create(64);

    set_variable(&table, "COMPILER", make_vector_from_values(compiler, 1));
    set_variable(&table, "LINKER", make_vector_from_values(linker, 1));
    set_variable(&table, "COMPILEFLAGS", make_vector_from_values(
            compileFlags, DEFAULT_COMPILE_FLAGS_COUNT));
    set_variable(&table, "LINKFLAGS", make_vector_from_values(NULL, 0));

    for (int i = 0; i < model->variableDefCount; i++) {
        VariableDef* def = &model->variableDefs[i];
        StringVector expanded = expand_tokens_once(&table, &def->rawTokens);

        set_variable(&table, def->name, expanded);
    }

    for (int i = 0; i < options->cliVarDefs.count; i++) {
        apply_variable_definition(&table, options->cliVarDefs.data[i]);
    }

    return table;
}

void set_variable(VariableTable* table, const char* name, StringVector values)
{
    Variable* existing = (Variable*)hashmap_get(table->map, name);
    Variable* newVar;

    if (existing != NULL) {
        strvec_free(&existing->values);
        existing->values = values;
        return;
    }

    newVar = xmalloc(sizeof(Variable));
    newVar->name = xstrdup(name);
    newVar->values = values;
    hashmap_put(table->map, name, newVar);
}

Variable* lookup_variable(VariableTable* table, const char* name)
{
    return (Variable*)hashmap_get(table->map, name);
}

StringVector expand_tokens_once(const VariableTable* table,
        const StringVector* rawTokens)
{
    StringVector expanded;

    strvec_init(&expanded);
    for (int i = 0; i < rawTokens->count; i++) {
        char* token = rawTokens->data[i];

        if (lexer_is_valid_variable_expansion(token)) {
            const char* name = token + 1;
            Variable* var = lookup_variable((VariableTable*)table, name);

            if (var == NULL) {
                log_warn("Unknown variable: \"%s\"\n", name);
                continue;
            }

            for (int j = 0; j < var->values.count; j++) {
                strvec_push(&expanded, var->values.data[j]);
            }
        } else {
            strvec_push(&expanded, token);
        }
    }
    return expanded;
}

void free_variable_table(VariableTable* table)
{
    hashmap_free(table->map, free_variable_cb);
    table->map = NULL;
}

static void free_variable_cb(void* val)
{
    Variable* var = (Variable*)val;

    free(var->name);
    strvec_free(&var->values);
    free(var);
}

static void apply_variable_definition(
        VariableTable* table, const char* definitionText)
{
    unsigned int tokenCount = 0;
    char** tokens;
    StringVector rawTokens;
    StringVector expanded;

    if (definitionText == NULL) {
        return;
    }

    tokens = lexer_tokenize_line(definitionText, &tokenCount);
    strvec_init(&rawTokens);
    for (unsigned int i = 2; i < tokenCount; i++) {
        strvec_push(&rawTokens, tokens[i]);
    }

    expanded = expand_tokens_once(table, &rawTokens);
    set_variable(table, tokens[0], expanded);

    strvec_free(&rawTokens);
    lexer_free_tokens(tokens, tokenCount);
}
