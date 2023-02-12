/* Library for creating and using Symbol tables */

#ifndef SYMTABLE_INCLUDE
#define SYMTABLE_INCLUDE

#include <stdio.h>

typedef void* SymTable_T;


/* Creates a SymTable struct with no bindings. 

Asserts: if memory was allocated succesfully for oSymTable at runtime. */
SymTable_T SymTable_new(void);


/* Frees all memory used by oSymTable.

Parameters:
* oSymTable: a SymTable_T type */
void SymTable_free(SymTable_T oSymTable);


/* Returns the number of bindings in oSymTable.

Asserts: if oSymTable is not NULL at runtime.

Parameters:
* oSymTable: a SymTable_T type */
unsigned int SymTable_getLength(SymTable_T oSymTable);


/* Creates a new binding for oSymTable from a given pcKey and pvValue.

Asserts: 
1) if oSymTable and pcKey are not NULL at runtime.
2) if necessary memory was allocated succesfully at runtime.

Parameters:
* oSymTable: a SymTable_T type
* pcKey: a character array (key). Must be null terminated.
* pvValue: pointer to any value

Returns: 1 if binding was created succesfully, 0 if there is already
a binding with key equal to pcKey. */
int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue);


/* Removes a binding with key equal to pcKey.

Asserts: if oSymTable and pcKey are not NULL at runtime.

Parameters:
* oSymTable: a SymTable_T type
* pcKey: a character array (key). Must be null terminated.

Returns: 1 if removal was successful, 0 if such binding was not found */
int SymTable_remove(SymTable_T oSymTable, const char *pcKey);


/* Checks whether a binding with key equal to pcKey is present in oSymTable.

Asserts: if oSymTable and pcKey are not NULL at runtime.

Parameters: 
* oSymTable: a SymTable_T type.
* pcKey: character array (key). Must be null terminated.

Returns: 1 if pcKey is found, 0 otherwise */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);


/* Finds in oSymTable a binding with key equal to pcKey.

Asserts: if oSymTable and pcKey are not NULL at runtime.

Parameters:
* oSymTable: a SymTable_T type
* pcKey: a character array (key). Must be null terminated.

Returns: a pointer to the value or NULL if such binding was not found. */
void* SymTable_get(SymTable_T oSymTable, const char *pcKey);


/* Applies function pfApply to every binding in oSymTable.

Asserts: if oSymTable and pfApply are not NULL at runtime.

Parameters:
* oSymTable: a SymTable_T type
* pfApply: function to apply
* pvExtra: a pointer to any value. Used by pfApply. */
void  SymTable_map(SymTable_T oSymTable,
        void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
        const void *pvExtra);


/* Prints basic information about the hashtable:
1) Max bindings in a bucket.
2) Min binding in a bucket.
3) Weighted average bucket size */
void SymTable_stats(SymTable_T oSymTable);

#endif
