#include "../common.h"
#include "parser.h"

void compile(const char* source) {
    init_scanner(source);
    for (;;) {
        Token token = scan_token();
        print_token(token);
        if (token.type == TOKEN_EOF) break;
    }
}