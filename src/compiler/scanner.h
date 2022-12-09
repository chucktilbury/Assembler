#ifndef _COMPILER_SCANNER_H
#define _COMPILER_SCANNER_H

char* get_file_name();
int get_line_no();
int get_col_number();
int open_file(const char* fname);
void close_file();
const char* get_tok_str();
int get_token();

/*
 * Defined by flex. Call one time to isolate a symbol and then use the global
 * symbol struct to access the symbol.
 */
extern int yylex();
extern int yyparse();
extern FILE* yyin;

// void yyerror(char *s, ...);
void yyerror(const char* s);

#endif /* _COMPILER_SCANNER_H */
