#!/bin/bash
#SBATCH -A SNIC2017-3-3
# Number of MPI tasks
#SBATCH -n 1
#
# Number of tasks per node
#SBATCH --tasks-per-node=1
#
#SBATCH --time=00:02:00

# Modules
module add openmpi/gcc/1.8.8
srun -n 4 ./validation.sh