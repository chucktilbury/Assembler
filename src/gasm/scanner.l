
%{
//#undef //_DEBUGGING
//#undef //_TRACE
//#define //_TRACE(...)

#include "gas.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

void yyerror(const char* s);

static int bidx = 0;
static char buffer[1024*64];
static int line_no = 1;
static char* file_name;

// these values are set by the preprocessor rule in the parser
void set_line_no(int no)
{
    // not including the line the directive is on
    line_no = no-1;
}

int get_line_no()
{
    return line_no;
}

void set_file_name(const char* fname)
{
    //file_name = _copy_str(fname);
    file_name = (char*)fname;
}

const char* get_file_name()
{
    return file_name;
}

static void __append_char(char ch)
{
// TODO: bounds checking
    buffer[bidx] = ch;
    bidx++;
}

static void __append_str(const char* str)
{
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

    /* overhead */
\n              { line_no++; }
[ \t\r;]        {}

    /* keywords */
"UINT"          { yylval.type = UINT; return TOK_UINT; }
"INT"           { yylval.type = INT; return TOK_INT; }
"FLOAT"         { yylval.type = FLOAT; return TOK_FLOAT; }
"BOOL"          { yylval.type = BOOL; return TOK_BOOL; }
"STRING"        { yylval.type = STRING; return TOK_STRING; }

"TRUE"          { yylval.boolval = true; return TOK_TRUE; }
"FALSE"         { yylval.boolval = false; return TOK_FALSE; }

"EXIT"          { yylval.opcode = OP_EXIT; return TOK_EXIT; }
"NOP"           { yylval.opcode = OP_NOP; return TOK_NOP; }
"CALL"          { yylval.opcode = OP_CALL; return TOK_CALL; }
"TRAP"          { yylval.opcode = OP_TRAP; return TOK_TRAP; }
"RETURN"        { yylval.opcode = OP_RETURN; return TOK_RETURN; }
"JMP"           { yylval.opcode = OP_JMP; return TOK_JMP; }
"BR"            { yylval.opcode = OP_BR; return TOK_BR; }
"PUSH"          { yylval.opcode = OP_PUSH; return TOK_PUSH; }
"POP"           { yylval.opcode = OP_POP; return TOK_POP; }
"PEEK"          { yylval.opcode = OP_PEEK; return TOK_PEEK; }
"SIDX"          { yylval.opcode = OP_SIDX; return TOK_SIDX; }
"LOAD"          { yylval.opcode = OP_LOAD; return TOK_LOAD; }
"STORE"         { yylval.opcode = OP_STORE; return TOK_STORE; }
"NOT"           { yylval.opcode = OP_NOT; return TOK_NOT; }
"EQ"            { yylval.opcode = OP_EQ; return TOK_EQ; }
"NEQ"           { yylval.opcode = OP_NEQ; return TOK_NEQ; }
"LEQ"           { yylval.opcode = OP_LEQ; return TOK_LEQ; }
"GEQ"           { yylval.opcode = OP_GEQ; return TOK_GEQ; }
"LESS"          { yylval.opcode = OP_LESS; return TOK_LESS; }
"GTR"           { yylval.opcode = OP_GTR; return TOK_GTR; }
"NEG"           { yylval.opcode = OP_NEG; return TOK_NEG; }
"ADD"           { yylval.opcode = OP_ADD; return TOK_ADD; }
"SUB"           { yylval.opcode = OP_SUB; return TOK_SUB; }
"MUL"           { yylval.opcode = OP_MUL; return TOK_MUL; }
"DIV"           { yylval.opcode = OP_DIV; return TOK_DIV; }
"MOD"           { yylval.opcode = OP_MOD; return TOK_MOD; }

"R0"            { yylval.reg = REG_0; return TOK_R0; }
"R1"            { yylval.reg = REG_1; return TOK_R1; }
"R2"            { yylval.reg = REG_2; return TOK_R2; }
"R3"            { yylval.reg = REG_3; return TOK_R3; }
"R4"            { yylval.reg = REG_4; return TOK_R4; }
"R5"            { yylval.reg = REG_5; return TOK_R5; }
"R6"            { yylval.reg = REG_6; return TOK_R6; }
"R7"            { yylval.reg = REG_7; return TOK_R7; }
"R8"            { yylval.reg = REG_8; return TOK_R8; }
"R9"            { yylval.reg = REG_9; return TOK_R9; }
"R10"           { yylval.reg = REG_10; return TOK_R10; }
"R11"           { yylval.reg = REG_11; return TOK_R11; }
"R12"           { yylval.reg = REG_12; return TOK_R12; }
"R13"           { yylval.reg = REG_13; return TOK_R13; }
"R14"           { yylval.reg = REG_14; return TOK_R14; }
"R15"           { yylval.reg = REG_15; return TOK_R15; }

"CONST"         { return TOK_CONST; }

        /* arithmetic operators */
"+"             { return '+'; }
"-"             { return '-'; }
"="             { return '='; }
"/"             { return '/'; }
"*"             { return '*'; }
"("             { return '('; }
")"             { return ')'; }
"{"             { return '{'; }
"}"             { return '}'; }
","             { return ','; }
":"             { return ':'; }

"#line"         { return TOK_LINE; }

    /* constructed tokens */

    /* A simple symbol */
[a-zA-Z_][a-zA-Z_0-9]* {
        yylval.symbol = (char*)_copy_str(yytext);
        return TOK_SYMBOL;
    }

[0-9]+ {
        yylval.num = strtol(yytext, NULL, 10);
        return TOK_INUM;
    }

    /* recognize a float */
([0-9]*\.)?[0-9]+([Ee][-+]?[0-9]+)? {
        yylval.fnum = strtod(yytext, NULL);
        return TOK_FNUM;
    }

0[Xx][0-9a-fA-F]+ {
        yylval.unum = strtol(yytext, NULL, 16);
        return TOK_UNUM;
    }

    /* double quoted strings have escapes managed */
\"  {
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(DQUOTES);
    }

<DQUOTES>\" {
        // TODO: make this a proper USRTYPE
        yylval.qstr = _copy_str(buffer);
        BEGIN(INITIAL);
        return TOK_QSTR;
    }

    /* the short rule matches before the long one does */
<DQUOTES>\\n    { __append_char('\n'); }
<DQUOTES>\\r    { __append_char('\r'); }
<DQUOTES>\\e    { __append_char('\x1b'); }
<DQUOTES>\\t    { __append_char('\t'); }
<DQUOTES>\\b    { __append_char('\b'); }
<DQUOTES>\\f    { __append_char('\f'); }
<DQUOTES>\\v    { __append_char('\v'); }
<DQUOTES>\\\\   { __append_char('\\'); }
<DQUOTES>\\\"   { __append_char('\"'); }
<DQUOTES>\\\'   { __append_char('\''); }
<DQUOTES>\\\?   { __append_char('\?'); }
<DQUOTES>\\.    { __append_char(yytext[1]); }
<DQUOTES>\\[0-7]{1,3} { __append_char((char)strtol(yytext+1, 0, 8));  }
<DQUOTES>\\[xX][0-9a-fA-F]{1,3} { __append_char((char)strtol(yytext+2, 0, 16));  }
<DQUOTES>[^\\\"\n]*  { __append_str(yytext); }
<DQUOTES>\n     { line_no++; } /* track line numbers, but strip new line */


    /* single quoted strings are absolute literals */
\'  {
        bidx = 0;
        memset(buffer, 0, sizeof(buffer));
        BEGIN(SQUOTES);
    }

<SQUOTES>\' {
        // TODO: make this a proper USRTYPE
        yylval.qstr = _copy_str(buffer);
        BEGIN(INITIAL);
        return TOK_QSTR;
    }

<SQUOTES>[^\\'\n]*  { __append_str(yytext); }
<SQUOTES>\\.    { __append_str(yytext); }
<SQUOTES>\n     { __append_str(yytext); line_no++; } /* don't strip new lines */

.               { printf("Warning: unrecognized character: %c (0x%02X)", yytext[0], yytext[0]);}


%%

int open_file(const char *fname) {

    yyin = fopen(fname, "r");
    file_name = _copy_str(fname);
    if(yyin == NULL)
        fatalError("cannot open input file: %s: %s", fname, strerror(errno));

    return 0;
}
