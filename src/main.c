#include "tokenizer/tokenizer.h"

int main() {
    Token tokens[TOKEN_MAX_AMOUNT];
    tokenize("number = -5 +16", tokens);
    print_tokens(tokens);
    return 0;
}
