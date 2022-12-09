%{
#include "goldfish.h"
#include "parser.h"
#include <stdio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

void yyerror(const char* s);

int bidx = 0;
char buffer[1024 * 64];
const char* file_name;

static void __append_char(char ch) {
    // TODO: bounds checking
    buffer[bidx] = ch;
    bidx++;
}

static void __append_str(const char* str) {
    // TODO: bounds checking
    strcpy(&buffer[bidx], str);
    bidx += strlen(str);
}

%}

/* state definitions */
%x SQUOTES
%x DQUOTES
%option noinput nounput
%option yylineno
%option noyywrap

%%
\n      { /*yylineno++;*/ }
[ \t\r] {}

"break"     { return BREAK; }
"case"      { return CASE; }
"continue"  { return CONTINUE; }
"const"     { return CONST; }
"default"   { return DEFAULT; }
"import"    { return IMPORT; }
"do"        { return DO; }
"else"      { return ELSE; }
"for"       { return FOR; }
"if"        { return IF; }
"return"    { return RETURN; }
"switch"    { return SWITCH; }
"while"     { return WHILE; }
"namespace" { return NAMESPACE; }
"class"     { return CLASS; }
"struct"    { return STRUCT; }
"num"       { return NUMBER; }
"number"    { return NUMBER; }
"nothing"   { return NOTHING; }
"string"    { return STRTYPE; }
"bool"      { return BOOLTYPE; }
"true"      { return TRUE; }
"false"     { return FALSE; }
"public"    { return PUBLIC; }
"private"   { return PRIVATE; }
"protected" { return PROTECTED; }
"in"        { return IN; }
"yield"     { return YIELD; }
"exit"      { return EXIT; }
"try"       { return TRY; }
"except"    { return EXCEPT; }
"raise"     { return RAISE; }
"ctor"      { return CTOR; }
"dtor"      { return DTOR; }
"construct" { return CTOR; }
"destruct"  { return DTOR; }
"#line"     { return MARKER; }

    /*
     * TODO: list, dict
     */

"<=" { return LORE; }
">=" { return GORE; }
"==" { return EQU; }
"!=" { return NEQU; }
"||" { return OR; }
"&&" { return AND; }

"+" { return '+'; }
"-" { return '-'; }
"=" { return '='; }
"/" { return '/'; }
"*" { return '*'; }
"%" { return '%'; }
"(" { return '('; }
")" { return ')'; }
"#" { return '#'; }
"{" { return '{'; }
"}" { return '}'; }
"," { return ','; }
"." { return '.'; }
"<" { return '<'; }
">" { return '>'; }
":" { return ':'; }
"|" { return '|'; }
"[" { return '['; }
"]" { return ']'; }
"@" { return '@'; }
"$" { return '$'; }
"&" { return '&'; }
"~" { return '~'; }
"!" { return '!'; }

[a-zA-Z_][a-zA-Z_0-9]* {
        yylval.str = (char*)strdup(yytext);
        return SYMBOL;
    }

[0-9]+ {
        yylval.num = (double)strtol(yytext, NULL, 10);
        return LNUM;
    }

    /* recognize a float */
([0-9]*\.)?[0-9]+([Ee][-+]?[0-9]+)? {
        yylval.num = strtod(yytext, NULL);
        return LNUM;
    }

0[Xx][0-9a-fA-F]+ {
        yylval.num = (double)strtol(yytext, NULL, 16);
        return LNUM;
    }

    /* double quoted strings have escapes managed */
\"  {
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(DQUOTES);
    }

<DQUOTES>\" {
        yylval.str = (char*)strdup(buffer);
        BEGIN(INITIAL);
        return STRG;
    }

    /* the short rule matches before the long one does */
<DQUOTES>\\n { __append_char('\n'); }
<DQUOTES>\\r { __append_char('\r'); }
<DQUOTES>\\e { __append_char('\x1b'); }
<DQUOTES>\\t { __append_char('\t'); }
<DQUOTES>\\b { __append_char('\b'); }
<DQUOTES>\\f { __append_char('\f'); }
<DQUOTES>\\v { __append_char('\v'); }
<DQUOTES>\\\\ { __append_char('\\'); }
<DQUOTES>\\\" { __append_char('\"'); }
<DQUOTES>\\\' { __append_char('\''); }
<DQUOTES>\\\? { __append_char('\?'); }
<DQUOTES>\\.  { __append_char(yytext[1]); }
<DQUOTES>\\[0-7]{1,3} { __append_char((char)strtol(yytext + 1, 0, 8)); }
<DQUOTES>\\[xX][0-9a-fA-F]{1,3} { __append_char((char)strtol(yytext + 2, 0, 16)); }
<DQUOTES>[^\\\"\n]*  { __append_str(yytext); }
<DQUOTES>\n     { yylineno++; } /* track line numbers, but strip new line */

    /* single quoted strings are absolute literals */
\'  {
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(SQUOTES);
    }

<SQUOTES>\' {
        yylval.str = (char*)strdup(buffer);
        BEGIN(INITIAL);
        return STRG;
    }

<SQUOTES>[^\\'\n]*  { __append_str(yytext); }
<SQUOTES>\\.    { __append_str(yytext); }
<SQUOTES>\n     {
        __append_str(yytext);
        yylineno++;
    } /* don't strip new lines */

.   {
        printf("Warning: unrecognized character: %c (0x%02X)\n", yytext[0], yytext[0]);
    }

%%

const char* _strdup(const char* str) {

    size_t len = strlen(str) + 1;
    char* buf = malloc(len);
    memcpy(buf, str, len);
    return buf;
}

int open_file(const char *fname) {

    // file_name = _copy_str(fname);
    yyin = fopen(fname, "r");
    if(yyin == NULL) {
        fprintf(stderr, "fatal error: cannot open input file: %s: %s\n", fname,
                strerror(errno));
        exit(1);
    }
    return 0;
}

void close_file() {

    fclose(yyin);
}

int get_line_no() {

    return yylineno;
}

const char* get_file_name() {

    return file_name;
}
