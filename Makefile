CFLAGS = -c -Wall -ansi -pedantic

hash: testsymtab.o symtablehash.o
	gcc testsymtab.o symtablehash.o -o hash

testsymtab.o: testsymtab.c symtable.h
	gcc $(CFLAGS) testsymtab.c

symtablehash.o: symtablehash.c symtable.h
	gcc $(CFLAGS) symtablehash.c

clean:
	rm -f *.o hash