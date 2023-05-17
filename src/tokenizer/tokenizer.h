#pragma once

#define TOKEN_MAX_VAL_LENGTH 128
#define TOKEN_MAX_AMOUNT 128

typedef enum {
    TokenEof,
    TokenIdentifier, 
    TokenNumber, 
    TokenEquals, 
    TokenBinaryOperator, 
    TokenOpenParen,
    TokenCloseParen
} TokenType;

typedef struct {
    char val[TOKEN_MAX_VAL_LENGTH];
    TokenType type;
} Token;

int tokenize(const char *str, Token* tokens);
void print_tokens(Token* tokens, int size);
void print_token(Token token);