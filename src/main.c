#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include <stdio.h>

/* static char* read_file(const char* path) { */
/*     FILE* file = fopen(path, "rb"); */
/*     if (file == NULL) { */
/*         fprintf(stderr, "Could not open file \"%s\".\n", path); */
/*         exit(74); */
/*     } */

/*     fseek(file, 0L, SEEK_END); */
/*     size_t file_size = ftell(file); */
/*     rewind(file); */

/*     char* buffer = (char*)malloc(file_size +1); */
/*     if (buffer == NULL) { */
/*         fprintf(stderr, "Not enough memory to read \"%s\".\n", path); */
/*         exit(74); */
/*     } */
/*     size_t bytes_read = fread(buffer, sizeof(char), file_size, file); */
/*     if (bytes_read < file_size) { */
/*         fprintf(stderr, "Could not read file \"%s\".\n", path); */
/*         exit(74); */
/*     } */
/*     buffer[bytes_read] = '\0'; */

/*     fclose(file); */
/*     return buffer; */
/* } */

/* static void repl() { */
/*     char line[1024]; */
/*     printf("lisp version 0.0.1\n\n"); */
/*     for(;;) { */
/*         printf("λ>"); */
/*         if (!fgets(line, sizeof(line), stdin)) { */
/*             printf("\n"); */
/*             break; */
/*         } */

/*         interpret(line); */
/*     } */
/* } */

/* static void run_file(const char* string) { */
/*     /\* char* source = read_file(path); *\/ */
/*     /\* InterpretResult result = interpret(source); *\/ */
/*     /\* free(source); *\/ */
/*     /\* if (result == INTERPRET_COMPILE_ERROR) exit(65); *\/ */
/*     /\* if (result == INTERPRET_RUNTIME_ERROR) exit(70); *\/ */
/*     return; */
/* } */

int main(int argc, const char* argv[]) {
    vm_init();
    Chunk chunk;
    init_chunk(&chunk);

    // true   ->  420
    // false  ->   69

    int constant;

    constant = add_constant(&chunk, 1);
    write_chunk(&chunk, OP_CONSTANT, 123);
    write_chunk(&chunk, constant, 123);

    constant = add_constant(&chunk, 2);
    write_chunk(&chunk, OP_CONSTANT, 123);
    write_chunk(&chunk, constant, 123);

    write_chunk(&chunk, OP_LESS, 123);

    constant = add_constant(&chunk, 11);
    write_chunk(&chunk, OP_JUMP_NOT_ZERO, 123);
    write_chunk(&chunk, constant, 123);


    constant = add_constant(&chunk, 69);
    write_chunk(&chunk, OP_CONSTANT, 123);
    write_chunk(&chunk, constant, 123);

    constant = add_constant(&chunk, 13);
    write_chunk(&chunk, OP_JUMP, 123);
    write_chunk(&chunk, constant, 123);


    constant = add_constant(&chunk, 420);
    write_chunk(&chunk, OP_CONSTANT, 123);
    write_chunk(&chunk, constant, 123);

    constant = add_constant(&chunk, 1);
    write_chunk(&chunk, OP_CONSTANT, 123);
    write_chunk(&chunk, constant, 123);

    write_chunk(&chunk, OP_ADD, 123);

    write_chunk(&chunk, OP_RETURN, 123);

    disassemble_chunk(&chunk, "test");

    InterpretResult result = interpret(&chunk);

    printf("%s\n", InterpretResult_to_string(result));

    free_chunk(&chunk);
    vm_free();
    return result >= 1; // returns 1 if not ERR_OK
}