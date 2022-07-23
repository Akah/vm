#include "parser.h"

Scanner scanner;

const char *token_t_strings[] = {
    "TOKEN_INV", "TOKEN_INT", "TOKEN_FLT", "TOKEN_STR",
    "TOKEN_OBR", "TOKEN_CBR", "TOKEN_QUO", "TOKEN_SYM",
};

void initScanner(const char* source) {
    scanner.col = 0;
    scanner.row = 1;
    scanner.current = source;
    scanner.start = source;
}

static Token make_token(Token_t type) {
    Token token;
    token.type = type;
    token.col = scanner.col;
    token.row = scanner.row;
    token.start = scanner.current;
    token.length = (int)(scanner.current -scanner.start);
    return token;
}

static inline bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static inline bool is_op(char c) {
    return c == '+'
        || c == '-'
        || c == '/'
        || c == '*'
        || c == '%';
}

static inline bool is_char(char c) {
    return (c >= 'A' && c <= 'Z')
        || (c >= 'a' && c <= 'z')
        || c == '_'
        || is_op(c);
}

Token scan_token() {
    while(*scanner.current != '\0') {
        if (peek(scanner.current) == ' ') {
            scanner.current++;
            scanner.col++;
            continue;
        }
        if (peek(scanner.current) == '\n') {
            scanner.current++;
            scanner.col++;
            scanner.row++;
            continue;
        }
        if (peek(scanner.current) == '\'') {
            scanner.current++;
            scanner.col++;
            return make_token(TOKEN_QUO);
        }
        if (peek(scanner.current) == '+') {
            scanner.current++;
            scanner.col;
        }
        if (is_digit(peek(scanner.current))) {
            Token_t type = TOKEN_INT;
            while (is_digit(peek(scanner.current)) || peek(scanner.current) == '.') {
                const char* next = scanner.current + 1;
                if (peek(scanner.current) == '.') {
                    if (!is_digit(*next)) {
                        type = TOKEN_INV;
                    } else {
                        type = TOKEN_FLT;
                    }
                }
                scanner.col++;
            }
            return make_token(type);
        }
        return make_token(TOKEN_INT);
    }
    return (Token){};
}