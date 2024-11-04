#ifndef SCOPE_CHECK_H
#define SCOPE_CHECK_H

#include "symtab.h"
#include "ast.h"

// Forward declaration for the function to check program declarations
void scope_check_program(AST *program_ast);

// Recursive function to check declarations in the AST
void check_declarations(symtab *st, AST *node);

#endif