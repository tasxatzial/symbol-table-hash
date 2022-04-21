# Symbol tables library

Library for creating and using [Symbol tables](https://en.wikipedia.org/wiki/Symbol_table) (hash table based implementation).

The following functions are provided:

* SymTable_new(): Create a new table.
* Symtable_free(table): Delete table. No other functions should be used after this one.
* SymTable_getLength(table): Get the total number of keys.
* SymTable_put(table, key, value): Put (key, value) in the table.
* SymTable_remove(table, key): Delete key from table.
* SymTable_contains(table, key): Check whether table has key.
* SymTable_get(table, key): Get the value associated with key.
* SymTable_map(table, function(key, new_value, extra_value), new_value): Apply a function to each value.
* SymTable_stats(table): Print basic information about the table.

## Implementation

The library provides an [opaque data type](https://en.wikipedia.org/wiki/Opaque_data_type) for the table that can be used with the above functions. The representation of the type is completely hidden from its users. C-strings are supported as keys but values can be of any type, therefore the table is used to store only pointers to values.

The tables are internally stored as arrays. Each element of the array is a linked list, this helps resolve conflicts arising from different keys having the same hash value. Operations like 'get', 'put', 'remove', 'contains' run in O(1) time.

For a less efficient implementation using only Linked lists, see [symbol-table-lists](https://github.com/tasxatzial/symbol-table-lists).

## Profiling

The program has been tested for memory leaks with [valgrind](https://valgrind.org/) and [AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer).

## Compile

Build the library:

```bash
make symtablehash.o
```

## Demo

Using the library is demonstrated in [runsymtab.c](src/runsymtab.c).

Build:

```bash
make hash
```

The predefined demo creates tables and inserts random (key, value) pairs. More specifically:

1. Values are always integers > 0.
2. Changing a value simply means adding 2 to it.

By default all operations are performed on one table. This can be alterned by changing the NTABLES constant. There is also the option to show all intermediate results by changing the DEBUG constant to 1.

### Example

To create a table and perform the following operations 2 times:

1) Insert 10 random (keys, values) with keys having 5 characters max from the alphabet 'abcde'.

2) Change the values of all keys.

3) Search for 10 random keys.

4) Delete 10 random keys.

Run:

```bash
./hash 10 5 abcde 2
```
