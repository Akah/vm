#include "../common.h"
#include "compiler.h"
#include "parser.h"
#include <stdint.h>
#include <stdlib.h>

typedef void (*ParseRule)();

// not needed for lisp?
// assuming this is for infix operators???
typedef struct {
    Token current;
    Token previous;
    bool had_error;
    bool panic_mode;
} Parser;

Parser parser;
Chunk *compiling_chunk;

static Chunk* current_chunk() {
    return compiling_chunk;
}

static void error_at(Token* token, const char* message) {
    if (parser.panic_mode) return;
    parser.panic_mode = true;
    parser.had_error = true;
    fprintf(stderr, "[%d:%d]: Error: %s\n", token->col, token->row, message);
}

static void error_at_current(const char* message) {
    error_at(&parser.current, message);
}

static void error(const char *message) {
    error_at(&parser.previous, message);
}


static void advance() {
    parser.previous = parser.current;

    for (;;) {
        parser.current = scan_token();
        if (parser.current.type == TOKEN_INV) break;

        error_at_current(parser.current.start);
    }
}

static void consume(Token_t type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }
    error_at_current(message);
}

static void emit_byte(uint8_t byte) {
    write_chunk(current_chunk(), byte, parser.previous.row);
}

static void emit_bytes(uint8_t byte1, uint8_t byte2) {
    emit_byte(byte1);
    emit_byte(byte2);
}

static void emit_return() {
    emit_byte(OP_RETURN);
}

static uint8_t make_constant(Value value) {
    int constant = add_constant(current_chunk(), value);
    if (constant > UINT8_MAX) {
        error("Too many constants (byte) in one chunk");
        return 0;
    }
    return (uint8_t)constant;
}

static void emit_constant(Value value) {
    emit_bytes(OP_CONSTANT, make_constant(value));
}

static void end_compilation() {
    return emit_return();
}

static void binary() {
    Token_t op_type = parser.previous.type;
    // parse params...
    // parse operands and push to stack before pushing opcode
    parse();

    switch (op_type) {
    case TOKEN_ADD:
        emit_byte(OP_ADD);
        break;
    case TOKEN_MIN:
        emit_byte(OP_SUB);
        break;
    case TOKEN_MUL:
        emit_byte(OP_MUL);
        break;
    case TOKEN_DIV:
        emit_byte(OP_DIV);
        break;
    default:
        return;
    }
}

static void expression() {
    //
    parse();
}

// parens:
static void grouping() {
    expression();
    consume(TOKEN_CBR, "Expect ')' after expession.");
}

static void unary() {
    Token_t op_type = parser.previous.type;
    parse();

    switch(op_type) {
    case TOKEN_MIN:
        emit_byte(OP_NEGATE);
        break;
    default:
        return;// unreachable
    }
}

static void number() {
    double value = strtod(parser.previous.start, NULL);
    emit_constant(value);
}

bool compile(const char* source, Chunk* chunk) {
    init_scanner(source);
    compiling_chunk = chunk;

    parser.had_error = false;
    parser.panic_mode = false;

    advance();

    /* for (;;) { */
    /*     Token token = scan_token(); */
    /*     print_token(token); */
    /*     if (token.type == TOKEN_EOF) break; */
    /* } */
    end_compilation();
    return !parser.had_error;
}