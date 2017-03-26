#!/bin/bash
#SBATCH -A SNIC2017-3-3
# Number of MPI tasks
#SBATCH -n 3
#
# Number of tasks per node
#SBATCH --tasks-per-node=1
#
#SBATCH --time=00:10:00

# Modules
module add openmpi/gcc/1.8.8

srun -n 2 ./mpi_life 1024 100 out.txt
srun -n 2 ./mpi_life 1024 100 out.txt
srun -n 2 ./mpi_life 1024 100 out.txt
srun -n 4 ./mpi_life 1024 100 out.txt
srun -n 4 ./mpi_life 1024 100 out.txt
srun -n 4 ./mpi_life 1024 100 out.txt
srun -n 8 ./mpi_life 1024 100 out.txt
srun -n 8 ./mpi_life 1024 100 out.txt
srun -n 8 ./mpi_life 1024 100 out.txt
srun -n 16 ./mpi_life 1024 100 out.txt
srun -n 16 ./mpi_life 1024 100 out.txt
srun -n 16 ./mpi_life 1024 100 out.txt
