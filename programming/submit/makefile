# Makefile
# Lorenz Gerber
# Programming Assignment VT17
# Makefile for Game of Life
MPICC=mpicc
CC=gcc
CFLAGS= -Wall 

all: mpi_life serial_life

mpi_life: mpi_life.c 
	$(MPICC) $(CFLAGS) -o mpi_life mpi_life.c

mpi_life.o: mpi_life.c mpi_life.h
	$(MPICC) $(CFLAGS) -c mpi_life.c

serial_life: serial_life.c
	$(CC) $(CFLAGS) -o serial_life serial_life.c

serial_life.o: serial_life.c serial_life.h
	$(CC) $(CFLAGS) -c serial_life.c

clean:
	rm -f rm *.o rm *.txt rm *.pdf
