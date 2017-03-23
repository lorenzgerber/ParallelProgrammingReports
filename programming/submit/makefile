# Makefile
# Lorenz Gerber
# Programming Assignment VT17
# Makefile for Game of Life
MPICC=mpicc
CFLAGS= -Wall 

all: mpi_life

mpi_life: mpi_life.o  
	$(MPICC) $(CFLAGS) -o mpi_life 

mpi_life.o: mpi_life.c mpi_life.h
	$(MPICC) $(CFLAGS) -c mpi_life.c

clean:
	rm -f rm *.o