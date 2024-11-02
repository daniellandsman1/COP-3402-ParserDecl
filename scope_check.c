// Ryan Rohan
// scope_check.c: Implements the main declaration checker

#include <stdlib.h>
#include "scope.h"
#include "symtab.h"
#include "utilities.h"
#include "ast.h"

// Forward declaration for recursive checking function
void check_declarations(symtab *st, AST *node);

// Main function to check program declarations
void scope_check_program(symtab *st, AST *program_ast) {
    symtab_initialize(st);        // Initialize the symbol table
    symtab_enter_scope(st);       // Enter global scope
    check_declarations(st, program_ast); // Perform declaration checks
    symtab_exit_scope(st);        // Exit global scope
}

// Recursively checks declarations in the AST
void check_declarations(symtab *st, AST *node) {
    if (node == NULL) return;

    switch (node->generic.type_tag) {
        case var_decl_ast:
            if (symtab_lookup(st, node->var_decl.ident_list.start->name) != NULL) {
                formatted_yyerror(node->var_decl.ident_list.start->file_loc->filename, 
                    "Duplicate declaration of variable '%s' at line %d.", 
                    node->var_decl.ident_list.start->name, ast_line(*node));
            } else {
                id_attrs *attrs = create_id_attrs(*node->var_decl.ident_list.start->file_loc, variable_idk, st->scopes[st->current_level]->size);
                symtab_insert(st, node->var_decl.ident_list.start->name, attrs);
            }
            break;

        case proc_decl_ast:
            if (symtab_lookup(st, node->proc_decl.name) != NULL) {
                formatted_yyerror(node->proc_decl.file_loc->filename, 
                    "Duplicate declaration of procedure '%s' at line %d.", 
                    node->proc_decl.name, ast_line(*node));
            } else {
                id_attrs *attrs = create_id_attrs(*node->proc_decl.file_loc, procedure_idk, st->scopes[st->current_level]->size);
                symtab_insert(st, node->proc_decl.name, attrs);
            }
            symtab_enter_scope(st);    // New scope for the procedure's block
            check_declarations(st, (AST *)&node->proc_decl.block->stmts); // Check statements in the new scope
            symtab_exit_scope(st);
            break;

        case ident_ast:
            if (symtab_lookup(st, node->ident.name) == NULL) {
                formatted_yyerror(node->ident.file_loc->filename, 
                    "Undeclared identifier '%s' used at line %d.", 
                    node->ident.name, ast_line(*node));
            }
            break;

        default:
            // Handle child nodes according to AST type and structure
            break;
    }
}