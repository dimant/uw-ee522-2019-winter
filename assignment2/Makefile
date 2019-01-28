CC=gcc
CFLAGS=-I. -O

LIBS = -lwiringPi -lbcm2835 -lpigpio -lrt

SRC=$(wildcard *.c)

assignment: $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
