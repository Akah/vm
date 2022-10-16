/* Known bugs:
 * - if stack is empty then program just quits even when operations are
 *   still to be executed. Should cause underflow error
 *
 * - CURRENT:
 *   'counter' is overridden during sub evaluations e.g: (+ (+ 1 1) 1).
 *   should be 2 params but only has one because of end;
 */

#include "../common.h"
#include "compiler.h"
#include "parser.h"

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
        puts("advance error:");
        error_at_current(parser.current.start);
        exit(1);
    }
}

static void consume(Token_t type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }
    puts("consume error:");
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
static void binary() {
    Token_t op_type = parser.current.type;
    int params = parse();
    printf("had %d params\n", params);
    /* printf("    previous -> "); */
    /* print_token(parser.previous); */
    /* printf("    next     -> "); */
    /* print_token(parser.previous); */

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
#pragma GCC diagnostic pop

static void number() {
    double value = strtod(parser.current.start, NULL);
    emit_constant(value);
}

int parse() {
    int counter = 0;
    for (;;++counter) {
        parser.previous = parser.current;
        Token token = scan_token();
        parser.current = token;
        print_token(token);

        if (token.type == TOKEN_EOF)
            return 0; // counter;

        if (token.type == TOKEN_CBR)
            return counter;

        ParseRule rule = get_rule(token.type);
        // call rule

        if (rule) {
            /* puts("had rule"); */
            rule();
        } else {
            /* puts("null rule"); */
        }
    }
    return counter;
}

static void expression() {
    //
    puts("expression");
    parse();
}

// parens:
static void grouping() {
    // assert first value is operator (or function)
    /* puts("grouping"); */
    /* // consume(TOKEN_OBR, "Must start with bracket"); */
    /* printf("is op? ->"); */
    /* print_token(parser.current); */
    puts("grouping");
    parse();
}

// used for non-implemented functions
// sometimes not eneeded hence the pragmas
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void not_implemented() {
    error_at_current("No parse rule implemented for token");
}
#pragma GCC diagnostic pop

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