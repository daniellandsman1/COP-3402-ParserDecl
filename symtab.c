// Ryan Rohan
// symtab.c: Implementation of symbol table functions

#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "scope.h"       // Ensure scope.h is included for scope_destroy
#include "utilities.h"

// Initialize the symbol table
void symtab_initialize(symtab *st) {
    st->current_level = -1;  // Start with no scopes
}

// Enter a new scope
void symtab_enter_scope(symtab *st) {
    if (st->current_level >= MAX_SCOPE_SIZE - 1) {
        bail_with_error("Maximum scope depth exceeded.");
    }
    st->current_level++;
    st->scopes[st->current_level] = scope_initialize();
}

// Exit the current scope
void symtab_exit_scope(symtab *st) {
    if (st->current_level < 0) {
        bail_with_error("No scope to exit.");
    }
    scope_destroy(st->scopes[st->current_level]);
    st->scopes[st->current_level] = NULL;
    st->current_level--;
}

// Insert a new identifier into the current scope
void symtab_insert(symtab *st, const char *name, id_attrs *attrs) {
    if (st->current_level < 0) {
        bail_with_error("No active scope to insert into.");
    }
    scope_insert(st->scopes[st->current_level], name, attrs);
}

// Lookup an identifier in the symbol table
id_attrs *symtab_lookup(symtab *st, const char *name) {
    for (int i = st->current_level; i >= 0; i--) {
        id_attrs *attrs = scope_lookup(st->scopes[i], name);
        if (attrs != NULL) {
            return attrs;  // Found the identifier
        }
    }
    return NULL;  // Not found
}