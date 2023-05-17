#pragma once

#define TOKEN_MAX_VAL_LENGTH 128
#define TOKEN_MAX_AMOUNT 128

typedef enum {
    Null,
    Variable, 
    Number, 
    Equals, 
    BinaryOperator
} TokenType;

typedef struct {
    char val[TOKEN_MAX_VAL_LENGTH];
    TokenType type;
} Token;

void tokenize(const char *str, Token* tokens);
void print_tokens(Token* tokens);