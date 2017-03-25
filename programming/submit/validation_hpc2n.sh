#!/bin/bash
#SBATCH -A SNIC2017-3-3
# Number of MPI tasks
#SBATCH -n 1
#
# Number of tasks per node
#SBATCH --tasks-per-node=1
#
#SBATCH --time=00:03:00

# Modules
module add openmpi/gcc/1.8.8

echo "Generating random 1024 x 1024 game field"
./serial_life 1024 1 test.txt

echo "running serial implementation on random data (100 generations)..."
./serial_life 1024 100 out_serial.txt test.txt

echo "running mpi implementation using 4 processes on random data (100 generations)..."
srun -n 4 ./mpi_life 1024 100 out_mpi.txt test.txt

echo "comparing the resulting datasets..."
if [[ $(diff out_mpi.txt out_serial.txt) ]]; then
    echo "The output is not the same, validation failed"
    Rscript --vanilla plot_script.R out_serial.txt out_serial.pdf &>/dev/null
    Rscript --vanilla plot_script.R out_mpi.txt out_mpi.pdf &>/dev/null
else
    echo "No output from 'diff', validation succeeded"
    Rscript --vanilla plot_script.R out_serial.txt validated.pdf
fi


