// Daniel Landsman, Ryan Rohan
// scope.c: scope file, includes function bodies

#include <stdlib.h>
#include <string.h>
#include "scope.h"
#include "utilities.h"

// Pre-Conditions: None.
// Post-Conditions: Returns an empty initialized scope with a size of 0
// and no associations. Produces error message if space cannot be allocated.
scope* scope_initialize()
{
    // Attempt to allocate space for new scope
    scope* new_scope = (scope*)malloc(sizeof(scope)); // FREE THIS
    if (new_scope == NULL) bail_with_error("No space to allocate scope!");

    // Initialize size and number of associations
    new_scope->size = 0;
    new_scope->loc_count = 0;
    
    // Initialize all associations to NULL
    for (int i = 0; i < MAX_SCOPE_SIZE; i++)
    {
        new_scope->assoc_arr[i] = NULL;
    }

    return new_scope;
}

// Pre-Conditions: my_scope is not NULL.
// Post-Conditions: Frees all memory associated with the given scope.
void scope_destroy(scope* my_scope)
{
    if (my_scope == NULL) return;

    // Free each association in the scope
    for (int i = 0; i < my_scope->size; i++) {
        if (my_scope->assoc_arr[i] != NULL) {
            free(my_scope->assoc_arr[i]->attrs);  // Free attributes
            free(my_scope->assoc_arr[i]);         // Free association struct
        }
    }
    free(my_scope); // Free the scope itself
}

// Pre-Conditions: my_scope is not NULL.
// Post-Conditions: Returns the number of associations in my_scope.
address_type scope_loc_count(scope* my_scope)
{
    if (my_scope == NULL) {
        bail_with_error("Attempted to get number of associations of a NULL scope!");
    }
    return my_scope->loc_count;
}

// Pre-Conditions: my_scope is not NULL.
// Post-Conditions: Returns the current size of my_scope.
unsigned int scope_size(scope* my_scope)
{
    if (my_scope == NULL) {
        bail_with_error("Attempted to get size of a NULL scope!");
    }
    return my_scope->size;
}

// Pre-Conditions: my_scope is not NULL.
// Post-Conditions: Returns true if scope is full, false if not full.
bool scope_full(scope* my_scope)
{
    return (scope_size(my_scope) >= MAX_SCOPE_SIZE);
}

// Pre-Conditions: my_scope and my_name are not NULL.
// Post-Conditions: Returns true if my_name has an association in my_scope,
// returns false if my_name has no association in my_scope.
bool scope_declared(scope* my_scope, const char* my_name)
{
    return (scope_lookup(my_scope, my_name) != NULL);
}

// Pre-Conditions: my_scope, my_name, the associations in the scope,
// the names of all scope associations, and my_attrs are all not NULL, 
// my_name has no association in my_scope, my_scope is not full.
// Post-Conditions: Inserts an association of my_name to my_attrs into
// my_scope. Produces an error message if space cannot be allocated.
void scope_insert(scope* my_scope, const char* my_name, id_attrs* my_attrs)
{
    if (scope_declared(my_scope, my_name)) {
        bail_with_error("An association already exists for (%s)!", my_name);
    }
    if (my_attrs == NULL) {
        bail_with_error("Attempted to insert an association with NULL attributes!");
    }
    if (scope_full(my_scope)) {
        bail_with_error("Attempted to insert an association into a full scope!");
    }
    scope_assoc* new_assoc = (scope_assoc*)malloc(sizeof(scope_assoc)); // FREE THIS
    if (new_assoc == NULL) bail_with_error("No space to allocate association!");

    new_assoc->name = my_name;
    new_assoc->attrs = my_attrs;
    new_assoc->attrs->offset_count = scope_loc_count(my_scope);
    my_scope->loc_count++;
    my_scope->assoc_arr[scope_size(my_scope)] = new_assoc;
    my_scope->size++;
}

// Pre-Conditions: my_scope, my_name, the associations in the scope,
// and the names of all scope associations are not NULL.
// Post-Conditions: Returns the attributes of the association that my_name has,
// returns NULL if an association cannot be found for my_name.
id_attrs* scope_lookup(scope* my_scope, const char* my_name)
{
    if (my_name == NULL) {
        bail_with_error("Attempted to lookup a NULL name!");
    }
    for (int i = 0; i < scope_size(my_scope); i++) {
        if (my_scope->assoc_arr[i] == NULL) {
            bail_with_error("Attempting to access a NULL association!");
        }
        if (my_scope->assoc_arr[i]->name == NULL) {
            bail_with_error("Attempting to access a NULL name!");
        }
        if (!strcmp(my_scope->assoc_arr[i]->name, my_name)) {
            return my_scope->assoc_arr[i]->attrs;
        }
    }
    return NULL;
}