// Daniel Landsman
// scope.c: scope file, includes function bodies

#include <string.h>
#include "scope.h"
#include "utilities.h"

scope* init_scope(file_location floc, id_kind k, unsigned int ofst_cnt, const char* ident_name, scope* surr_scope)
{
    scope* new_scope = (scope*)malloc(sizeof(scope)); // FREE THIS
    if (new_scope == NULL) bail_with_error("No space to allocate scope!");

    strcpy(new_scope->name, ident_name); // NEED TO ALLOCATE MEMORY HERE?
    new_scope->attrs = create_id_attrs(floc, k, ofst_cnt); // FREE THIS
    new_scope->parent_scope = surr_scope;

    return new_scope;
}