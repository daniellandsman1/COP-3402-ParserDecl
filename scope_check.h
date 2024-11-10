//Michael John
//Header file for scope_check.c

#ifndef SCOPE_CHECK_H
#define SCOPE_CHECK_H

#include "symtab.h"
#include "ast.h"

// Function to check the program's declarations
void scope_check_program(AST *program_ast);

// Recursive function to check declarations in the AST
void check_declarations(symtab *st, AST *node);

#endif
