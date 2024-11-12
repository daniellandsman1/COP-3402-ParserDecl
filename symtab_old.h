// Ryan Rohan
// symtab.h: Header file for Symbol Table Management

#ifndef SYMTAB_OLD_H
#define SYMTAB_OLD_H

#include "scope.h"
#include "id_attrs.h"
#include "file_location.h"

// The symbol table, represented as a stack of scopes.
typedef struct {
    scope *scopes[MAX_SCOPE_SIZE];
    int current_level;  // Tracks the current depth in the symbol table stack
} symtab;

// Initialize the symbol table, setting the current level to -1 (empty stack).
void symtab_initialize();

// Enter a new scope by pushing it onto the symbol table stack.
void symtab_enter_scope(symtab *st);

// Exit the current scope by popping it from the symbol table stack.
void symtab_exit_scope(symtab *st);

// Insert a new identifier into the current scope.
void symtab_insert(symtab *st, const char *name, id_attrs *attrs);

// Lookup an identifier in the symbol table, starting from the current scope
// and working outward to enclosing scopes. Returns NULL if not found.
id_attrs *symtab_lookup(symtab *st, const char *name);

#endif // SYMTAB_H
