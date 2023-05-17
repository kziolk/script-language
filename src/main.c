#include <stdio.h>

#include "tokenizer/tokenizer.h"
#include "parser/parser.h"

int main() {
    const char* line = "2 - (y + 4) % x";
    printf("\ninput:\n%s\n\n", line);

    Token tokens[TOKEN_MAX_AMOUNT];
    int size = tokenize(line, tokens);
    if (size <= 0) 
        return -1;
    print_tokens(tokens, size);

    Program program;
    if (parse(tokens, &program) != 0)
        return -1;
    print_program(&program);
    free_program(&program);
    return 0;
}
