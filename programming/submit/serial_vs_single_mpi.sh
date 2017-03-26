#!/bin/bash
#SBATCH -A SNIC2017-3-3
# Number of MPI tasks
#SBATCH -n 1
#
# Number of tasks per node
#SBATCH --tasks-per-node=1
#
#SBATCH --time=00:10:00

# Modules
module add openmpi/gcc/1.8.8
echo "serial triplicate 1024 1000"
./serial_life 1024 1000 out.txt
./serial_life 1024 1000 out.txt
./serial_life 1024 1000 out.txt
echo "mpi triplicate 1024 1000"
srun -n 1 ./mpi_life 1024 1000 out.txt
srun -n 1 ./mpi_life 1024 1000 out.txt
srun -n 1 ./mpi_life 1024 1000 out.txt