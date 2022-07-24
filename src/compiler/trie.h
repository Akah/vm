#include "../common.h"
#include "parser.h"

#define ALPHABET_SIZE 26

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

typedef struct Trie_node {
    struct Trie_node *children[ALPHABET_SIZE];
    bool end;
    Token_t type;
} Trie;

typedef struct Trie_node Trie_node;

Trie_node* trie_new_node();

void trie_insert(Trie* trie, const char* key, Token_t type);

Token_t trie_get_token(Trie* trie, const char* key);