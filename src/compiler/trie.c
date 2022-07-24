#include "trie.h"

Trie_node* trie_new_node() {
    Trie_node *pnode = NULL;
    pnode = (Trie_node*)malloc(sizeof(Trie_node));
    if (pnode) {
        pnode->end = false;
        pnode->type = TOKEN_INV; // or token_inv???
        for (int i=0; i<ALPHABET_SIZE; i++) {
            pnode->children[i] = NULL;
        }
    }
    return pnode;
}

void trie_insert(Trie* trie, const char* key, Token_t type) {
    int level;
    int length = strlen(key);
    int index;
    Trie_node* pnode = trie;

    for (level=0; level<length; level++) {
        index = CHAR_TO_INDEX(key[level]);
        if (!pnode->children[index]) {
            pnode->children[index] = trie_new_node();
        }
        pnode = pnode->children[index];
    }
    pnode->end = true;
    pnode->type = type;
}

Token_t trie_get_token(Trie* trie, const char* key) {
    int level;
    int length = strlen(key);
    int index;
    Trie_node* pnode = trie;

    for (level = 0; level < length; level++) {
        index = CHAR_TO_INDEX(key[level]);
        if (!pnode->children[index]) {
            return TOKEN_SYM;
        }
        pnode = pnode->children[index];
    }

    if (pnode->end) {
        return pnode->type;
    }
    // shouldn't get here but just incase...
    return TOKEN_INV;
}