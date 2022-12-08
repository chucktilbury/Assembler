

#include "common.h"
#include "errors.h"
#include "memory.h"

#include "parser.h"
#include "scanner.h"
#include "symbol_tab.h"

extern FILE* outfile;

int main(int argc, char** argv) {
    outfile = stdout;
    _init_memory();
    if(argc < 2) {
        if(isatty(fileno(stdin))) {
            fprintf(stderr, "Nothing to input!\n");
            return 1;
        }
    }
    else {
        open_file(_copy_str(argv[1]));
    }


    yyparse();

    dumpSymTab();
    close_file();
    _uninit_memory();

    return 0;
}
