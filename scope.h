// Daniel Landsman
// scope.h: scope header file, includes scope data structure and function declarations

#include "id_attrs.h"
#include "id_use.h"
#include "machine_types.h"

typedef struct scope
{
    const char* name; // Name of identifier
    id_attrs* attrs; // Attributes of identifier
    struct scope* parent_scope; // Scope immediately surrounding this scope
} scope;