#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

/* 切分字串並回傳 tokens 陣列，numTokens 會被賦值為 token 的數量 */
char** lexer_tokenize_line(const char* line, unsigned int* numTokens);

/* 釋放 tokens 陣列及內部字串 */
void lexer_free_tokens(char** tokens, unsigned int numTokens);

/* 單一 Token 驗證 */
bool lexer_is_valid_identifier(const char* token);
bool lexer_is_valid_filename(const char* token);
bool lexer_is_valid_variable_expansion(const char* token);

/* 語法行驗證 */
bool lexer_is_valid_variable_definition(char** tokens, unsigned int numTokens);
bool lexer_is_valid_dependency_definition(char** tokens, unsigned int numTokens);
bool lexer_is_valid_phony_target_definition(char** tokens, unsigned int numTokens);

#endif /* LEXER_H */