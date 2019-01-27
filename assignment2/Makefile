CC=gcc
CFLAGS=-I. -O
DEPS = bcm2835-experiments.h export-pin.h memgpio-experiments.h pigpio-experiments.h wiringpi-experiments.h arith-benchmarks.h

OBJ = main.o bcm2835-experiments.o export-pin.o memgpio-experiments.o pigpio-experiments.o wiringpi-experiments.o arith-benchmarks.o

LIBS = -lwiringPi -lbcm2835 -lpigpio -lrt

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

assignment: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
