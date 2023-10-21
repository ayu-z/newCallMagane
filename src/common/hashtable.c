#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hashTable_t* createHashTable(keyInsertHandler keyInsertHashHandle, keyCompareHandler keyCompareHandle) 
{
    hashTable_t* hashTable = (hashTable_t*)malloc(sizeof(hashTable_t));
    hashTable->entries = (hashEntry_t**)calloc(MAX_HASH_SIZE, sizeof(hashEntry_t *));
    hashTable->keyInsertHashHandle = keyInsertHashHandle;
    hashTable->keyCompareHandle = keyCompareHandle;
    return hashTable;
}

/*
int insertKeyHashTable(hashTable_t* hashTable,  void* key, void* value) {
    unsigned int hashValue = hashTable->keyInsertHashHandle(key);
    hashEntry_t* entry = (hashEntry_t*)malloc(sizeof(hashEntry_t));
    if (entry == NULL){
        return EXIT_FAILURE;
    }
    entry->key = key;
    entry->value = value;
    entry->next = hashTable->entries[hashValue];
    hashTable->entries[hashValue] = entry;
    return 1; 
}
*/
int insertKeyHashTable(hashTable_t* hashTable, void* key, void* value) {
    unsigned int hashValue = hashTable->keyInsertHashHandle(key);
    hashEntry_t* current = hashTable->entries[hashValue];
    hashEntry_t* previous = NULL;

    while (current != NULL) {
        if (hashTable->keyCompareHandle(current->key, key) == 1) {
            current->value = value;
            return 1;
        }
        previous = current;
        current = current->next;
    }

    hashEntry_t* entry = (hashEntry_t*)malloc(sizeof(hashEntry_t));
    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    if (previous == NULL) {
        hashTable->entries[hashValue] = entry;
    } else {
        previous->next = entry;
    }

    return 1;
}



void* searchKeyHashTable(hashTable_t* hashTable,  void* key) 
{
    unsigned int hashValue = hashTable->keyInsertHashHandle(key);
    hashEntry_t* entry = hashTable->entries[hashValue];

    while (entry != NULL) {
        if (hashTable->keyCompareHandle(entry->key, key)) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

int deleteKeyHashTable(hashTable_t* hashTable,  void* key) 
{
    unsigned int hashValue = hashTable->keyInsertHashHandle(key);
    hashEntry_t* current = hashTable->entries[hashValue];
    hashEntry_t* previous = NULL;

    while (current != NULL) {
        if (hashTable->keyCompareHandle(current->key, key)) {
            if (previous == NULL) {
                hashTable->entries[hashValue] = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            return 1; 
        }
        previous = current;
        current = current->next;
    }
    return 0; 
}

void destroyHashTable(hashTable_t* hashTable) 
{
    for (int i = 0; i < MAX_HASH_SIZE; i++) {
        hashEntry_t* current = hashTable->entries[i];
        while (current != NULL) {
            hashEntry_t* next = current->next;
            free(current);
            current = next;
        }
    }
    free(hashTable->entries);
    free(hashTable);
}

unsigned int countKeysHashTable(hashTable_t* hashTable) 
{
    int count = 0;
    for (int i = 0; i < MAX_HASH_SIZE; i++) {
        hashEntry_t* entry = hashTable->entries[i];
        while (entry != NULL) {
            count++;
            entry = entry->next;
        }
    }
    return count;
}

void scanHashTable(hashTable_t* hashTable) 
{
    for (int i = 0; i < MAX_HASH_SIZE; i++) {
        hashEntry_t* entry = hashTable->entries[i];
        while (entry != NULL) {
            printf("Key: %p, Value: %s\n", entry->key, (char*)entry->value);
            entry = entry->next;
        }
    }
}
