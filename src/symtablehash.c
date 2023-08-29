/* Library for creating and using Symbol tables.

Hash array based implementation with linked lists for resolving conflicts */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "symtable.h"

#define HASH_MULTIPLIER 65599
#define MAX_BUCKETS 65521
#define MIN_BUCKETS 519

static unsigned const BUCKARR[] = {MIN_BUCKETS, 1021, 2053, 4093, 8191, 16381, 32771, MAX_BUCKETS};
static unsigned int SymTable_hash(unsigned int uiBuckets, const char *pcKey);
static void SymTable_change(SymTable_T oSymTable, unsigned int uiBuckets);


/* Struct that represents a binding in the symbol table. Each binding
has a pointer to a key, a pointer to a value and a pointer to the next
binding.

Note: A binding owns its key. That means each binding should have a copy of
the key. On the other hand, a binding does not own its value because it has
type (void *) */
struct abind {
    char *key;
    void *value;
    struct abind *next;
};


/* Struct that represents a symbol table as a hash table.
uiBuckets: number of buckets
uiBindings: number of bindings
array: a pointer to an array of pointers to bindings */
struct SymTable {
    unsigned int uiBindings;
    unsigned int uiBuckets;
    struct abind **array;
};


/* Computes the hash code for pcKey, an unsigned int in [0 : uiBuckets-1]

Asserts: if pcKey is NULL at runtime.

Parameters:
* uiBuckets: number of buckets
* pcKey: character array (key). Must be null terminated. */
static unsigned int SymTable_hash(unsigned int uiBuckets, const char *pcKey) {
    size_t ui;
    unsigned int uiHash;

    assert(pcKey);
    uiHash = 0U;
    for (ui = 0U; pcKey[ui] != '\0'; ui++) {
        uiHash = uiHash * HASH_MULTIPLIER + pcKey[ui];
    }
    uiHash = uiHash % uiBuckets;
    return uiHash;
}


/* Changes the number of buckets in oSymTable to uiBuckets.

uiBuckets MUST be a number in BUCKARR.

Asserts:
1) if oSymTable is NULL at runtime.
2) if necessary memory is allocated succesfully at runtime.

Parameters:
* oSymTable: a SymTable_T type
* uiBuckets: the new number of buckets */
static void SymTable_change(SymTable_T oSymTable, unsigned int uiBuckets) {
    struct abind **bind_arr, *ptr, *ptr_next;
    struct SymTable *symtable;
    unsigned int ui, uiHash;

    symtable = oSymTable;
    assert(symtable);

    /* allocate memory for a new array of pointers to bindings */
    bind_arr = malloc(uiBuckets * sizeof(struct abind *));
    assert(bind_arr);
    for (ui = 0U; ui < uiBuckets; ui++) {
        bind_arr[ui] = NULL;
    }

    for (ui = 0U; ui < (symtable->uiBuckets); ui++) {
        ptr = symtable->array[ui]; /* first binding of the bucket */
        while (ptr) {

            /* binding is inserted first in the bucket indicated by the hash of its key */
            uiHash = SymTable_hash(uiBuckets, ptr->key);
            ptr_next = ptr->next;
            ptr->next = bind_arr[uiHash];
            bind_arr[uiHash] = ptr;
            ptr = ptr_next;
        }
    }

    /* old binding array is not needed anymore */
    free(symtable->array);

    /* assign the number of buckets and the array of pointers to bindings
    to their new values. */
    symtable->uiBuckets = uiBuckets;
    symtable->array = bind_arr;

    return;
}


/* Creates a SymTable struct with no bindings and MIN_BUCKETS number of buckets. 

Asserts: if memory was allocated succesfully for oSymTable at runtime. */
SymTable_T SymTable_new(void) {
    struct SymTable *symtable;
    unsigned int ui;

    symtable = malloc(sizeof(struct SymTable));
    assert(symtable);
    symtable->array = malloc(MIN_BUCKETS * sizeof(struct abind *));
    assert(symtable->array);
    for (ui = 0U; ui < BUCKARR[0]; ui++) {
        symtable->array[ui] = NULL;
    }
    symtable->uiBindings = 0U;
    symtable->uiBuckets = MIN_BUCKETS;
    return (SymTable_T) symtable;
}


/* Frees all memory used by oSymTable.

Parameters:
* oSymTable: a SymTable_T type */
void SymTable_free(SymTable_T oSymTable) {
    struct abind *ptr, *ptr_next;
    struct SymTable *symtable;
    unsigned int ui;

    symtable = oSymTable;
    if (!symtable) {
        return;
    }
    for (ui = 0U; ui < (symtable->uiBuckets); ui++) {
        ptr = symtable->array[ui]; /* first binding of the bucket */
        while (ptr) {
            ptr_next = ptr->next;
            free(ptr->key); 
            free(ptr);
            ptr = ptr_next;
        }
    }
    free(symtable->array);
    free(symtable);
    return;
}


/* Returns the number of bindings in oSymTable.

Asserts: if oSymTable is not NULL at runtime.

Parameters:
* oSymTable: a SymTable_T type */
unsigned int SymTable_getLength(SymTable_T oSymTable) {
    struct SymTable *symtable;

    symtable = oSymTable;
    assert(symtable);

    return (symtable->uiBindings);
}


/* Checks whether a binding with key equal to pcKey is present in oSymTable.

Asserts: if oSymTable and pcKey are not NULL at runtime.

Parameters: 
* oSymTable: a SymTable_T type.
* pcKey: character array (key). Must be null terminated.

Returns: 1 if pcKey is found, 0 otherwise */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    struct abind *ptr;
    struct SymTable *symtable;
    unsigned int uiHash;

    symtable = oSymTable;
    assert(symtable);
    assert(pcKey);

    /* search only the bucket that corresponds to the pcKey hash code */
    uiHash = SymTable_hash(symtable->uiBuckets, pcKey);
    ptr = symtable->array[uiHash]; /* first binding of the bucket */
    
    while (ptr) {
        if (!strcmp(ptr->key, pcKey)) {
            return 1;
        }
        ptr = ptr->next;
    }
    return 0;
}


/* Creates a new binding for oSymTable from a given pcKey and pvValue.

Asserts: 
1) if oSymTable and pcKey are not NULL at runtime.
2) if necessary memory was allocated succesfully at runtime.

Parameters:
* oSymTable: a SymTable_T type
* pcKey: a character array (key). Must be null terminated.
* pvValue: pointer to any value */
void SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
    struct abind *new_bind, *ptr;
    struct SymTable *symtable;
    char *new_key;
    unsigned int uiHash, uiBuckets;
    int idx = 0;
    
    symtable = oSymTable;
    assert(symtable);
    assert(pcKey);

    /* search only the bucket that corresponds to the pcKey hash code */
    uiHash = SymTable_hash(symtable->uiBuckets, pcKey);
    ptr = symtable->array[uiHash]; /* first binding of the bucket */

    while (ptr) {
        if (!strcmp(ptr->key, pcKey)) {
            ptr->value = (void *) pvValue;
            return;
        }
        ptr = ptr->next;
    }

    /* find if #buckets need to increase, if so, call Symtable_change */
    uiBuckets = symtable->uiBuckets;
    while (((symtable->uiBindings) >= uiBuckets) && (uiBuckets != MAX_BUCKETS)) {
        uiBuckets = BUCKARR[++idx];
    }
    if (uiBuckets != symtable->uiBuckets) {
        SymTable_change(symtable, uiBuckets);
    }

    /* allocate memory for a new binding */
    new_bind = malloc(sizeof(struct abind));
    assert(new_bind);
    new_key = malloc((strlen(pcKey) + 1) * sizeof(char));
    assert(new_key);

    /* initialize binding */
    strcpy(new_key, pcKey);
    new_bind->key = new_key;
    new_bind->value = (void *) pvValue;

    /* binding is inserted in the bucket indicated by the hash of its key 
    and before every other binding */
    uiHash = SymTable_hash(uiBuckets, pcKey);
    new_bind->next = symtable->array[uiHash];
    symtable->array[uiHash] = new_bind;

    symtable->uiBindings += 1;
}


/* Applies function pfApply to every binding in oSymTable.

Asserts: if oSymTable and pfApply are not NULL at runtime.

Parameters:
* oSymTable: a SymTable_T type
* pfApply: function to apply
* pvExtra: a pointer to any value. Used by pfApply. */
void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), const void *pvExtra) {
    struct abind *ptr;
    struct SymTable *symtable;
    unsigned int ui;

    symtable = oSymTable;
    assert(symtable);
    assert(pfApply);
    
    for (ui = 0U; ui < (symtable->uiBuckets); ui++) {
        ptr = symtable->array[ui]; /* first binding of the bucket */
        while (ptr) {
            pfApply(ptr->key, ptr->value, (void *) pvExtra);
            ptr = ptr->next;
        }
    }
}


/* Finds in oSymTable a binding with key equal to pcKey.

Asserts: if oSymTable and pcKey are not NULL at runtime.

Parameters:
* oSymTable: a SymTable_T type
* pcKey: a character array (key). Must be null terminated.

Returns: a pointer to the value or NULL if such binding was not found. */
void* SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    struct abind *ptr;
    struct SymTable *symtable;
    unsigned int uiHash;

    symtable = oSymTable;
    assert(symtable);
    assert(pcKey);

    /* search only the bucket that corresponds to the pcKey hash code */
    uiHash = SymTable_hash(symtable->uiBuckets, pcKey);
    ptr = symtable->array[uiHash]; /* first binding of the bucket */

    while (ptr) {
        if (!strcmp(ptr->key, pcKey)) {
            return ptr->value;
        }
        ptr = ptr->next;
    }
    return NULL;
}


/* Removes a binding with key equal to pcKey.

Asserts: if oSymTable and pcKey are not NULL at runtime.

Parameters:
* oSymTable: a SymTable_T type
* pcKey: a character array (key). Must be null terminated.

Returns: 1 if removal was successful, 0 if such binding was not found */
int SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    struct abind *ptr, *ptr_prev;
    struct SymTable *symtable;
    unsigned int uiHash;

    symtable = oSymTable;
    assert(symtable);
    assert(pcKey);

    /* search only the bucket that corresponds to the pcKey hash code */
    uiHash = SymTable_hash(symtable->uiBuckets, pcKey);
    ptr = symtable->array[uiHash]; /* first binding of the bucket */

    while (ptr) {
        /* compare pcKey with the key of each binding */
        if (strcmp(ptr->key, pcKey)) {  
            ptr_prev = ptr;
            ptr = ptr->next;
            continue;       
        }

        /* pcKey was found. when it is in first position, update the first
        key to point to the 2nd one. when not in first position, update the
        previous key to point to the next one. */
        if (ptr == symtable->array[uiHash]) {
            symtable->array[uiHash] = ptr->next;
        }
        else {
            ptr_prev->next = ptr->next;
        }
        
        symtable->uiBindings -= 1;
        free(ptr->key);
        free(ptr);
        return 1;
    }
    return 0;
}

/* Prints basic information about the hashtable:
1) Max bindings in a bucket.
2) Min binding in a bucket.
3) Weighted average bucket size */
void SymTable_stats(SymTable_T oSymTable) {
    struct abind* ptr;
    struct SymTable *symtable;
    unsigned int ui, num_bindings, max_bindings, min_bindings, non_empty;

    symtable = oSymTable;
    assert(symtable);

    max_bindings = 0U;
    non_empty = 0U;
    min_bindings = symtable->uiBindings;
    for (ui = 0U; ui < symtable->uiBuckets; ui++) {
        ptr = symtable->array[ui];
        if (ptr) {
            non_empty++;
        }
        num_bindings = 0U;
        while (ptr) {
            ptr = ptr->next;
            num_bindings++;
        }
        if (num_bindings > max_bindings) {
            max_bindings = num_bindings;
        }
        if (num_bindings < min_bindings) {
            min_bindings = num_bindings;
        }
    }
    printf("++> Max #bindings in a bucket: %d\n", max_bindings);
    printf("++> Min #bindings in a bucket: %d\n", min_bindings);
    printf("++> Weighted average bucket size: %f\n", symtable->uiBindings / (float) non_empty);
}
