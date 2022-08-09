#ifndef vm_h
#define vm_h

#include "chunk.h"
#include "value.h"

#define STACK_CAP 1024

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_CAP];
    Value* stack_top;
} VM;

typedef enum {
    ERR_OK,
    ERR_COMPILE_ERROR,
    ERR_RUNTIME_ERROR,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_STACK_DIV_BY_ZERO,
} InterpretResult;

const char* InterpretResult_to_string(InterpretResult result);

void vm_init();
void vm_free();

void stack_push(Value value);
Value stack_pop();

InterpretResult interpret(const char* source);
// InterpretResult interpret(Chunk *chunk);

#endif