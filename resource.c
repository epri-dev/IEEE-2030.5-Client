// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

/** @defgroup resource Resource

    Provides a container and database for client and server IEEE 2030.5
    resources.
    @{
*/

/** @brief Resource structure */
typedef struct _Resource {
  struct _Resource *next; //< pointer to the next resource with the same name
  char *name; //< name of Resource (the path component of the href)
  void *data; //< pointer to a 2030.5 object 
  int type; //< the schema type for the object
  ListInfo *info; //< pointer to the ListInfo for 2030.5 List objects
  int64_t time; //< the time the resource was created or last updated
} Resource;

// pointer to the list field of a list type resource
// ListInfo must be valid
#define list_field(r) se_list_field (((Resource *)r)->data, \
				     ((Resource *)r)->info)
#define resource_name(r) ((Resource *)r)->name
#define resource_data(r) ((Resource *)r)->data
#define resource_type(r) ((Resource *)r)->type

/** @brief Find a resource with the matching name.
    @param name is a pointer to a string of the name to match
    @returns a pointer to a Resource with a matching name, or NULL if none were
    found.
*/
void *find_resource (void *name);

/** @brief Insert a resource into the database.
    @param res is a pointer to a Resource
*/
void insert_resource (void *res);

/** @brief Delete a resource with the name from the database and return it.
    @param name is a pointer to a string that contains the name
    @returns a pointer to a Resource with a matching name, or NULL if none were
    found.
*/
void *delete_resource (void *name);

/** @brief Create a new resource.
    @param size is the size of the resource, should be sizeof (Resource) or
    sizeof (type) for a subtype of Resource
    @param name is a pointer to string with the name of the resource
    @param data is pointer to a IEEE 2030.5 object
    @param type is the schema type for the object.
    @returns a pointer to newly allocated resource
*/
void *new_resource (int size, char *name, void *data, int type);

/** @brief Free a resource.

    Frees the name and IEEE 2030.5 object (if any)
    @param res is a pointer to a Resource.
*/
void free_resource (void *res);

/** @brief Initialize the Resource database. */
void resource_init ();

/** @} */

#include <string.h>

void *resource_key (void *data) {
  Resource *r = data; return r->name;
}

// find_resource, insert_resource, delete_resource, resource_init
global_hash(resource, string, 512)

void *new_resource (int size, char *name, void *data, int type) {
  Resource *r = calloc (1, size);
  r->name = strdup (name); r->data = data; r->type = type; 
  r->info = find_list_info (type);
  return r;
}

void free_resource (void *res) { Resource *r = res;
  if (r->data) free_se_object (r->data, r->type);
  free (r->name); free (r);
}
