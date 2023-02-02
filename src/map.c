#include "map.h"

#include "hash/xxhash.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef __GCC__
#define __builtin_expect(EXP, C) (EXP)
#endif

// 左移4位，16
#define MIN_HASH_TABLE_SIZE (1 << 4)

typedef struct hash_node_s
{
    struct hash_node_s *next;
    struct hash_node_s *prev;
    void *key;
    size_t key_len;
    uint32_t full_hash;
    void *value;
} hash_node_t;

typedef struct
{
    hash_node_t *head;
} hash_entry_t;

typedef struct
{
    hash_entry_t *hash_table;
    uint32_t hash_mask;
    size_t table_len;
    size_t item_len;
    size_t decrease_th;
    // for forEach
    size_t iterator_cnt;
    hash_node_t *iterator_node;
} map_t;

/* pre defines */
static inline uint32_t get_hash(uint8_t *data, size_t len) { return (uint32_t)XXH3_64bits(data, len); }
static inline hash_node_t *map_locate(map_t *map, void *key, size_t key_len, hash_entry_t **p_entry);
static inline void try_increase_hash_table(map_t *map);
static inline void try_decrease_hash_table(map_t *map);
/* public methods */

map_handle_t map_create(void) {
    map_t *map = malloc(sizeof(map_t));
    if (!map) goto error;
    memset(map, 0, sizeof(map_t));
    map->item_len = 0;
    // decrase threshold = table_len/4
    // decrase threshold is 0 if it < min_hash_table_size/2
    map->decrease_th = 0;
    map->table_len = MIN_HASH_TABLE_SIZE;
    map->hash_mask = (uint32_t)(map->table_len) - 1;
    map->hash_table = malloc(sizeof(hash_entry_t) * (map->table_len));
    if (!map->hash_table) goto error;
    memset(map->hash_table, 0, sizeof(hash_entry_t) * (map->table_len));

    return (map_handle_t)map;
error:
    if (map) {
        if (map->hash_table != NULL) free(map->hash_table);
        free(map);
    }
    return NULL;
}

int map_clear(map_handle_t handle, void (*free_value)(void *value, void *ctx), void *ctx) {
    if (__builtin_expect(!handle, 0)) return -1;
    map_t *map = (map_t *)handle;
    /* free values */
    for (size_t i = 0; i < map->table_len; i++) {
        hash_entry_t *entry = &(map->hash_table[i]);
        if (entry->head != NULL) {
            hash_node_t *node = entry->head;
            while (node != NULL) {
                hash_node_t *next = node->next;
                if (free_value != NULL) { free_value(node->value, ctx); }
                // key & node use one block of memory
                free(node);
                node = next;
            }
        }
    }
    /* reset hash table */
    map->item_len = 0;
    map->decrease_th = 0;
    map->table_len = MIN_HASH_TABLE_SIZE;
    map->hash_mask = (uint32_t)(map->table_len) - 1;
    map->hash_table = realloc(map->hash_table, sizeof(hash_entry_t) * (map->table_len));
    if (!map->hash_table) return -1;
    memset(map->hash_table, 0, sizeof(hash_entry_t) * (map->table_len));
    return 0;
}

int map_delete(map_handle_t handle, void (*free_value)(void *value, void *ctx), void *ctx) {
    int result = map_clear(handle, free_value, ctx);
    if (result != 0) return result;
    map_t *map = (map_t *)handle;
    free(map->hash_table);
    free(map);
    return 0;
}

void *map_add(map_handle_t handle, void *key, size_t key_len, void *value) {
    if (__builtin_expect(!handle, 0)) return NULL;
    map_t *map = (map_t *)handle;

    // check for repeat key
    uint32_t full_hash = get_hash(key, key_len);
    uint32_t hash = full_hash & map->hash_mask;
    hash_entry_t *entry = &(map->hash_table[hash]);
    hash_node_t *old_node = entry->head;
    while (old_node) {
        if (old_node->key_len == key_len) {
            if (memcmp(old_node->key, key, key_len) == 0) {
                void *old_value = old_node->value;
                old_node->value = value;
                return old_value;
            }
        }
        old_node = old_node->next;
    }

    // key & node use one block of memory
    hash_node_t *new_node = malloc(sizeof(hash_node_t) + key_len);
    if (__builtin_expect(!new_node, 0)) return NULL;

    // redundant operation
    // new_node->next = NULL;
    new_node->prev = NULL;
    new_node->key = (uint8_t *)(new_node + 1);
    new_node->key_len = key_len;
    memcpy(new_node->key, key, key_len);
    new_node->value = value;
    new_node->full_hash = full_hash;
    new_node->next = entry->head;
    if (new_node->next != NULL) new_node->next->prev = new_node;
    entry->head = new_node;
    map->item_len++;

    try_increase_hash_table(map);

    return value;
}


void *map_remove(map_handle_t handle, void *key, size_t key_len) {
    if (__builtin_expect(!handle, 0)) return NULL;
    map_t *map = (map_t *)handle;
    hash_entry_t *entry = NULL;
    hash_node_t *node = map_locate(map, key, key_len, &entry);
    if (__builtin_expect(!node, 0)) return NULL;
    void *value = node->value;
    if (node->next) node->next->prev = node->prev;
    if (node->prev)
        node->prev->next = node->next;
    else
        entry->head = node->next;
    free(node);
    map->item_len--;

    try_decrease_hash_table(map);

    return value;
}

void *map_get(map_handle_t handle, void *key, size_t key_len) {
    if (__builtin_expect(!handle, 0)) return NULL;
    map_t *map = (map_t *)handle;
    hash_node_t *node = map_locate(map, key, key_len, NULL);
    if (node) return node->value;
    return NULL;
}

bool map_has(map_handle_t handle, void *key, size_t key_len) {
    if (__builtin_expect(!handle, 0)) return false;
    map_t *map = (map_t *)handle;
    return map_locate(map, key, key_len, NULL) != NULL;
}

size_t map_get_length(map_handle_t handle) {
    if (__builtin_expect(!handle, 0)) return -1;
    map_t *map = (map_t *)handle;
    return map->item_len;
}


map_key_t *map_keys(map_handle_t handle, size_t *len) {
    if (__builtin_expect(!handle, 0)) {
        *len = 0;
        return NULL;
    }
    map_t *map = (map_t *)handle;
    map_key_t *keys = malloc(sizeof(map_key_t) * (map->item_len));
    if (__builtin_expect(!keys, 0)) {
        *len = 0;
        return NULL;
    }
    size_t i = 0;
    for (size_t j = 0; j < map->table_len; j++) {
        hash_entry_t *entry = &(map->hash_table[j]);
        hash_node_t *node = entry->head;
        while (node != NULL) {
            keys[i].key = node->key;
            keys[i].len = node->key_len;
            i++;
            node = node->next;
        }
    }
    *len = map->item_len;
    return keys;
}

// Values are persist if they are not freed in map_delete.
void **map_values(map_handle_t handle, size_t *len) {
    if (__builtin_expect(!handle, 0)) {
        *len = 0;
        return NULL;
    }
    map_t *map = (map_t *)handle;
    void **values = malloc(sizeof(void *) * (map->item_len));
    if (__builtin_expect(!values, 0)) {
        *len = 0;
        return NULL;
    }
    size_t i = 0;
    for (size_t j = 0; j < map->table_len; j++) {
        hash_entry_t *entry = &(map->hash_table[j]);
        hash_node_t *node = entry->head;
        while (node != NULL) {
            values[i] = node->value;
            i++;
            node = node->next;
        }
    }
    *len = map->item_len;
    return values;
}

map_entry_t *map_entries(map_handle_t handle, size_t *len) {
    if (__builtin_expect(!handle, 0)) {
        *len = 0;
        return NULL;
    }
    map_t *map = (map_t *)handle;
    map_entry_t *entries = malloc(sizeof(map_entry_t) * (map->item_len));
    if (__builtin_expect(!entries, 0)) {
        *len = 0;
        return NULL;
    }
    size_t i = 0;
    for (size_t j = 0; j < map->table_len; j++) {
        hash_entry_t *entry = &(map->hash_table[j]);
        hash_node_t *node = entry->head;
        while (node != NULL) {
            entries[i].key.key = node->key;
            entries[i].key.len = node->key_len;
            entries[i].value = node->value;
            i++;
            node = node->next;
        }
    }
    *len = map->item_len;
    return entries;
}

int map_forEach_start(map_handle_t handle, map_entry_t *entry) {
    if (__builtin_expect(!handle, 0)) { return -1; }
    map_t *map = (map_t *)handle;
    map->iterator_cnt = 0;
    map->iterator_node = (map->hash_table[0]).head;
    return map_forEach_next(handle, entry);
}

int map_forEach_next(map_handle_t handle, map_entry_t *entry) {
    map_t *map = (map_t *)handle;
    // get next valid node
    while (map->iterator_node == NULL) {
        map->iterator_cnt++;
        if (map->iterator_cnt >= map->table_len) return -1;
        map->iterator_node = (map->hash_table[map->iterator_cnt]).head;
    }
    // set entry
    entry->key.key = map->iterator_node->key;
    entry->key.len = map->iterator_node->key_len;
    entry->value = map->iterator_node->value;
    // progress to next
    map->iterator_node = map->iterator_node->next;
    return 0;
}

/* private methods */

static inline hash_node_t *map_locate(map_t *map, void *key, size_t key_len, hash_entry_t **p_entry) {
    uint32_t full_hash = get_hash(key, key_len);
    uint32_t hash = full_hash & map->hash_mask;
    hash_entry_t *entry = &(map->hash_table[hash]);
    hash_node_t *node = entry->head;
    while (node != NULL) {
        if (key_len == node->key_len) {
            if (memcmp(key, node->key, key_len) == 0) break;
        }
        node = node->next;
    }
    if (p_entry) *p_entry = entry;
    return node;
}

static inline void try_increase_hash_table(map_t *map) {
    if (__builtin_expect(map->item_len <= map->table_len, 1)) return;
    size_t old_len = map->table_len;
    void *new_table = realloc(map->hash_table, sizeof(hash_entry_t) * old_len * 2);
    if (!new_table) return;
    map->hash_table = new_table;
    memset((map->hash_table) + old_len, 0, sizeof(hash_entry_t) * old_len);
    map->table_len = old_len * 2;
    map->hash_mask = (uint32_t)(map->table_len) - 1;
    map->decrease_th = map->table_len / 4;
    // this is redundant
    // if(map->decrease_th < MIN_HASH_TABLE_SIZE/2)
    // map->decrease_th = 0;
    // adjust nodes
    uint32_t select_mask = (uint32_t)old_len;
    for (size_t i = 0; i < old_len; i++) {
        hash_entry_t *entry = &(map->hash_table[i]);
        hash_entry_t *new_entry = entry + old_len;
        hash_node_t *node = entry->head;
        while (node != NULL) {
            hash_node_t *next = node->next;
            if (node->full_hash & select_mask) {
                // move node
                if (node->next) node->next->prev = node->prev;
                if (node->prev)
                    node->prev->next = node->next;
                else
                    entry->head = node->next;
                node->prev = NULL;
                node->next = new_entry->head;
                if (new_entry->head) new_entry->head->prev = node;
                new_entry->head = node;
            }
            node = next;
        }
    }
}

static inline void try_decrease_hash_table(map_t *map) {
    if (__builtin_expect(map->item_len >= map->decrease_th, 1)) return;
    size_t old_len = map->table_len;
    size_t new_len = old_len / 2;

    for (size_t i = new_len; i < old_len; i++) {
        hash_entry_t *entry = &(map->hash_table[i]);
        if (entry->head) {
            hash_entry_t *new_entry = &(map->hash_table[i - new_len]);
            hash_node_t *old_head = entry->head;
            hash_node_t *old_tail = entry->head;
            while (old_tail->next) old_tail = old_tail->next;
            if (new_entry->head) new_entry->head->prev = old_tail;
            old_tail->next = new_entry->head;
            new_entry->head = old_head;
            // this is redundant
            // old_head->head = NULL;
        }
    }

    map->hash_table = realloc(map->hash_table, sizeof(hash_entry_t) * new_len);
    map->table_len = new_len;
    map->hash_mask = (uint32_t)(map->table_len) - 1;
    map->decrease_th = map->table_len / 4;
    if (map->decrease_th < MIN_HASH_TABLE_SIZE / 2) map->decrease_th = 0;
}

/* diagnostic */

float map_get_conflict_ratio(map_handle_t *handle) {
    map_t *map = (map_t *)handle;
    size_t conflict_cnt = 0;
    for (size_t i = 0; i < map->table_len; i++) {
        hash_node_t *node = map->hash_table[i].head;
        size_t entry_len = 0;
        while (node != NULL) {
            entry_len++;
            node = node->next;
        }
        if (entry_len > 1) conflict_cnt += entry_len;
    }
    return ((float)conflict_cnt) / ((float)map->item_len);
}

float map_get_average_ops(map_handle_t *handle) {
    map_t *map = (map_t *)handle;
    size_t working_entry_cnt = 0;
    for (size_t i = 0; i < map->table_len; i++) {
        if (map->hash_table[i].head != NULL) working_entry_cnt++;
    }
    return ((float)map->item_len) / ((float)working_entry_cnt);
}

size_t map_get_max_ops(map_handle_t *handle) {
    map_t *map = (map_t *)handle;
    size_t max_entry_len = 0;
    for (size_t i = 0; i < map->table_len; i++) {
        hash_node_t *node = map->hash_table[i].head;
        size_t entry_len = 0;
        while (node != NULL) {
            entry_len++;
            node = node->next;
        }
        if (entry_len > max_entry_len) max_entry_len = entry_len;
    }
    return max_entry_len;
}
