# Makefile
# Lorenz Gerber
# Programming Assignment VT17
# Makefile for Game of Life
MPICC=mpicc
CC=gcc
CFLAGS= -Wall 

all: mpi_life

mpi_life: mpi_life.c 
	$(MPICC) $(CFLAGS) -o mpi_life mpi_life.c
clean:
	rm -f rm *.o
