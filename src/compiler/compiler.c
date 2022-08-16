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
    bool  had_error;
    bool  panic_mode;
    int   params;
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

static void emit_byte_n_times(uint8_t byte, int amount) {
    for (int i=0; i<amount-1; i++) {
        emit_byte(byte);
    }
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


static ParseRule get_rule(Token_t type);

static int parse();

static void binary() {
    Token_t op_type = parser.current.type;
    int params = parse();

    switch (op_type) {
    case TOKEN_ADD:
        emit_byte_n_times(OP_ADD, params);
        break;
    case TOKEN_MIN:
        emit_byte_n_times(OP_SUB, params);
        break;
    case TOKEN_MUL:
        emit_byte_n_times(OP_MUL, params);
        break;
    case TOKEN_DIV:
        emit_byte_n_times(OP_DIV, params);
        break;
    case TOKEN_LT:
        emit_byte_n_times(OP_LESS, params);
        break;
    case TOKEN_LTE:
        emit_byte_n_times(OP_LESS_EQ, params);
        break;
    case TOKEN_GT:
        emit_byte_n_times(OP_GREATER, params);
        break;
    case TOKEN_GTE:
        emit_byte_n_times(OP_GREATER_EQ, params);
        break;
    default:
        puts("was something else");
        return;
    }
}

static void number() {
    double value = strtod(parser.current.start, NULL);
    emit_constant(value);
}

static int parse() {
    for (int counter = 0;;counter++) {
        parser.previous = parser.current;
        Token token = scan_token();
        parser.current = token;
        print_token(token);

        if (token.type == TOKEN_EOF)
            return counter;

        get_rule(token.type)();
    }
    return 0;
}

static void expression() {
    //
    parse();
}

static void not_implemented() {
    error_at_current("No parse rule implemented for token");
}

// parens:
static void grouping() {
    expression();
    consume(TOKEN_CBR, "Expect ')' after expession.");
}

ParseRule rules[] = {
    [TOKEN_OBR] = grouping,
    [TOKEN_CBR] = NULL,
    [TOKEN_ADD] = binary,
    [TOKEN_MIN] = binary,
    [TOKEN_DIV] = binary,
    [TOKEN_MUL] = binary,
    [TOKEN_MOD] = binary,
    [TOKEN_LT]  = binary,
    [TOKEN_GT]  = binary,
    [TOKEN_LTE] = binary,
    [TOKEN_GTE] = binary,
    [TOKEN_INT] = number,
    [TOKEN_INV] = NULL,
    [TOKEN_DEF] = NULL,
    [TOKEN_LET] = NULL,
    [TOKEN_IF]  = NULL,
    [TOKEN_QUO] = NULL,
    [TOKEN_STR] = NULL,
    [TOKEN_FLT] = number,
    [TOKEN_F]   = NULL,
    [TOKEN_T]   = NULL,
    [TOKEN_EOF] = NULL,
};

static ParseRule get_rule(Token_t type) {
    return rules[type];
}

bool compile(const char* source, Chunk* chunk) {
    init_scanner(source);
    compiling_chunk = chunk;

    parser.had_error = false;
    parser.panic_mode = false;

    parse();

    end_compilation();
    return !parser.had_error;
}