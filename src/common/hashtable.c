#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10

// Define your custom struct for keys
typedef struct {
    int id;
    char name[50];
} KeyStruct;

typedef int (*insertHandler)(const void *);
typedef int (*matchHandler)(const void *, const void *);

typedef struct hash_entry_t {
    KeyStruct key;
    void *value;
    struct hash_entry_t* next;
} hash_entry_t;

typedef struct hashTable_t {
    hash_entry_t** entries;
    insertHandler keyInsertHandle;
    matchHandler keyMatchHandle;
} hashTable_t;

// Define a custom hash function for your struct
int customHash(const void *key) {
    const KeyStruct *k = (const KeyStruct *)key;
    return k->id % HASH_SIZE;
}

int keyMatch(const void *key1, const void *key2) {
    const KeyStruct *k1 = (const KeyStruct *)key1;
    const KeyStruct *k2 = (const KeyStruct *)key2;
    return (k1->id == k2->id && strcmp(k1->name, k2->name) == 0);
}

hashTable_t* createHashTable(insertHandler keyInsertHandle, matchHandler keyMatchHandle) {
    hashTable_t* hashTable = (hashTable_t*)malloc(sizeof(hashTable_t));
    hashTable->entries = (hash_entry_t**)calloc(HASH_SIZE, sizeof(hash_entry_t*));
    hashTable->keyInsertHandle = keyInsertHandle;
    hashTable->keyMatchHandle = keyMatchHandle;
    return hashTable;
}

int insertHashTable(hashTable_t* hashTable, const KeyStruct key, void* value) {
    unsigned int hashval = hashTable->keyInsertHandle(&key);
    hash_entry_t* entry = (hash_entry_t*)malloc(sizeof(hash_entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = hashTable->entries[hashval];
    hashTable->entries[hashval] = entry;
    return 1; // Return success (you can add error handling if needed).
}

void* search(hashTable_t* hashTable, const KeyStruct key) {
    unsigned int hashval = hashTable->keyInsertHandle(&key);

    hash_entry_t* entry = hashTable->entries[hashval];

    while (entry != NULL) {
        if (keyMatch(&entry->key, &key)) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

int main() {
    hashTable_t* hashTable = createHashTable(customHash, keyMatch);

    KeyStruct key1 = {1, "John"};
    KeyStruct key2 = {2, "Alice"};

    insertHashTable(hashTable, key1, "value1");
    insertHashTable(hashTable, key2, "value2");

    char *res1 = (char *)search(hashTable, key1);
    char *res2 = (char *)search(hashTable, key2);

    printf("res1 = %s\n", res1);
    printf("res2 = %s\n", res2);

    return EXIT_SUCCESS;
}
