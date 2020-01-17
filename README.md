# Description

Library for creating and using [Symbol tables](https://en.wikipedia.org/wiki/Symbol_table).

The following functions are provided:

* SymTable_new(): Create a new table.
* Symtable_free(table): Delete table and free memory.
* SymTable_getLength(table): Get the total number of (keys, values) in the table.
* SymTable_put(table, key, value): Put (key, value) in the table.
* SymTable_remove(table, key): Delete key from table and free memory.
* SymTable_contains(table, key): Check whether table has key.
* SymTable_get(table, key): Get the value associated with key.
* SymTable_map(table, function, new_value): Apply function to each (key, value) in the table.

## Implementation

The tables are internally stored as [Hash arrays](https://en.wikipedia.org/wiki/Hash_table) with
[linked lists](https://en.wikipedia.org/wiki/Linked_list) for resolving conflicts. Operations
like 'get', 'put', 'remove', 'contains' should run in O(1) time.

For a less efficient implementation using only linked lists, see
[symtable-lists](https://bitbucket.org/txatzial/symbol-table-lists).

## Compile

The program is written in C. To compile, use the provided Makefile.

### Building the library

```bash
make symtablehash.o
```

### Building the test files

Using the library is demonstrated in [testsymtab.c](testsymtab.c). Build the tests with:

```bash
make hash
```

Running the tests is explained in the next section.

## Typical usage

The predefined tests in [testsymtab.c](testsymtab.c) create tables and insert random (keys, values). The following are always true:

1. Values are always integers > 0.
2. Changing a value simply means adding 2 to it.

By default all operations
are performed on one table. This can be alterned by changing the NTABLES constant.
There is also the option to show all intermediate results, to do so, change the DEBUG constant.

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
