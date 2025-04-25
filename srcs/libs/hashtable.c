#include "hashtable.h"

unsigned int hash_string(const char *str) {
    unsigned int hash = 0;
    
    for (unsigned int i = 0; str[i]; i++) {
        hash = hash * 31 + str[i];
    }
    
    return hash;
}

t_hashtable *ht_create(size_t size) {
    t_hashtable *ht = malloc(sizeof(t_hashtable));
    if (!ht)
        return NULL;
        
    ht->size = size;
    ht->entries = calloc(size, sizeof(t_ht_entry*));
    if (!ht->entries) {
        free(ht);
        return NULL;
    }
    
    return ht;
}

void ht_set(t_hashtable *ht, const char *key, void *value) {
    if (!ht || !key)
        return;
        
    unsigned int index = hash_string(key) % ht->size;
    t_ht_entry *entry = ht->entries[index];
    
    // Check if key already exists
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    
    // Create new entry
    entry = malloc(sizeof(t_ht_entry));
    if (!entry)
        return;
        
    entry->key = strdup(key);
    entry->value = value;
    entry->next = ht->entries[index];
    ht->entries[index] = entry;
}

void *ht_get(t_hashtable *ht, const char *key) {
    if (!ht || !key)
        return NULL;
        
    unsigned int index = hash_string(key) % ht->size;
    t_ht_entry *entry = ht->entries[index];
    
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    
    return NULL;
}

void ht_delete(t_hashtable *ht, const char *key) {
    if (!ht || !key)
        return;
        
    unsigned int index = hash_string(key) % ht->size;
    t_ht_entry *entry = ht->entries[index];
    t_ht_entry *prev = NULL;
    
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                ht->entries[index] = entry->next;
            }
            
            free(entry->key);
            free(entry);
            return;
        }
        
        prev = entry;
        entry = entry->next;
    }
}

void ht_destroy(t_hashtable *ht)
{
    if (!ht)
        return;
        
    for (size_t i = 0; i < ht->size; i++) {
        t_ht_entry *entry = ht->entries[i];
        
        while (entry) {
            t_ht_entry *next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    
    free(ht->entries);
    free(ht);
}