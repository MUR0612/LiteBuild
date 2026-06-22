#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

/* Splits a string and returns a token array; numTokens receives the count. */
char** lexer_tokenize_line(const char* line, unsigned int* numTokens);

/* Frees the token array and its strings. */
void lexer_free_tokens(char** tokens, unsigned int numTokens);

/* Single-token validation. */
bool lexer_is_valid_identifier(const char* token);
bool lexer_is_valid_filename(const char* token);
bool lexer_is_valid_variable_expansion(const char* token);

/* Syntax-line validation. */
bool lexer_is_valid_variable_definition(char** tokens, unsigned int numTokens);
bool lexer_is_valid_dependency_definition(char** tokens, unsigned int numTokens);
bool lexer_is_valid_phony_target_definition(char** tokens, unsigned int numTokens);

#endif /* LEXER_H */
