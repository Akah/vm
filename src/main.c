#include "common.h"
#include "vm/vm.h"
#include "compiler/compiler.h"

static void repl() {
    char line[1024];
    for (;;) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        interpret(line);
    }
}

static char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }
    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }
    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

static void run_file(const char* path) {
    char* source = read_file(path);
    InterpretResult result = interpret(source);
    free(source);
    if (result == ERR_COMPILE_ERROR) exit(65);
    if (result == ERR_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char* argv[]) {
    vm_init();

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        fprintf(stderr, "Usage: lisp [path]\n");
        exit(64);
    }

    // TODO: free vm
    return 0;
}