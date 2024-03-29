//> Scanning on Demand scanner-c
#include "../common.h"
#include "parser.h"
#include "trie.h"

Scanner scanner;

Trie* keywords;

const char *token_t_strings[] = {
    "TOKEN_INV", "TOKEN_INT", "TOKEN_FLT", "TOKEN_STR", "TOKEN_OBR",
    "TOKEN_CBR", "TOKEN_QUO", "TOKEN_SYM", "TOKEN_IF",  "TOKEN_MIN",
    "TOKEN_ADD", "TOKEN_MUL", "TOKEN_DIV", "TOKEN_MOD", "TOKEN_LET",
    "TOKEN_DEF", "TOKEN_LTE", "TOKEN_GTE", "TOKEN_LT",  "TOKEN_GT",
    "TOKEN_T",   "TOKEN_F",   "TOKEN_EOF",
};

// define keywords
void init_keywords() {
#define K(form, token) (trie_insert(keywords, form, token))
    keywords = trie_new_node();
    K("if",     TOKEN_IF);
    K("let",    TOKEN_LET);
    K("define", TOKEN_IF);
    K("quote",  TOKEN_QUO);
#undef K
}

void init_scanner(const char* source) {
    init_keywords();
    scanner.start = source;
    scanner.current = source;
    scanner.row = 1;
    scanner.col = 0;
}

void print_token(Token token) {
    printf("[%d:%d] %s: %.*s\n",
           token.row,
           token.col,
           token_t_strings[token.type],
           token.length,
           token.start);
}

static bool is_char(char c) {
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
}

static inline bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static inline bool is_end() {
    return *scanner.current == '\0';
}

static inline char advance() {
    scanner.current++;
    scanner.col++;
    return scanner.current[-1];
}

static inline char peek() {
    return *scanner.current;
}

static inline char peek_next() {
    if (is_end()) return '\0';
    return scanner.current[1];
}

static inline Token make_token(Token_t type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.row = scanner.row;
    token.col = scanner.col - token.length;
    return token;
}

static inline Token invalid_token(const char* message) {
    Token token;
    token.type = TOKEN_INV;
    token.start = message;
    token.length = (int)strlen(message);
    token.row = scanner.row;
    token.col = scanner.col;
    return token;
}

static inline void handle_whitespace() {
    for (;;) {
        char c = peek();
        switch (c) {
        case ' ':
        case '\r':
        case '\t':
            advance();
            break;
        case '\n':
            scanner.row++;
            advance();
            scanner.col = 0;
            break;
        default:
            return;
        }
    }
}

static inline Token number() {
    Token_t type = TOKEN_INT;
    while (is_digit(peek())) advance();

    if (peek() == '.' && is_digit(peek_next())) {
        type = TOKEN_FLT;
        advance();
        while (is_digit(peek())) advance();
    }

    return make_token(type);
}

static inline Token string() {
    while (peek() != '"' && !is_end()) {
        if (peek() == '\n') scanner.row++;
        advance();
    }

    if (is_end()) return invalid_token("Unterminated string.");
    advance();
    return make_token(TOKEN_STR);
}

static inline Token_t identifier_type() {
    int length = scanner.current - scanner.start;
    char str[length];
    strncpy(str, scanner.start, length);
    return trie_get_token(keywords, str);
}

static inline Token identifier() {
    while (is_char(peek()) || is_digit(peek())) {
        advance();
    }
    return make_token(identifier_type());
}

static inline Token less_than() {
    if (peek() == '=') {
        advance();
        return make_token(TOKEN_LTE);
    }
    return make_token(TOKEN_LT);
}

static inline Token more_than() {
    if (peek() == '=') {
        advance();
        return make_token(TOKEN_GTE);
    }
    return make_token(TOKEN_GT);
}

Token scan_token() {
    handle_whitespace();
    scanner.start = scanner.current;

    if (is_end())
        return make_token(TOKEN_EOF);

    char c = advance();
    if (is_digit(c))
        return number();

    if (is_char(c))
        return identifier();

    if (c == '<')// including <=
        return less_than();

    if (c == '>') // including >=
        return more_than();

    switch (c) {
    case '(':  return make_token(TOKEN_OBR);
    case ')':  return make_token(TOKEN_CBR);
    case '\'': return make_token(TOKEN_QUO);
    case '+':  return make_token(TOKEN_ADD);
    case '-':  return make_token(TOKEN_MIN);
    case '*':  return make_token(TOKEN_MUL);
    case '/':  return make_token(TOKEN_DIV);
    case '"':  return string();
    }

    return invalid_token("Unexpected character");
}