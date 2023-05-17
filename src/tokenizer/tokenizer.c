#include <stdio.h>

#include "tokenizer.h"

void printToken(Token token) 
{
    switch (token.type) {
        case Variable: printf("VARIABLE"); break;
        case Number: printf("NUMBER"); break;
        case Equals: printf("EQUALS"); break;
        case BinaryOperator: printf("BINARY_OPERATOR"); break;
    }
    printf("(%s)", token.val);
}

void print_tokens(Token* tokens) 
{
    for (int i = 0; tokens[i].type != Null; i++) {
        printToken(tokens[i]);
        // spacing
        if (i % 4 == 3) 
            printf("\n");
        else printf("\t");
    }
    printf("\n");
}

void create_token_binary(Token* token, char val) 
{
    token->val[0] = val;
    token->val[1] = '\0';
    token->type = BinaryOperator;
}

void create_token_equals(Token* token, char val) 
{
    token->val[0] = val;
    token->val[1] = '\0';
    token->type = Equals;
}

void create_token_number(Token* token, const char* str, int num_len) 
{
    int i = 0;
    while(i < num_len) {
        token->val[i] = str[i];
        i++;
    }
    token->val[num_len] = '\0';
    token->type = Number;
}

void create_token_variable(Token* token, const char* str, int var_len) 
{
    int i = 0;
    while(i < var_len) {
        token->val[i] = str[i];
        i++;
    }
    token->val[var_len] = '\0';
    token->type = Variable;
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

void tokenize(const char *str, Token* tokens) 
{
    unsigned int size = 0;

    for(int i = 0; str[i] != '\0'; i++) {
        if (size == TOKEN_MAX_AMOUNT) {
            // exceeding max amount
                printf("Encountered characters after reaching max tokens (i == %d).\nTokenizer aborted.", i);
                return;
        }

        char c = str[i];
        if (c == '+' || c == '-' || c == '*' || c == '*')
            create_token_binary(&tokens[size++], c);
        else if (c == '=')
            create_token_equals(&tokens[size++], c);
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
                create_token_variable(&tokens[size++], &str[i], var_len);
                i += var_len - 1;

            }
            // ignore whitespace
            else if (isBlank(c)) {
                ;
            }
            // abort on unsupported characters
            else {
                printf("Unhandled character [%c].\nTokenizer aborted.", c);
                return;
            }
        }
    }
    tokens[size].type = Null;
}