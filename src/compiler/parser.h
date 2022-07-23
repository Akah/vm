#ifndef PARSER_H
#define PARSER_H

#include "../common.h"

#define as_string(c) (char[]){c, '\0'}

#define peek(string) *string

#define consume(string) *(*string)++

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
} Token_t;

typedef struct {
    Token_t type;
    int row;
    int col;
    int length;
    const char* start;
} Token;

typedef struct Node{
    Token token;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct {
    size_t length;
    Node* head;
    Node* last;
} Token_List;

extern const char *token_t_strings[];

void initScanner(const char* source);

void token_list_init(Token_List* tokens);

void token_list_free(Token_List* tokens);

void token_init(Token* token);

void token_free(Token* token);

Token scan_token();

#endif