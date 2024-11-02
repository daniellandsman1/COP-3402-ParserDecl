// Daniel Landsman
// scope.h: scope header file, includes scope data structure and function declarations
#ifndef _SCOPE_H
#define _SCOPE_H

#include <stdbool.h>
#include "id_attrs.h"
#include "id_use.h"
#include "machine_types.h"

#define MAX_SCOPE_SIZE 4096

typedef struct
{
    const char* name; // Name of identifier
    id_attrs* attrs; // Attributes of identifier
} scope_assoc;

typedef struct
{
    unsigned int size; // Size of scope
    unsigned int loc_count; // Number of associations in scope
    scope_assoc* assoc_arr[MAX_SCOPE_SIZE]; // Array of pointers to scope associations
} scope;

// Pre-Conditions: None.
// Post-Conditions: Returns an empty initialized scope with a size of 0
// and no associations. Produces error message if space cannot be allocated.
extern scope* scope_initialize();

// Pre-Conditions: my_scope is not NULL.
// Post-Conditions: Frees all memory associated with the given scope.
extern void scope_destroy(scope *my_scope);  // Added declaration for scope_destroy

// Pre-Conditions: my_scope is not NULL.
// Post-Conditions: Returns the number of associations in my_scope.
extern address_type scope_loc_count(scope* my_scope);

// Pre-Conditions: my_scope is not NULL.
// Post-Conditions: Returns the current size of my_scope.
extern unsigned int scope_size(scope* my_scope);

// Pre-Conditions: my_scope is not NULL.
// Post-Conditions: Returns true if scope is full, false if not full.
extern bool scope_full(scope* my_scope);

// Pre-Conditions: my_scope and my_name are not NULL.
// Post-Conditions: Returns true if my_name has an association in my_scope,
// returns false if my_name has no association in my_scope.
extern bool scope_declared(scope* my_scope, const char* my_name);

// Pre-Conditions: my_scope, my_name, the associations in the scope,
// the names of all scope associations, and my_attrs are all not NULL, 
// my_name has no association in my_scope, my_scope is not full.
// Post-Conditions: Inserts an association of my_name to my_attrs into
// my_scope. Produces an error message if space cannot be allocated.
extern void scope_insert(scope* my_scope, const char* my_name, id_attrs* my_attrs);

// Pre-Conditions: my_scope, my_name, the associations in the scope,
// and the names of all scope associations are not NULL.
// Post-Conditions: Returns the attributes of the association that my_name has,
// returns NULL if an association cannot be found for my_name.
extern id_attrs* scope_lookup(scope* my_scope, const char* my_name);

#endif