#!/bin/bash
echo "Generating random 1024 x 1024 game field"
./serial_life 1024 1 test.txt &>/dev/null

echo "running serial implementation on random data (1000 generations)..."
./serial_life 1024 1000 out_serial.txt test.txt &>/dev/null

echo "running mpi implementation using 4 processes on random data (1000 generations)..."
mpiexec -n 4 ./mpi_life 1024 1000 out_mpi.txt test.txt &>/dev/null

echo "comparing the resulting datasets..."
if [[ $(diff out_mpi.txt out_serial.txt) ]]; then
    echo "The output is not the same, validation failed"
    Rscript --vanilla plot_script.R out_serial.txt out_serial.pdf &>/dev/null &>
    Rscript --vanilla plot_script.R out_mpi.txt out_mpi.pdf &>/dev/null
else
    echo "No output from 'diff', validation succeeded"
    Rscript --vanilla plot_script.R out_serial.txt validated.pdf &>/dev/null
fi


