#!/bin/bash
#SBATCH -A SNIC2017-3-3
# Number of MPI tasks
#SBATCH -n 16
#
# Number of tasks per node
#
#SBATCH --time=01:30:00

# Modules
module add openmpi/gcc/1.8.8

srun -n 8 ./mpi_life 1024 100 out.txt
srun -n 8 ./mpi_life 1024 100 out.txt
srun -n 8 ./mpi_life 1024 100 out.txt
srun -n 16 ./mpi_life 1024 100 out.txt
srun -n 16 ./mpi_life 1024 100 out.txt
srun -n 16 ./mpi_life 1024 100 out.txt

srun -n 8 ./mpi_life 256 100 out.txt
srun -n 8 ./mpi_life 256 100 out.txt
srun -n 8 ./mpi_life 256 100 out.txt
srun -n 16 ./mpi_life 256 100 out.txt
srun -n 16 ./mpi_life 256 100 out.txt
srun -n 16 ./mpi_life 256 100 out.txt

srun -n 8 ./mpi_life 512 100 out.txt
srun -n 8 ./mpi_life 512 100 out.txt
srun -n 8 ./mpi_life 512 100 out.txt
srun -n 16 ./mpi_life 512 100 out.txt
srun -n 16 ./mpi_life 512 100 out.txt
srun -n 16 ./mpi_life 512 100 out.txt

srun -n 8 ./mpi_life 2048 100 out.txt
srun -n 8 ./mpi_life 2048 100 out.txt
srun -n 8 ./mpi_life 2048 100 out.txt
srun -n 16 ./mpi_life 2048 100 out.txt
srun -n 16 ./mpi_life 2048 100 out.txt
srun -n 16 ./mpi_life 2048 100 out.txt

srun -n 8 ./mpi_life 4096 100 out.txt
srun -n 8 ./mpi_life 4096 100 out.txt
srun -n 8 ./mpi_life 4096 100 out.txt
srun -n 16 ./mpi_life 4096 100 out.txt
srun -n 16 ./mpi_life 4096 100 out.txt
srun -n 16 ./mpi_life 4096 100 out.txt

srun -n 8 ./mpi_life 8192 100 out.txt
srun -n 8 ./mpi_life 8192 100 out.txt
srun -n 8 ./mpi_life 8192 100 out.txt
srun -n 16 ./mpi_life 8192 100 out.txt
srun -n 16 ./mpi_life 8192 100 out.txt
srun -n 16 ./mpi_life 8192 100 out.txt

