// Daniel Landsman
// symtab.h: symbol table file, includes function declarations

#ifndef _SYMTAB_H
#define _SYMTAB_H

#include "scope.h"
#include "id_use.h"

#define MAX_NEST_LVL 100

// Pre-Conditions: Symbol table is properly declared with proper max size
// Post-Conditions: Initializes symbol table to be completely empty
extern void symtab_initialize();

// Pre-Conditions: Symbol table is properly declared with proper max size
// Post-Conditions: Returns size of the symbol table as an unsigned int
extern unsigned int symtab_size();

// Pre-Conditions: Symbol table is properly declared with proper max size
// Post-Conditions: Returns true if symbol table is empty, false otherwise
extern bool symtab_empty();

// Pre-Conditions: Symbol table is properly declared with proper max size,
// scope at top of symbol table isn't NULL
// Post-Conditions: Returns the number of associations in the scope at the
// top of the symbol table
extern unsigned int symtab_scope_loc_count();

// Pre-Conditions: Symbol table is properly declared with proper max size,
// scope at top of symbol table isn't NULL
// Post-Conditions: Returns the current size of the scope at the top
// of the symbol table
extern unsigned int symtab_scope_size();

// Pre-Conditions: Symbol table is properly declared with proper max size,
// scope at top of symbol table isn't NULL
// Post-Conditions: Returns true if the scope at the top of the symbol
// table is full, returns false otherwise
extern bool symtab_scope_full();

// Pre-Conditions: Symbol table is properly declared with proper max size and
// is in an active scope (size is positive)
// Post-Conditions: Returns the current nesting level of the symbol table as
// an unsigned int
extern unsigned int symtab_current_nest_lvl();

// Pre-Conditions: Symbol table is properly declared with proper max size
// Post-Conditions: Returns true if the symbol table is full, false otherwise
extern bool symtab_full();

// Pre-Conditions: Symbol table is properly declared with proper max size and
// my_name is not NULL
// Post-Conditions: Searches for an association involving my_name in the
// entire symbol table, returns the corresponding id_use structure if found,
// returns NULL if not found
extern id_use* symtab_lookup(const char* my_name);

// Pre-Conditions: Symbol table is properly declared with proper max size and
// my_name is not NULL
// Post-Conditions: Returns true if an association involving my_name is found
// anywhere in the symbol table, returns false otherwise. Effectively a boolean
// version of the symtab_lookup() function.
extern bool symtab_name_declared(const char* my_name);

// Pre-Conditions: Symbol table is properly declared with proper max size,
// current scope at top of symbol table is not NULL, and my_name is not NULL
// Post-Conditions: Returns true if my_name has an association in the
// current scope at the top of the symbol table, returns false otherwise
extern bool symtab_name_declared_currently(const char* my_name);

// Pre-Conditions: Symbol table is properly declared with proper max size and
// is in an active scope (size is positive), my_name and my_attrs are not NULL, 
//current scope at top of symbol table is not NULL
// Post-Conditions: Inserts an association of my_name to my_attrs into the current
// scope at the top of the symbol table, produces error message if association
// for my_name already exists in current scope
extern void symtab_insert(const char* my_name, id_attrs* my_attrs);

// Pre-Conditions: Symbol table is properly declared with proper max size
// Post-Conditions: Enter a new scope for the symbol table
extern void symtab_enter_scope();

// Pre-Conditions: Symbol table is properly declared with proper max size and
// is in an active scope (not empty)
// Post-Conditions: Leaves the current scope that the symbol table is in,
// produces an error message if there are no more scopes to leave
extern void symtab_exit_scope();

#endif
