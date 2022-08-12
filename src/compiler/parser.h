#ifndef PARSER_H
#define PARSER_H

typedef struct {
    int row;
    int col;
    const char* start;
    const char* current;
} Scanner;

typedef enum {
    TOKEN_INV,
    TOKEN_INT,
    TOKEN_FLT,
    TOKEN_STR,
    TOKEN_OBR,
    TOKEN_CBR,
    TOKEN_QUO,
    TOKEN_SYM,
    TOKEN_IF,
    TOKEN_MIN,
    TOKEN_ADD,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_MOD,
    TOKEN_LET,
    TOKEN_DEF,
    TOKEN_LTE,
    TOKEN_GTE,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_T,
    TOKEN_F,
    TOKEN_EOF,
} Token_t;

typedef struct {
    Token_t type;
    const char* start;
    int length;
    int row;
    int col;
} Token;

extern const char *token_t_strings[];

void init_scanner(const char* source);

Token scan_token();

void print_token(Token token);

#endif