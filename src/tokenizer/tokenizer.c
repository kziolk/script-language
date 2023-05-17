#include <stdio.h>
#include <string.h>

#include "tokenizer.h"

void print_token(Token token) 
{
    switch (token.type) {
        case TokenEof: printf("EOF"); break;
        case TokenIdentifier: printf("IDENTIFIER"); break;
        case TokenNumber: printf("NUMBER"); break;
        case TokenEquals: printf("EQUALS"); break;
        case TokenBinaryOperator: printf("BINARY_OP"); break;
        case TokenOpenParen: printf("OPEN_PAREN"); break;
        case TokenCloseParen: printf("CLOSED_PAREN"); break;
    }
    printf("(%s)", token.val);
}

void print_tokens(Token* tokens, int size) 
{
    printf("Tokens:\n");
    for (int i = 0; i < size; i++) {
        print_token(tokens[i]);
        // spacing
        if (i % 4 == 3) 
            printf("\n");
        else printf("\t");
    }
    printf("\n\n");
}

void create_token_binary(Token* token, char val) 
{
    token->val[0] = val;
    token->val[1] = '\0';
    token->type = TokenBinaryOperator;
}

void create_token_equals(Token* token, char val) 
{
    token->val[0] = val;
    token->val[1] = '\0';
    token->type = TokenEquals;
}

void create_token_open_paren(Token* token, char val) 
{
    token->val[0] = val;
    token->val[1] = '\0';
    token->type = TokenOpenParen;
}

void create_token_closed_paren(Token* token, char val) 
{
    token->val[0] = val;
    token->val[1] = '\0';
    token->type = TokenCloseParen;
}

void create_token_number(Token* token, const char* str, int num_len) 
{
    int i = 0;
    while(i < num_len) {
        token->val[i] = str[i];
        i++;
    }
    token->val[num_len] = '\0';
    token->type = TokenNumber;
}

void create_token_identifier(Token* token, const char* str, int var_len) 
{
    int i = 0;
    while(i < var_len) {
        token->val[i] = str[i];
        i++;
    }
    token->val[var_len] = '\0';
    token->type = TokenIdentifier;
}

void create_token_eof(Token* token)
{
    strcpy(token->val, "EndOfFile");
    token->type = TokenEof;
}

int isAlpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int isNumber(char c) 
{
    return c >= '0' && c <= '9';
}

int isBlank(char c) 
{
    return (c == ' ') || (c == '\t');
}

int detect_number(const char* str) 
{
    int i = 0;
    while (isNumber(str[i])) {
        i++;
    }
    return i;
}

int detect_variable(const char* str) 
{
    int i = 0;
    while (isAlpha(str[i])) {
        i++;
    }
    return i;
}

int tokenize(const char *str, Token* tokens) 
{
    unsigned int size = 0;

    for(int i = 0; str[i] != '\0'; i++) {
        if (size == TOKEN_MAX_AMOUNT - 1) {
            // exceeding max amount
                printf("Encountered characters after reaching max tokens (i == %d).\nTokenizer aborted.\n", i);
                return -1;
        }

        char c = str[i];
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
            create_token_binary(&tokens[size++], c);
        else if (c == '=')
            create_token_equals(&tokens[size++], c);
        else if (c == '(')
            create_token_open_paren(&tokens[size++], c);
        else if (c == ')')
            create_token_closed_paren(&tokens[size++], c);
        else {
            // handle number
            if (c >= '1' && c <= '9') {
                int number_of_digits = 1 + detect_number(&str[i+1]);
                create_token_number(&tokens[size++], &str[i], number_of_digits);
                i += number_of_digits - 1;
            }
            // handle Variable
            else if (isAlpha(c)) {
                int var_len = detect_variable(&str[i]);
                create_token_identifier(&tokens[size++], &str[i], var_len);
                i += var_len - 1;

            }
            // ignore whitespace
            else if (isBlank(c)) {
                ;
            }
            // abort on unsupported characters
            else {
                printf("Unhandled character [%c].\nTokenizer aborted.\n", c);
                return -1;
            }
        }
    }
    create_token_eof(&tokens[size++]);
    return size;
}