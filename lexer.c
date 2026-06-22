#include "lexer.h"

#include "utils/memory.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/* --- Lexer implementation --- */

char** lexer_tokenize_line(const char* line, unsigned int* numTokens) {
    if (!line || !numTokens) return NULL;
    
    char* line_copy = xstrdup(line);
    unsigned int capacity = 10;
    char** tokens = xmalloc(capacity * sizeof(char*));
    unsigned int count = 0;

    char* saveptr;
    char* token = strtok_r(line_copy, " \t\r\n\v\f", &saveptr);
    
    while (token != NULL) {
        if (count >= capacity) {
            capacity *= 2;
            tokens = xrealloc(tokens, capacity * sizeof(char*));
        }
        tokens[count++] = xstrdup(token);
        token = strtok_r(NULL, " \t\r\n\v\f", &saveptr);
    }
    
    free(line_copy);
    *numTokens = count;
    return tokens;
}

void lexer_free_tokens(char** tokens, unsigned int numTokens) {
    if (!tokens) return;
    for (unsigned int i = 0; i < numTokens; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

/* --- Validation logic --- */

bool lexer_is_valid_identifier(const char* token) {
    if (!token || !isalpha(token[0])) return false;
    for (int i = 1; token[i] != '\0'; i++) {
        if (!isalnum(token[i]) && token[i] != '_') return false;
    }
    return true;
}

bool lexer_is_valid_filename(const char* token) {
    // Check token[0] directly instead of calling strlen for an empty string.
    if (!token || token[0] == '\0') return false; 
    const char* invalid_chars = ":=>/#\\$~";
    for (int i = 0; token[i] != '\0'; i++) {
        if (isspace(token[i]) || strchr(invalid_chars, token[i])) return false;
    }
    return true;
}

bool lexer_is_valid_variable_expansion(const char* token) {
    if (!token || token[0] != '$') return false;
    return lexer_is_valid_identifier(token + 1);
}

/* --- Statement validation --- */

bool lexer_is_valid_variable_definition(char** tokens, unsigned int numTokens) {
    if (numTokens < 2) return false;
    if (!lexer_is_valid_identifier(tokens[0])) return false;
    if (strcmp(tokens[1], "=") != 0) return false;
    return true;
}

bool lexer_is_valid_dependency_definition(char** tokens, unsigned int numTokens) {
    if (numTokens < 2) return false;
    if (!lexer_is_valid_filename(tokens[0])) return false;
    if (strcmp(tokens[1], ":") != 0) return false;
    return true;
}

bool lexer_is_valid_phony_target_definition(char** tokens, unsigned int numTokens) {
    if (numTokens < 2) return false;
    if (!lexer_is_valid_identifier(tokens[0])) return false;
    if (strcmp(tokens[1], "~") != 0) return false;
    return true;
}
