#include "../common.h"
#include "parser.h"

void compile(const char* source) {
    initScanner(source);
    for (int i=0; i<100; i++) {
        Token token = scan_token();
        printf("%s\n", token_t_strings[token.type]);
    }
}