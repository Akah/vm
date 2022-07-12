#include "debug.h"
#include "chunk.h"
#include "value.h"

static int inst_simple(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int inst_constant(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    value_print(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

void disassemble_chunk(Chunk* chunk, const char* name) {
    printf("== %s == %d/%d(Bytes)\n", name, chunk->count, chunk->capacity);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassemble_instruction(chunk, offset);
    }
}

int disassemble_instruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);

    // if line is same as before just show |
    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }

    OP_code instruction = chunk->code[offset];
    switch (instruction) {
    case OP_RETURN:
        return inst_simple("RETURN", offset);
    case OP_CONSTANT:
        return inst_constant("CONSTANT", chunk, offset);
    case OP_NEGATE:
        return inst_simple("NEGATE", offset);
    case OP_ADD:
        return inst_simple("ADD", offset);
    case OP_SUB:
        return inst_simple("SUB", offset);
    case OP_DIV:
        return inst_simple("DIV", offset);
    case OP_MUL:
        return inst_simple("MUL", offset);
    case OP_JUMP:
        return inst_constant("JUMP", chunk, offset);
    case OP_JUMP_NOT_ZERO:
        return inst_constant("JUMP_NOT_ZERO", chunk, offset);
    case OP_EQUAL:
        return inst_simple("EQUAL", offset);
    case OP_GREATER:
        return inst_simple("OP_GREATER", offset);
    case OP_GREATER_EQ:
        return inst_simple("OP_GREATER_EQ", offset);
    case OP_LESS:
        return inst_simple("OP_LESS", offset);
    case OP_LESS_EQ:
        return inst_simple("OP_LESS_EQ", offset);
    default:
        printf("Unkown opcode %d\n", instruction);
        return offset + 1;
    }
}
