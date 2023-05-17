#pragma once
#include "../tokenizer/tokenizer.h"

#define PARSER_MAX_STATEMENTS 128

typedef enum {
    StmtIdentifier,
    StmtNumericLiteral,
    StmtExpr,
    StmtBinaryExpr
} StmtType;

typedef struct {
    StmtType subtype;
    StmtType type;
    void* val;
} Stmt;

struct Identifier {
    char symbol[TOKEN_MAX_VAL_LENGTH];
};

struct NumericLiteral {
    int value;
};

struct BinaryExpr {
    Stmt* left;
    Stmt* right;
    char op[TOKEN_MAX_VAL_LENGTH];
};

typedef struct {
    Stmt body[PARSER_MAX_STATEMENTS];
    int size;
} Program;

int parse(Token* in, Program* out);
void print_program(Program* program);
void free_program(Program* program);