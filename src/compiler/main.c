

#include "goldfish.h"
#include "parser.h"
#include "scanner.h"
#include "symtable.h"

extern FILE* outfile;

int main(int argc, char** argv)
{
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

  dumpSymtab();
  close_file();
  _uninit_memory();

  return 0;
}
