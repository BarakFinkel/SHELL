.PHONY: clean, all

# Make all:

all: cmp copy codecA codecB encode decode stshell


# Compiling & Linking:

cmp: cmp.c
	gcc -o cmp cmp.c

copy: copy.c
	gcc -o copy copy.c

encode: encode.c
	gcc -o encode encode.c -L. -lcodecA -lcodecB

decode: decode.c
	gcc -o decode decode.c -L. -lcodecA -lcodecB

codecA: codecA.o codecA.h
	gcc -c -fPIC codecA.c -o codecA.o
	gcc -shared -o libcodecA.so codecA.o

codecB: codecB.o codecB.h
	gcc -c -fPIC codecB.c -o codecB.o
	gcc -shared -o libcodecB.so codecB.o

stshell: stshell.c
	gcc -o stshell stshell.c

# Cleaning:

clean:
	rm -f *.o *.so cmp copy encode decode stshell