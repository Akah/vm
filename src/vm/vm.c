#include "../common.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "vm.h"
#include "../compiler/compiler.h"

#define DEBUG_TRACE_EXECUTION

VM vm;

static InterpretResult run() {
    disassemble_chunk(vm.chunk, "here");
#define CHECK_OVERFLOW()                                \
    ({                                                  \
        if ((vm.stack_top - vm.stack) >= STACK_CAP)     \
            return ERR_STACK_OVERFLOW;                  \
    })
#define CHECK_UNDERFLOW(size)                   \
    ({                                          \
        if ((vm.stack_top - vm.stack) < size)   \
            return ERR_STACK_UNDERFLOW;         \
    })
#define READ_BYTE() (*vm.ip++)
#define READ_OPERAND() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)                           \
    ({                                          \
        CHECK_UNDERFLOW(2);                     \
        double b = stack_pop();                 \
        double a = stack_pop();                 \
        CHECK_OVERFLOW();                       \
        stack_push(a op b);                     \
    })

    if (vm.chunk->count == 0) {
        printf("Error: chunk was empty\n");
        printf("Exiting VM\n");
        exit(1);
    }

    for (int i=0;i<50;i++) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value* slot = vm.stack; slot < vm.stack_top; slot++) {
            printf("[ ");
            value_print(*slot);
            printf(" ]");
        }
        printf("\n");
        disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        OP_code instruction;
        switch (instruction = READ_BYTE()) {

        case OP_RETURN:;
            CHECK_UNDERFLOW(1);
            value_print(stack_pop());
            printf("\n");
            return ERR_OK;

        case OP_CONSTANT:;
            Value constant = READ_OPERAND();
            CHECK_OVERFLOW();
            stack_push(constant);
            break;

        case OP_NEGATE:;
            CHECK_UNDERFLOW(1);
            CHECK_OVERFLOW();
            stack_push(-stack_pop());
            break;

        case OP_ADD:;
            BINARY_OP(+);
            break;

        case OP_SUB:;
            BINARY_OP(-);
            break;

        case OP_MUL:;
            BINARY_OP(*);
            break;

        case OP_DIV:;
            BINARY_OP(/);
            break;

        case OP_JUMP:;
            uint8_t jump = READ_OPERAND();
            vm.ip = vm.chunk->code + jump;
            break;

        case OP_JUMP_NOT_ZERO:;
            CHECK_UNDERFLOW(1);
            double a = stack_pop();
            uint8_t jumpnz = READ_OPERAND();
            if (a != 0) {
                vm.ip = vm.chunk->code + jumpnz;
            }
            break;

        case OP_EQUAL:;
            BINARY_OP(==);
            break;

        case OP_GREATER:;
            BINARY_OP(>);
            break;

        case OP_GREATER_EQ:;
            BINARY_OP(>=);
            break;

        case OP_LESS:;
            BINARY_OP(<);
            break;

        case OP_LESS_EQ:;
            BINARY_OP(<=);
            break;

        default:
            printf("Error: invalid instruction: %d\n", instruction);
            return ERR_COMPILE_ERROR;
        }
    }
    return ERR_COMPILE_ERROR;
#undef CHECK_OVERFLOW
#undef CHECK_UNDERFLOW
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

static void stack_reset() {
    vm.stack_top = vm.stack;
}

void stack_push(Value value) {
    *vm.stack_top = value;
    vm.stack_top++;
}

Value stack_pop() {
    vm.stack_top--;
    return *vm.stack_top;
}

void vm_init() {
    stack_reset();
}

void vm_free() {}

const char* InterpretResult_to_string(InterpretResult result) {
    switch(result) {
    case ERR_OK:
        return "ERR_OK";
    case ERR_RUNTIME_ERROR:
        return "ERR_RUNTIME_ERROR";
    case ERR_COMPILE_ERROR:
        return "ERR_COMPILE_ERROR";
    case ERR_STACK_OVERFLOW:
        return "ERR_STACK_OVERFLOW";
    case ERR_STACK_UNDERFLOW:
        return "ERR_STACK_UNDERFLOW";
    case ERR_STACK_DIV_BY_ZERO:
        return "ERR_STACK_DIV_BY_ZERO";
    default:
        return "ERROR: INVALID_INTERPRET_RESULT";
    }
}

InterpretResult interpret(const char* source) {
    /* to test lexer run: */
    /* compile(source); */
    /* return ERR_OK; */
    Chunk chunk;
    init_chunk(&chunk);

    if (!compile(source, &chunk)) {
        free_chunk(&chunk);
        return ERR_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    free_chunk(&chunk);
    return result;
}
