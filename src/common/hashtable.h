#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#define MAX_HASH_SIZE 0xffff


typedef unsigned int (*keyInsertHandler)( void *);
typedef unsigned int (*keyCompareHandler)( void *,  void *);

typedef struct hashEntry_t {
    void *key;
    void *value;
    struct hashEntry_t* next;
} hashEntry_t;

typedef struct hashTable_t {
    hashEntry_t** entries;
    keyInsertHandler keyInsertHashHandle;
    keyCompareHandler keyCompareHandle; 
} hashTable_t;

hashTable_t* createHashTable(keyInsertHandler keyInsertHashHandle, keyCompareHandler keyCompareHandle);
int insertKeyHashTable(hashTable_t* hashTable,  void* key, void* value);
void* searchKeyHashTable(hashTable_t* hashTable,  void* key);
unsigned int countKeysHashTable(hashTable_t* hashTable);
int deleteKeyHashTable(hashTable_t* hashTable,  void* key);
void destroyHashTable(hashTable_t* hashTable);

unsigned long djb2Hash(char *str);
void scanHashTable(hashTable_t* hashTable) ;

#endif // !__HASHTABLE_H__



