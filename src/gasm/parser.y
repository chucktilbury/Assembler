/*
 * This is the complete grammar for the NOP programming language.
 */

%debug
%defines
%locations

%{

#include "gas.h"
#include <stdarg.h>

// defined by flex
extern int yylex(void);
extern int yyparse(void);
extern FILE *yyin;
void yyerror(const char* s)
{
    syntaxError("%s", s);
}

void syntaxError(const char*, ...);

extern Module* module;

%}

%define parse.error verbose
%locations

%union {
    uint64_t unum;
    int64_t num;
    double fnum;
    bool boolval;
    int opcode;
    int type;
    int reg;
    char* symbol;
    char* qstr;
    void* usr;

    Value* val;
    Label* lab;
    DataDef* ddef;
};

%token <symbol> TOK_SYMBOL
%token <unum> TOK_UNUM
%token <num> TOK_INUM
%token <fnum> TOK_FNUM
%token <boolval> TOK_TRUE TOK_FALSE
    /*
    %type <literal> expression expression_factor expr_parameter
    %type <literal> type_name type_specifier bool_value
    %type <symbol> data_declaration data_definition
    */

    // %token <usr> TOK_USRTYPE
%token <qstr> TOK_QSTR

%token <type> TOK_INT TOK_FLOAT TOK_UINT TOK_STRING
%token <type>  TOK_BOOL

%token <opcode> TOK_EXIT TOK_NOP TOK_CALL TOK_TRAP TOK_LINE TOK_BREAK
%token <opcode> TOK_RETURN TOK_JMP TOK_BR TOK_PEEK TOK_SIDX
%token <opcode> TOK_PUSH TOK_POP TOK_LOAD TOK_DIV TOK_MOD
%token <opcode> TOK_STORE TOK_NOT TOK_EQ TOK_NEQ TOK_LTE
%token <opcode> TOK_GTE TOK_LT TOK_GT TOK_NEG TOK_ADD TOK_SUB TOK_MUL

%token <reg> TOK_R0 TOK_R1 TOK_R2 TOK_R3 TOK_R4 TOK_R5 TOK_R6 TOK_R7
%token <reg> TOK_R8 TOK_R9 TOK_R10 TOK_R11 TOK_R12 TOK_R13 TOK_R14 TOK_R15

%type <reg> register
%type <lab> label
%type <val> expr_parameter expression type_name  type_specifier
%type <val> expression_factor bool_value str_value
%type <ddef> data_declaration data_definition

%token TOK_CONST

%right '='
%left '+' '-'
%left '*' '/'
%left NEGATE

%%
program
    :module_item_list
    ;

module_item_list
    : module_item
    | module_item_list module_item
    ;

module_item
    : instruction
    | data_definition
    | data_declaration
    | pp_marker
    | label
    ;

pp_marker
    : TOK_LINE TOK_INUM TOK_QSTR {
        set_line_no($2);
        set_file_name($3);
    }
    ;

label
    : TOK_SYMBOL ':' { addLabel(module, $1); }
    ;

instruction
    : class0_instr
    | class1_instr
    | class2_instr
    | class3_instr
    | class4_instr
    | class5_instr
    | class6_instr
    | class7_instr
    | class8_instr
    | class9_instr
    | class10_instr
    | class11_instr
    | class12_instr
    ;

register
    : TOK_R0 { $$ = $1; }
    | TOK_R1 { $$ = $1; }
    | TOK_R2 { $$ = $1; }
    | TOK_R3 { $$ = $1; }
    | TOK_R4 { $$ = $1; }
    | TOK_R5 { $$ = $1; }
    | TOK_R6 { $$ = $1; }
    | TOK_R7 { $$ = $1; }
    | TOK_R8 { $$ = $1; }
    | TOK_R9 { $$ = $1; }
    | TOK_R10 { $$ = $1; }
    | TOK_R11 { $$ = $1; }
    | TOK_R12 { $$ = $1; }
    | TOK_R13 { $$ = $1; }
    | TOK_R14 { $$ = $1; }
    | TOK_R15 { $$ = $1; }
    ;

class0_instr
    : TOK_EXIT { addClass0(module, OP_EXIT); }
    | TOK_RETURN { addClass0(module, OP_RETURN); }
    | TOK_NOP { addClass0(module, OP_NOP); }
    | TOK_BREAK { addClass0(module, OP_BREAK); }
    ;

    /* one register parm */
class1_instr
    : TOK_NOT register { addClass1(module, OP_NOT, $2); }
    | TOK_POP register { addClass1(module, OP_POP, $2); }
    | TOK_PUSH register { addClass1(module, OP_PUSH, $2); }
    | TOK_SIDX register { addClass1(module, OP_SIDX, $2); }
    ;

    /* binary compare into zero flag */
class2_instr
    : TOK_NEG register ',' register { addClass2(module, OP_NEG, $2, $4); }
    | TOK_EQ register ',' register { addClass2(module, OP_EQ, $2, $4); }
    | TOK_NEQ register ',' register { addClass2(module, OP_NEQ, $2, $4); }
    | TOK_LTE register ',' register { addClass2(module, OP_LTE, $2, $4); }
    | TOK_GTE register ',' register { addClass2(module, OP_GTE, $2, $4); }
    | TOK_LT register ',' register { addClass2(module, OP_LT, $2, $4); }
    | TOK_GT register ',' register { addClass2(module, OP_GT, $2, $4); }
    | TOK_LOAD register ',' register { addClass2(module, OP_LOADR, $2, $4); }
    ;

    /* binary arithmetic into register */
class3_instr
    : TOK_ADD register ',' register ',' register { addClass3(module, OP_ADD, $2, $4, $6); }
    | TOK_SUB register ',' register ',' register { addClass3(module, OP_SUB, $2, $4, $6); }
    | TOK_MUL register ',' register ',' register { addClass3(module, OP_MUL, $2, $4, $6); }
    | TOK_DIV register ',' register ',' register { addClass3(module, OP_DIV, $2, $4, $6); }
    | TOK_MOD register ',' register ',' register { addClass3(module, OP_MOD, $2, $4, $6); }
    ;

    /* If there is no type coersion, then just take the largest one. */
expr_parameter
    : expression { $$ = $1; }
    | '(' type_name ')' expression { $$ = castVal($2->type, $4); }
    ;

    /* Numeric literal as parameter */
class4_instr
    : TOK_TRAP TOK_UNUM { addClass4(module, OP_TRAP, $2); }
    | TOK_TRAP TOK_INUM { addClass4(module, OP_TRAP, $2); }
    ;

    /* moving data in and out of registers */
class5_instr
    : TOK_LOAD register ',' TOK_SYMBOL { addClass5(module, OP_LOAD, $2, $4); }
    ;

class6_instr
    : TOK_LOAD register ',' expr_parameter { addClass6(module, OP_LOADI, $2, $4); }
    | TOK_LOAD register ',' TOK_QSTR { syntaxError("immediate strings are not allowed");
        // Value* val = createValue(STRING);
        // //val->data.str = addStr($4);
        // val->data.str = $4;
        // val->isAssigned = true;
        // addClass6(module, OP_LOADI, $2, val);
    }
    ;

class7_instr
    : TOK_STORE TOK_SYMBOL ',' register { addClass7(module, OP_STORE, $2, $4); }
    ;

class8_instr
    : TOK_CALL TOK_SYMBOL { addClass8(module, OP_CALL, $2); }
    | TOK_JMP TOK_SYMBOL { addClass8(module, OP_JMP, $2); }
    | TOK_BR TOK_SYMBOL { addClass8(module, OP_BR, $2); }
    ;

class9_instr
    : TOK_PEEK register ',' register ',' TOK_INUM { addClass9(module, OP_PEEK, $2, $4, (uint16_t)$6); }
    | TOK_PEEK register ',' register ',' TOK_UNUM { addClass9(module, OP_PEEK, $2, $4, (uint16_t)$6); }
    ;

class10_instr
    : TOK_PUSH TOK_SYMBOL { addClass10(module, OP_PUSHV, $2); }
    ;

class11_instr
    : TOK_PUSH expr_parameter { addClass11(module, OP_PUSHI, $2); }
    ;

class12_instr
    : TOK_STORE TOK_SYMBOL ',' expr_parameter { addClass12(module, OP_STOREI, $2, $4); }
    ;


type_name
    : TOK_INT { $$ = createValue(INT); }
    | TOK_UINT { $$ = createValue(UINT); }
    | TOK_FLOAT { $$ = createValue(FLOAT); }
    | TOK_BOOL { $$ = createValue(BOOL); }
    | TOK_STRING { $$ = createValue(STRING); }
    ;

type_specifier
    : TOK_CONST type_name { $$ = $2; $$->isConst = true; }
    | type_name { $$->isConst = false; }
    ;

data_declaration
    : type_specifier TOK_SYMBOL { $$ = addDataDef(module, $1, $2); }
    ;

data_definition
    : data_declaration '=' expression { assignDataDecl($1, $3); }
    | data_declaration '=' bool_value { assignDataDecl($1, $3); }
    | data_declaration '=' str_value { assignDataDecl($1, $3); }
    ;

str_value
    : TOK_QSTR {
        $$ = createValue(STRING);
        $$->isAssigned = true;
        //$$->data.str = addStr($1);
        $$->data.str = $1;
    }
    ;

bool_value
    : TOK_TRUE {
        $$ = createValue(BOOL);
        $$->isAssigned = true;
        $$->data.bval = true;
    }
    | TOK_FALSE {
        $$ = createValue(BOOL);
        $$->isAssigned = true;
        $$->data.bval = false;
    }
    ;

expression_factor
    : TOK_UNUM {
        $$ = createValue(UINT);
        $$->isAssigned = true;
        $$->data.unum = $1;
    }
    | TOK_INUM {
        $$ = createValue(INT);
        $$->isAssigned = true;
        $$->data.num = $1;
    }
    | TOK_FNUM {
        $$ = createValue(FLOAT);
        $$->isAssigned = true;
        $$->data.fnum = $1;
    }
    ;

expression
    : expression_factor { $$ = $1; }
    | expression '+' expression { addNum($$, $1, $3); }
    | expression '-' expression { subNum($$, $1, $3); }
    | expression '*' expression { mulNum($$, $1, $3); }
    | expression '/' expression { divNum($$, $1, $3); }
    | '-' expression %prec NEGATE { negNum($$, $2); }
    | '(' expression ')' { $$ = $2; }
    ;

%%
