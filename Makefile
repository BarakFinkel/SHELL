.PHONY: clean, all

all: cmp copy

cmp: cmp.o
	gcc -Wall -o cmp cmp.o

copy: copy.o
	gcc -Wall -o copy copy.o

cmp.o: cmp.c
	gcc -Wall -c cmp.c

copy.o: copy.c
	gcc -Wall -c copy.c

clean:
	rm -f *.o cmp copy