/* Test file for the Symbol table library */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include "symtable.h"

#define NTABLES 1   /* number of tables to create */
#define DEBUG 0     /* 1 or 0: print intermediate results or not */

void print_bind(const char *pcKey, void *pvValue, void *pvExtra);
void update_bind(const char *pcKey, void *pvValue, void *pvExtra);
char** random_keys(char *alphabet, int num_keys, int max_key_len);
void random_actions(SymTable_T oSymTable, char **keys, int num_keys, int* values);


/*  main

Parameters:
argc: number of command line arguments. Can be 1 (will run default actions)
or 4 (to create random tables).
argv: command line arguments. 
    1st argument: executable file name
    2nd argument: number of the keys in the array
    3rd argument: maximum key length
    4th argument: characters to be used for creating the keys
    5th argument: number of iterations of actions on each table */
int main(int argc, char** argv) {
    SymTable_T oSymTable;
    int i, j;
    int iter;           /* number of iterations of actions on each table */
    int max_key_len;    /* maximum key length */
    int num_keys;       /* number of keys to create */
    char **keys;        /* array of character keys */
    int *values;        /* array of integer values */
    char *alphabet;     /* array of the available characters for a key */
    clock_t start, end;
    double cpu_time_used;

    /* extra command line arguments: random table is created */
    if (argc != 1) {
        if (argc != 5) {
            printf("Usage: %s {NUM_KEYS} {MAX_KEY_LEN} {ALPHABET} {NUM_ITER}\n", argv[0]);
            return 1;
        }
        srand(getpid());

        num_keys = atoi(argv[1]);
        max_key_len = atoi(argv[2]);
        alphabet = argv[3];
        iter = atoi(argv[4]);

        /* initialize values to random integers */
        values = malloc(num_keys * sizeof(int));
        assert(values);
        for (i = 0; i < num_keys; i++) {
            values[i] =  rand() % num_keys + 1;
        }

        /* generate an array of random keys */
        keys = random_keys(alphabet, num_keys, max_key_len);

        for (i = 0; i < NTABLES; i++) {
            printf("++> ----------Creating table #%d----------\n", i+1);
            oSymTable = SymTable_new();

            for (j = 0; j < iter; j++) {
                printf("++> ----------Iteration %d----------\n", j+1);
                start = clock();
                random_actions(oSymTable, keys, num_keys, values);
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("++> CPU time: %f\n", cpu_time_used);
            }
            
            /* free memory */
            printf("++> Deleting table...");
            SymTable_free(oSymTable);
            printf("DONE\n");
        }
        for (i = 0; i < num_keys; i++) {
            free(keys[i]);
        }
        free(keys);
        free(values);
    }

    /* no extra command line arguments: manually create and test tables below */
    else {
        printf("No tables specified\n");
        printf("To run random tests use:\n");
        printf("%s {NUM_KEYS} {MAX_KEY_LEN} {ALPHABET} {NUM_ITER}\n", argv[0]);
    }

    return 0;
}


/* random_actions

Performs random operations on table oSymTable like:

1) Bindings with random keys/values are inserted from (keys, values)
2) The values of all bindings are changed.
3) Random keys are queried from the table.
4) Random bindings are deleted.

Parameters:
oSymTable: a SymTable_T type.
keys: array of character keys.
values: array of integer values.
num_keys: number of keys to create.

Returns: void */
void random_actions(SymTable_T oSymTable, char **keys, int num_keys, int *values) {
    int j, *bind_value;
    char *key;
    int pvValue = 2;    /* used to change the value of each binding */

    /* perform some actions on the table */
    printf("++> Inserting %d random keys...\n", num_keys);
    for (j = 0; j < num_keys; j++) {
        key = keys[rand() % num_keys];
        if (SymTable_put(oSymTable, key, &values[j])) {
            #if DEBUG
                printf("(%s : %d) inserted\n", key, values[j]);
            #endif
        }
        else {
            #if DEBUG
                printf("\'%s\' already exists\n", key);
            #endif
        }
    }
    printf("DONE\n");
    printf("++> Keys inserted: %d\n", SymTable_getLength(oSymTable));

    #if DEBUG
        printf("Table after insertion:\n");
        SymTable_map(oSymTable, print_bind, NULL);
    #endif
    
    SymTable_stats(oSymTable);

    printf("++> Transforming the values of bindings...");
    SymTable_map(oSymTable, update_bind, &pvValue);
    printf("DONE\n");

    #if DEBUG
        printf("Table after transform:\n");
        SymTable_map(oSymTable, print_bind, NULL);
    #endif

    printf("++> Searching for keys...\n");
    for (j = 0; j < num_keys; j++) {
        key = keys[rand() % num_keys];
        bind_value = SymTable_get(oSymTable, key);
        if (bind_value) {
            #if DEBUG
                print_bind(key, bind_value, NULL);
            #endif
        }
        else {
            #if DEBUG
                printf("\'%s\' not found\n", key);
            #endif
        }
    }
    printf("DONE\n");

    printf("++> Deleting %d random keys...\n", num_keys);
    for (j = 0; j < num_keys; j++) {
        key = keys[rand() % num_keys];
        if (SymTable_remove(oSymTable, key)) {
            #if DEBUG
                printf("\'%s\' deleted\n", key);
            #endif
        }
        else {
            #if DEBUG
                printf("\'%s\' NOT found\n", key);
            #endif
        }
    }
    printf("DONE\n");
    
    #if DEBUG
        printf("Table after deletion\n");
        SymTable_map(oSymTable, print_bind, NULL);
    #endif

    printf("++> #bindings remaining: %d\n", SymTable_getLength(oSymTable));
    return;
}


/* print_bind

Function used by SymTable_map() to print the
key and value of a binding.

Checks: if pvValue is not NULL at runtime.

Parameters:
pcKey: pointer to a character array (key). Must be null-terminated.
pvValue: pointer to a void value (treated as integer).
pvExtra: pointer to a void value. Ignored in this function.

Returns: void */
void print_bind(const char *pcKey, void *pvValue, void *pvExtra) {
    int *val;
    assert(pvValue);
    val = pvValue;
    printf("(%s : %d)\n", pcKey, *val);
    return;
}


/* update_bind

Function used by SymTable_map() for changing
the value of a binding. This particular function sets
the new value of a binding to pvValue + pvExtra.

Checks: if pvValue and pvExtra are not NULL at runtime.

Parameters:
pcKey: pointer to a character array (key). Ignored in this function.
pvValue: pointer to a void value (treated as integer).
pvExtra: pointer to a void value (treated as integer).

Returns: void*/
void update_bind(const char *pcKey, void *pvValue, void *pvExtra) {
    int *val, *val_extra;
    assert(pvValue);
    assert(pvExtra);
    val = pvValue;
    val_extra = pvExtra;
    *val += *val_extra;
    return;
}


/* random_keys

Creates an array of character arrays (keys). Each key has
at most max_key_len characters and is created by selecting random characters
from the character array alphabet.

Runtime checks:
1) if alphabet is not NULL
2) if length of alphabet is not 0
3) if number of keys is >=0
4) if maximum key length is >0
5) if memory was allocated succesfully for keys

Parameters:
alphabet: pointer to an array of characters. Must be null-terminated.
num_keys: the number of keys in the array.
max_key_len: the maximum number of characters in each key.

Returns: a pointer to an array of non-empty null-terminated keys. */
char** random_keys(char *alphabet, int num_keys, int max_key_len) {
    char **keys;
    int i, j, rand_int, alpha_length;
    
    assert(alphabet);
    assert(num_keys >= 0);
    assert(max_key_len > 0);
    alpha_length  = strlen(alphabet);
    assert(alpha_length);
    keys = malloc(num_keys * sizeof(char *));
    assert(keys);

    for (i = 0; i < num_keys; i++) {

        /* generate a random length for each key */
        rand_int = rand() % max_key_len + 1;
        keys[i] = malloc((rand_int + 1) * sizeof(char));

        /* fill the key with random characters from alphabet */
        for (j = 0; j < rand_int; j++) {
            keys[i][j] = alphabet[rand() % alpha_length];
        }
        keys[i][j] = '\0';
    }
    return keys;
}