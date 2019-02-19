CC=gcc
CFLAGS=-I. -O -Wswitch -Wno-deprecated-declarations -Wempty-body -Wreturn-type -Wparentheses -Wpointer-sign -Wformat -Wuninitialized -Wunreachable-code -Wunused-function -Wunused-value -Wunused-variable -Wconversion -Wall -fstrict-aliasing -std=c11 -pedantic -Wshadow -Wcast-qual -Wwrite-strings -Werror -g

LIBS = -lm -lasound

SRC=$(wildcard *.c)

assignment: $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o assignment
