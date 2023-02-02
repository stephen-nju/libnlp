#ifndef __MAP_H
#define __MAP_H

#include <stdbool.h>
#include <stddef.h>

typedef void *map_handle_t;

// only used for return values
typedef struct
{
    void *key;
    size_t len;
} map_key_t;
typedef struct
{
    map_key_t key;
    void *value;
} map_entry_t;

map_handle_t map_create(void);
/*
    Delete the map. NOT for removing one item in the map.
    Use free_value and ctx to free the remain values if needed.
    Those two can be NULL if not used.
*/
int map_delete(map_handle_t handle, void (*free_value)(void *value, void *ctx), void *ctx);

/*
    Return NULL if failed.
    Return value if key is new.
    Return the replaced value if key exists.
*/
void *map_add(map_handle_t handle, void *key, size_t key_len, void *value);
/*
    Return the removed value.
    If there is no such entry, return NULL.
*/
void *map_remove(map_handle_t handle, void *key, size_t key_len);
/*
    Clear the map.
    Use free_value and ctx to free the values themselves.
    Those two can be NULL if not used.
*/
int map_clear(map_handle_t handle, void (*free_value)(void *value, void *ctx), void *ctx);
/* Return NULL if the entry does not exist */
void *map_get(map_handle_t handle, void *key, size_t key_len);
bool map_has(map_handle_t handle, void *key, size_t key_len);
size_t map_get_length(map_handle_t handle);

/*
    Return the map keys as a map_key_t* array.
    The array length is set to len.
    DO NOT modify the keys! For they are references to the map keys.
    Return value needs to be freed.
*/
map_key_t *map_keys(map_handle_t handle, size_t *len);
/*
    Return the map values as a void* array.
    The array length is set to len.
    Return value needs to be freed.
*/
void **map_values(map_handle_t handle, size_t *len);
/*
    Return the map values as a map_entry_t array.
    The array length is set to len.
    DO NOT modify the keys! For they are references to the map keys.
    Return value needs to be freed.
*/
map_entry_t *map_entries(map_handle_t handle, size_t *len);

/* DO NOT use this directly */
int map_forEach_start(map_handle_t handle, map_entry_t *entry);
/* DO NOT use this directly */
int map_forEach_next(map_handle_t handle, map_entry_t *entry);
/*
    Map forEach iterator. The order has nothing to do with the insertion order.
    handle is a map_handle_t, entry is a map_entry_t.
    DO NOT modify the map during this operation!
*/
#define map_forEach(handle, entry)                                            \
    for (int i_f921f793 = map_forEach_start(handle, &entry); i_f921f793 == 0; \
         i_f921f793 = map_forEach_next(handle, &entry))

/* for diagnose */
float map_get_conflict_ratio(map_handle_t *handle);
float map_get_average_ops(map_handle_t *handle);
size_t map_get_max_ops(map_handle_t *handle);

#endif