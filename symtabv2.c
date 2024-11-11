// Daniel Landsman
// symtabv2.c: symbol table file, includes function bodies

#include "utilities.h"

static int symtab_top = -1; // Index in symtab array that represents top of stack and current nesting level
static scope* symtab[MAX_NEST_LVL]; // Declare symbol table

// Pre-Conditions: Symbol table is properly declared with proper max size
// Post-Conditions: Initializes symbol table to be completely empty
extern void symtab_initialize()
{
    symtab_top = -1; // Symbol table with no active scopes

    // Set all possible scope levels to NULL
    for (int i = 0; i < MAX_NEST_LVL; i++)
    {
        symtab[i] = NULL;
    }
}

// Pre-Conditions: Symbol table is properly declared with proper max size
// Post-Conditions: Returns size of the symbol table as an unsigned int
extern unsigned int symtab_size()
{
    return symtab_top + 1;
}

// Pre-Conditions: Symbol table is properly declared with proper max size
// Post-Conditions: Returns true if symbol table is empty, false otherwise
extern bool symtab_empty()
{
    return (symtab_size() == 0);
}

// Pre-Conditions: Symbol table is properly declared with proper max size,
// scope at top of symbol table isn't NULL
// Post-Conditions: Returns the number of associations in the scope at the
// top of the symbol table
extern unsigned int symtab_scope_loc_count()
{
    return scope_loc_count(symtab[symtab_top]);
}

// Pre-Conditions: Symbol table is properly declared with proper max size,
// scope at top of symbol table isn't NULL
// Post-Conditions: Returns the current size of the scope at the top
// of the symbol table
extern unsigned int symtab_scope_size()
{
    return scope_size(symtab[symtab_top]);
}

// Pre-Conditions: Symbol table is properly declared with proper max size,
// scope at top of symbol table isn't NULL
// Post-Conditions: Returns true if the scope at the top of the symbol
// table is full, returns false otherwise
extern bool symtab_scope_full()
{
    return scope_full(symtab[symtab_top]);
}

// Pre-Conditions: Symbol table is properly declared with proper max size and
// is in an active scope (size is positive)
// Post-Conditions: Returns the current nesting level of the symbol table as
// an unsigned int
extern unsigned int symtab_current_nest_lvl()
{
    if (symtab_empty())
    {
        bail_with_error("Symbol table is not in an active scope!")
    }

    return symtab_top;
}

// Pre-Conditions: Symbol table is properly declared with proper max size
// Post-Conditions: Returns true if the symbol table is full, false otherwise
extern bool symtab_full()
{
    return (symtab_size() >= MAX_NEST_LVL);
}

// Pre-Conditions: Symbol table is properly declared with proper max size and
// my_name is not NULL
// Post-Conditions: Searches for an association involving my_name in the
// entire symbol table, returns the corresponding id_use structure if found,
// returns NULL if not found
extern id_use* symtab_lookup(const char* my_name)
{
    unsigned int lvlsOut = 0; // Start off at 0 levels out

    // Iterate down the stack from the top
    for (int i = symtab_top; i >= 0; i--)
    {
        // Look for the association involving my_name
        id_attrs* my_attrs = scope_lookup(symtab[i], my_name);

        if (my_attrs != NULL) // Association was found, create and return an id_use structure
        {
            return id_use_create(my_attrs, lvlsOut); // FREE THIS
        }

        lvlsOut++; // Not found in current scope, we have to search another level out
    }

    return NULL; // Association was not found anywhere in symbol table
}

// Pre-Conditions: Symbol table is properly declared with proper max size and
// my_name is not NULL
// Post-Conditions: Returns true if an association involving my_name is found
// anywhere in the symbol table, returns false otherwise. Effectively a boolean
// version of the symtab_lookup() function.
extern bool symtab_name_declared(const char* my_name)
{
    return (symtab_lookup(my_name) != NULL);
}

// Pre-Conditions: Symbol table is properly declared with proper max size,
// current scope at top of symbol table is not NULL, and my_name is not NULL
// Post-Conditions: Returns true if my_name has an association in the
// current scope at the top of the symbol table, returns false otherwise
extern bool symtab_name_declared_currently(const char* my_name)
{
    // Search for my_name association in top of symtab stack
    return (scope_declared(symtab[symtab_current_nest_lvl()], my_name));
}

// Pre-Conditions: Symbol table is properly declared with proper max size and
// is in an active scope (size is positive), my_name and my_attrs are not NULL, 
//current scope at top of symbol table is not NULL
// Post-Conditions: Inserts an association of my_name to my_attrs into the current
// scope at the top of the symbol table, produces error message if association
// for my_name already exists in current scope
extern void symtab_insert(const char* my_name, id_attrs* my_attrs)
{
    // If association for my_name found in current scope
    if (symtab_name_declared_currently(my_name))
    {
        bail_with_prog_error(my_attrs->file_loc, "Attempted to insert \"%s\", which has already been declared in the symbol table's current scope!", my_name);
    }

    else // Association not found, we're good to insert at current scope
    {
        scope_insert(symtab[symtab_current_nest_lvl()], my_name, my_attrs);
    }
}

// Pre-Conditions: Symbol table is properly declared with proper max size
// Post-Conditions: Enter a new scope for the symbol table
extern void symtab_enter_scope()
{
    symtab_top++; // Increment index, "pushes" another scope onto stack
    symtab[symtab_top] = scope_initialize(); // Initialize new entered scope
}

// Pre-Conditions: Symbol table is properly declared with proper max size and
// is in an active scope (not empty)
// Post-Conditions: Leaves the current scope that the symbol table is in,
// produces an error message if there are no more scopes to leave
extern void symtab_exit_scope()
{
    // Check if symbol table is in an active scope (not empty)
    if (symtab_empty())
    {
        bail_with_error("Attempted to exit scope when symbol table is not in an active scope!");
    }
    symtab_top--; // Decrement index, "pops" scope off of stack
}