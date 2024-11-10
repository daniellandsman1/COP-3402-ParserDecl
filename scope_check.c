// Ryan Rohan, Michael John
// scope_check.c: Implements the main declaration checker

#include <stdlib.h>
#include "scope.h"
#include "symtab.h"
#include "utilities.h"
#include "ast.h"
#include "scope_check.h"  // Ensure this is included to define check_declarations

// Create a static symbol table instance for use within this file
static symtab st_instance;

void scope_check_program(AST *program_ast) {
    symtab_initialize();             // Call without parameters
    symtab_enter_scope(&st_instance);  // Enter global scope
    check_declarations(&st_instance, program_ast);  // Perform declaration checks
    symtab_exit_scope(&st_instance);  // Exit global scope
}

// Forward declaration for the recursive checking function
void check_declarations(symtab *st, AST *node);

void check_declarations(symtab *st, AST *node) {
    if (node == NULL) return;

    switch (node->generic.type_tag) {
        case var_decl_ast:
            // Check for duplicate variable declaration
            for (int i = 0; i < node->var_decl.ident_list.size; i++) {
                const char *identifier = node->var_decl.ident_list.start[i].name;
                if (symtab_lookup(st, identifier) != NULL) {
                    bail_with_prog_error(*node->var_decl.ident_list.start[i].file_loc,
                        "Duplicate declaration of variable '%s'.", 
                        identifier);
                } else {
                    id_attrs *attrs = create_id_attrs(*node->var_decl.ident_list.start[i].file_loc, variable_idk, st->scopes[st->current_level]->size);
                    symtab_insert(st, identifier, attrs);
                }
            }
            break;

        case proc_decl_ast:
            // Check for duplicate procedure declaration
            if (symtab_lookup(st, node->proc_decl.name) != NULL) {
                bail_with_prog_error(*node->proc_decl.file_loc,
                    "Duplicate declaration of procedure '%s'.", 
                    node->proc_decl.name);
            } else {
                id_attrs *attrs = create_id_attrs(*node->proc_decl.file_loc, procedure_idk, st->scopes[st->current_level]->size);
                symtab_insert(st, node->proc_decl.name, attrs);
            }
            symtab_enter_scope(st);  // New scope for the procedure's block
            check_declarations(st, (AST *)&node->proc_decl.block->stmts); // Check statements in the new scope
            symtab_exit_scope(st);
            break;

        case ident_ast:
            // Check for undeclared identifiers
            if (symtab_lookup(st, node->ident.name) == NULL) {
                bail_with_prog_error(*node->ident.file_loc,
                    "Undeclared identifier '%s' used.", 
                    node->ident.name);
            }
            break;

        case block_ast:
            // Check for syntax errors within the block
            for (int i = 0; i < node->block.stmts.size; i++) {
                if (node->block.stmts.start[i].type_tag == unexpected_token_ast) {
                    // Placeholder for expected tokens
                    const char *expected_tokens = "unknown token";
                    bail_with_prog_error(*node->block.stmts.start[i].file_loc,
                        "Syntax error: unexpected token. Expected tokens: %s", expected_tokens);
                } else {
                    check_declarations(st, &node->block.stmts.start[i]); // Recursively check statements
                }
            }
            break;

        default:
            // Handle other cases or ignore
            break;
    }
}
