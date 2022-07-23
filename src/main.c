#include "common.h"
#include "vm/chunk.h"
#include "vm/debug.h"
#include "vm/vm.h"
#include "compiler/compiler.h"
#include "compiler/parser.h"

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
    char* source = "(+ 1 1)";

    compile(source);

    return 0;
}