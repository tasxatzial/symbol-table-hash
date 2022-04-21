CFLAGS = -c -Wall -ansi -pedantic

hash: runsymtab.o symtablehash.o
	gcc runsymtab.o symtablehash.o -o hash

runsymtab.o: runsymtab.c symtable.h
	gcc $(CFLAGS) runsymtab.c

symtablehash.o: symtablehash.c symtable.h
	gcc $(CFLAGS) symtablehash.c

clean:
	rm -f *.o hash