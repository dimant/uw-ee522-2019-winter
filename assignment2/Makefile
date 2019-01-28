CC=gcc
CFLAGS=-I. -O -Wswitch -Wno-deprecated-declarations -Wempty-body -Wreturn-type -Wparentheses -Wpointer-sign -Wformat -Wuninitialized -Wunreachable-code -Wunused-function -Wunused-value -Wunused-variable -Wconversion -Wall -fstrict-aliasing -std=c11 -pedantic -Wshadow -Wcast-qual -Wwrite-strings -Werror 

LIBS = -lwiringPi -lbcm2835 -lpigpio -lrt

SRC=$(wildcard *.c)

assignment: $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o assignment
