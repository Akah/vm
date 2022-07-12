#ifndef chunk_h
#define chunk_h

#include "../common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,      //  0 push operand constant to stack
    OP_NEGATE,        //  1 swap number's sign
    OP_ADD,           //  2 add last two values on stack
    OP_SUB,           //  3 substract last two values on stack
    OP_MUL,           //  4 multiply last two values on stack
    OP_DIV,           //  5 divide last two values on stack
    OP_RETURN,        //  6 print last stack value and exit
    OP_JUMP,          //  7 jump to operand position in code
    OP_JUMP_NOT_ZERO, //  8 jump to operand position in code if last stack value is not 0
    OP_EQUAL,         //  9 compare last two stack values; if equal push 1 else push 0
    OP_GREATER,       // 10 stack - 1 >  stack - 2 ? 0 : 1
    OP_GREATER_EQ,    // 11 stack - 1 >= stack - 2 ? 0 : 1
    OP_LESS,          // 12 stack - 1 <  stack - 2 ? 1 : 0
    OP_LESS_EQ,       // 13 stack - 1 <= stack - 2 ? 1 : 0
 // OP_LOCAL_LOAD,    // 14
 // OP_LOCAL_STORE    // 15
 // OP_GLOBAL_LOAD,   // 16
 // OP_GLOBAL_STORE,  // 17
} OP_code;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    int* lines;
    Value_Array constants;
} Chunk;

void init_chunk(Chunk* chunk);

void write_chunk(Chunk* chunk, uint8_t byte, int line);

void free_chunk(Chunk *chunk);

int add_constant(Chunk* chunk, Value value);

#endif