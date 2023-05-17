#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"

static Token** tokens;

Token* getToken() {
    return *tokens;
}

Token* eatToken() {
    return (*tokens)++;
}

void pushStatement(Stmt* statement, Program* program)
{
    program->body[program->size++] = *statement;
}

void print_statement(Stmt* statement, int depth)
{
    char padding[32];
    memset(padding, ' ', depth);
    padding[depth] = 0;
    printf("%s", padding);
    switch (statement->subtype)
    {
        case StmtIdentifier:
        {
            struct Identifier* ident = (struct Identifier*) statement->val;
            printf("Identifier(%s)", ident->symbol);
        break;
        }
        case StmtNumericLiteral:
        {
            struct NumericLiteral* num = (struct NumericLiteral*) statement->val;
            printf("NumericLiteral(%d)", num->value);
        break;
        }
        case StmtBinaryExpr:
        {
            struct BinaryExpr* binexp = (struct BinaryExpr*) statement->val;
            printf("BinaryExpr(%s):\n", binexp->op);
            print_statement(binexp->left, depth + 4);
            printf("\n");
            print_statement(binexp->right, depth + 4);
        break;
        }
        default:
            printf("Undefined");
            break;
    }
}

void print_program(Program* program)
{
    printf("\nProgram:\n");
    for (int i = 0; i < program->size; i++)
    {
        print_statement(&program->body[i], 0);
        printf("\n");
    }
    printf("\n");
}

Stmt* parse_stmt();

int parse(Token* in, Program* out)
{
    out->size = 0;
    tokens = &in;
    while(getToken()->type != TokenEof)
    {
        Stmt* statement = parse_stmt();
        if (!statement)
            return -1;
        pushStatement(statement, out);
    }
    return 0;
}


Stmt* create_stmt_identifier()
{
    Stmt* statement = (Stmt*) malloc(sizeof(Stmt));
    Token* token = eatToken();
    statement->type = StmtExpr;
    statement->subtype = StmtIdentifier;
    struct Identifier* ident = (struct Identifier*) malloc(sizeof(struct Identifier));
    strcpy(ident->symbol, token->val);
    statement->val = ident;
    return statement;
}

Stmt* create_stmt_numericliteral()
{
    Stmt* statement = (Stmt*) malloc(sizeof(Stmt));
    Token* token = eatToken();
    statement->type = StmtExpr;
    statement->subtype = StmtNumericLiteral;
    struct NumericLiteral* num = (struct NumericLiteral*) malloc(sizeof(struct NumericLiteral));
    num->value = strtol(token->val, NULL, 10);
    statement->val = num;
    return statement;
}

Stmt* create_stmt_binary_expr(Stmt* left, Stmt* right, const char* op)
{
    Stmt* statement = (Stmt*) malloc(sizeof(statement));
    statement->type = StmtExpr;
    statement->subtype = StmtBinaryExpr;
    struct BinaryExpr* binexp = (struct BinaryExpr*) malloc(sizeof(struct BinaryExpr));
    binexp->left = left;
    binexp->right = right;
    strcpy(binexp->op, op);
    statement->val = binexp;
    return statement;
}

Stmt* parse_primary_expr()
{
    Token* token = getToken();
    switch(token->type) {
        case TokenIdentifier:
            return create_stmt_identifier();
        case TokenNumber:
            return create_stmt_numericliteral();
        case TokenOpenParen:
        {
            eatToken(); // (
            Stmt* value = parse_stmt();
            eatToken(); // )
            return value;
        }
        break;
        default:
            printf("Unexpected token while parsing primary expr: ");
            print_token(*token);
            printf("\n");
    }
    return 0;
}

Stmt* parse_multiplicative_expr()
{
    Stmt* left = parse_primary_expr();

    while(
        !strcmp("*", getToken()->val) ||
        !strcmp("/", getToken()->val) || 
        !strcmp("%", getToken()->val))
    {
        char* op = (char*) malloc(TOKEN_MAX_VAL_LENGTH);
        strcpy(op, eatToken()->val);
        Stmt* right =  parse_primary_expr();
        left = create_stmt_binary_expr(left, right, op);
    }
    return left;
}

Stmt* parse_additive_expr()
{
    Stmt* left = parse_multiplicative_expr();

    while(
        !strcmp("+", getToken()->val) || 
        !strcmp("-", getToken()->val))
    {
        const char* op = eatToken()->val;
        Stmt* right =  parse_multiplicative_expr();
        left = create_stmt_binary_expr(left, right, op);
    }
    return left;
}

Stmt* parse_stmt()
{

    // order of prescidence:

    // - assignment expr
    // - member expr
    // - function call
    // - logical expr
    // - comparison expr
    // + additive expr
    // + multiplicative expr
    // - unary expr
    // + primary expr

    return parse_additive_expr();
}

void free_statement(Stmt* statement, int free_this)
{
    switch (statement->subtype)
    {
        case StmtBinaryExpr:
        {
            struct BinaryExpr* binexp = (struct BinaryExpr*) statement->val;
            free_statement(binexp->left, 1);
            free_statement(binexp->right, 1);
        break;
        }
        default:
        break;
    }

    free(statement->val);
    if (free_this) free(statement);
}

void free_program(Program* program)
{
    for (int i = 0; i < program->size; i++)
        free_statement(&program->body[i], 0);
}