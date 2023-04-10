# Symbol tables library

A simple library for [Symbol tables](https://en.wikipedia.org/wiki/Symbol_table). The implementation is based on **hash tables**.

The following functions are provided:

* SymTable_new(): Create a new table.
* Symtable_free(table): Delete table. No other functions should be used after this one.
* SymTable_getLength(table): Get the total number of keys.
* SymTable_put(table, key, value): Put (key, value) in the table only if key does not exist.
* SymTable_remove(table, key): Delete key from table.
* SymTable_contains(table, key): Check whether table has key.
* SymTable_get(table, key): Get the value associated with key.
* SymTable_map(table, function(key, new_value, extra_value), new_value): Apply a function to each value.
* SymTable_stats(table): Print basic information about the table.

## Implementation

The symbol table is defined as an [opaque data type](https://en.wikipedia.org/wiki/Opaque_data_type).

C-strings are supported as keys and are stored directly in the table. Values can be of any type, therefore they should already be stored in a different data structure.

Internally the symbol tables are stored as arrays. Each element of the array is a linked list, this helps resolve conflicts arising from different keys having the same hash value. Operations like 'get', 'put', 'remove', 'contains' run in O(1) time.

For a less efficient implementation using Linked lists, see [symbol-table-lists](https://github.com/tasxatzial/symbol-table-lists).

## Compile

Build the library (functions declared in [symtable.h](src/symtable.h)):

```bash
make symtablehash.o
```

## Demo

Using the library is demonstrated in [runsymtab.c](src/runsymtab.c).

Build:

```bash
make hash
```

The demo creates tables and inserts random (key, value) pairs. More specifically:

1. Values are always integers > 0.
2. Changing a value means adding 2 to it.

By default all operations are performed on one table. This can be altered by changing the NTABLES constant. There is also the option to show all intermediate results by changing the DEBUG constant to 1.

### Example

```bash
./hash 10 5 abcde 2
```

will perform the following sequence of operations 2 times:

1. Insert 10 random (keys, values) with keys having 5 characters max from the alphabet 'abcde'.
2. Change the values of all keys.
3. Search for 10 random keys.
4. Delete 10 random keys.

## Profiling

'hash' has been tested for memory leaks with [valgrind](https://valgrind.org/) and [AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer).
