#include "common.h"
#include "compiler/compiler.h"

int main(int argc, const char* argv[]) {
    char* source = "\'(if symbol 42 ? \"string\")";

    compile(source);

    return 0;
}