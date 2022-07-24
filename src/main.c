#include "common.h"
#include "vm/chunk.h"
#include "vm/debug.h"
#include "vm/vm.h"
#include "compiler/compiler.h"
#include "compiler/parser.h"
#include "compiler/trie.h"

int main(int argc, const char* argv[]) {
    char* source = "\'(if symbol 42 \"string\")";

    compile(source);

    return 0;
}