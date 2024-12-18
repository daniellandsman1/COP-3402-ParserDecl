/* $Id: spl_lexer_definitions_top.l,v 1.4 2024/10/06 19:16:25 leavens Exp leavens $ */
/* Lexical Analyzer for SPL */

%option header-file = "spl_lexer.h"
%option outfile = "spl_lexer.c"
%option yylineno
%option bison-bridge

%{
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include "ast.h"
#include "parser_types.h"
#include "utilities.h"
#include "lexer.h"

 /* Tokens generated by Bison */
#include "spl.tab.h"

 /* need declaration of fileno, part of the C standard library.
   (Putting an extern declaration here shuts off a gcc warning.) */
extern int fileno(FILE *stream);

/* The filename of the file being read */
static char *input_filename;

/* Have any errors been noted? */
static bool errors_noted;

/* The value of a token */
extern YYSTYPE yylval;

/* The FILE used by the generated lexer */
extern FILE *yyin;

// We are not using yyunput or input
#define YY_NO_UNPUT
#define YY_NO_INPUT

#undef yywrap   /* sometimes a macro by default */

// apparently strdup is not declared in <string.h>
extern char *strdup(const char *s);

// set the lexer's value for a token in yylval as an AST
static void tok2ast(int code) {
    AST t;
    t.token.file_loc = file_location_make(input_filename, yylineno);
    t.token.type_tag = token_ast;
    t.token.code = code;
    t.token.text = strdup(yytext);
    yylval = t;
}

// Creates an AST node for an identifier token
static void ident2ast(const char *name) {
    AST t;
    assert(input_filename != NULL);
    t.ident.file_loc = file_location_make(input_filename, yylineno);
    t.ident.type_tag = ident_ast;
    t.ident.name = strdup(name);
    yylval = t;
}

// Creates an AST node for a number token
static void number2ast(unsigned int val)
{
    AST t;
    t.number.file_loc = file_location_make(input_filename, yylineno);
    t.number.type_tag = number_ast;
    t.number.text = strdup(yytext);
    t.number.value = val;
    yylval = t;
}

%}

 /* you can add actual definitions below, before the %% */

IDENT           {LETTER}({LETTERORDIGIT}*)
LETTER          [a-zA-Z]
NUMBER          {DIGIT}({DIGIT}*)
DIGIT           [0-9]
LETTERORDIGIT   ({LETTER}|{DIGIT})
PLUS            \+
MINUS           -
MULT            \*
DIV             \/
PUNCTUATION     (\.|;|=|,|:=|(|))
RESERVEDWORD    (const|var|proc|call|begin|end|if|then|else|while|do|read|print|skip|divisible|by)
RELOPS          (==|!=|<|<=|>|>=)
IGNORED         ({BLANK}|{TAB}|{VT}|{FORMFEED}|{EOL}|{COMMENT})
BLANK           [ ]
TAB             \t
VT              \v
FORMFEED        \f
NEWLINE         \n
CR              \r
EOL             ({NEWLINE}|({CR}{NEWLINE}))
COMMENT         {PERCENTSIGN}({NONNL}*){NEWLINE}
PERCENTSIGN     %
NONNL           .

 /* the rules section starts after the %% below */

%%

 /* fill in the rules for your lexer below! */
 /* Whitespace and comments */

{IGNORED}     { ; } /* do nothing with ignored characters */

 /* Reserved words */
"const"       { tok2ast(constsym); return constsym; }
"var"         { tok2ast(varsym); return varsym; }
"proc"        { tok2ast(procsym); return procsym; }
"call"        { tok2ast(callsym); return callsym; }
"begin"       { tok2ast(beginsym); return beginsym; }
"end"         { tok2ast(endsym); return endsym; }
"if"          { tok2ast(ifsym); return ifsym; }
"then"        { tok2ast(thensym); return thensym; }
"else"        { tok2ast(elsesym); return elsesym; }
"while"       { tok2ast(whilesym); return whilesym; }
"do"          { tok2ast(dosym); return dosym; }
"read"        { tok2ast(readsym); return readsym; }
"print"       { tok2ast(printsym); return printsym; }
"divisible"   { tok2ast(divisiblesym); return divisiblesym; }
"by"          { tok2ast(bysym); return bysym; }

 /* Punctuation and operators */
\.            { tok2ast(periodsym); return periodsym; }
;             { tok2ast(semisym); return semisym; }
=             { tok2ast(eqsym); return eqsym; }
,             { tok2ast(commasym); return commasym; }
:=            { tok2ast(becomessym); return becomessym; }
\(             { tok2ast(lparensym); return lparensym; }
\)             { tok2ast(rparensym); return rparensym; }
{PLUS}        { tok2ast(plussym); return plussym; }
{MINUS}       { tok2ast(minussym); return minussym; }
{MULT}        { tok2ast(multsym); return multsym; }
{DIV}         { tok2ast(divsym); return divsym; }
==            { tok2ast(eqeqsym); return eqeqsym; }
!=            { tok2ast(neqsym); return neqsym; }
\<             { tok2ast(ltsym); return ltsym; }
"<="            { tok2ast(leqsym); return leqsym; }
\>             { tok2ast(gtsym); return gtsym; }
">="            { tok2ast(geqsym); return geqsym; }

 /* Numbers and identifiers */
 /* Check if numbers exceed maximum allowed int size */
{NUMBER}      { 
                unsigned int num;
                sscanf(yytext, "%u", &num);
                if (num > INT_MAX) 
                {
                    char errmsg[512];
                    sprintf(errmsg, "Number (%s) is too large!", yytext);
                    yyerror(input_filename, errmsg);
                }

                number2ast((int)num);
                return numbersym;
              }
{IDENT}       { ident2ast(yytext); return identsym; }

 /* Invalid character */
.             { 
                char errmsg[512];
                sprintf(errmsg, "invalid character: '%c' ('\\0%o')", *yytext, *yytext);
                yyerror(input_filename, errmsg);
              } 

%%

 /* This code goes in the user code section of the spl_lexer.l file,
   following the last %% above. */

// Requires: fname != NULL
// Requires: fname is the name of a readable file
// Initialize the lexer and start it reading
// from the given file name
void lexer_init(char *fname)
{
    errors_noted = false;
    yyin = fopen(fname, "r");
    if (yyin == NULL) {
	bail_with_error("Cannot open %s", fname);
    }
    input_filename = fname;
}

// Close the file yyin
// and return 0 to indicate that there are no more files
int yywrap() {
    if (yyin != NULL) {
	int rc = fclose(yyin);
	if (rc == EOF) {
	    bail_with_error("Cannot close %s!", input_filename);
	}
    }
    input_filename = NULL;
    return 1;  /* no more input */
}

// Return the name of the current input file
const char *lexer_filename() {
    return input_filename;
}

// Return the line number of the next token
unsigned int lexer_line() {
    return yylineno;
}

/* Report an error to the user on stderr */
void yyerror(const char *filename, const char *msg)
{
    fflush(stdout);
    fprintf(stderr, "%s:%d: %s\n", input_filename, lexer_line(), msg);
    errors_noted = true;
}

// On standard output:
// Print a message about the file name of the lexer's input
// and then print a heading for the lexer's output.
void lexer_print_output_header()
{
    printf("Tokens from file %s\n", lexer_filename());
    printf("%-6s %-4s  %s\n", "Number", "Line", "Text");
}

// Have any errors been noted by the lexer?
bool lexer_has_errors()
{
    return errors_noted;
}

// Print information about the token t to stdout
// followed by a newline
void lexer_print_token(enum yytokentype t, unsigned int tline,
		       const char *txt)
{
    printf("%-6d %-4d \"%s\"\n", t, tline, txt);
}


/* Read all the tokens from the input file
 * and print each token on standard output
 * using the format in lexer_print_token */
void lexer_output()
{
    lexer_print_output_header();
    AST dummy;
    yytoken_kind_t t;
    do {
	t = yylex(&dummy);
	if (t == YYEOF) {
	    break;
        }
        lexer_print_token(t, yylineno, yytext);
    } while (t != YYEOF);
}
