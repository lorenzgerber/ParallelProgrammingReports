#!/bin/bash
for i in `seq 1 3`;
do
    ./nbody_basic 500 1000 0.05 1 g
done

for i in `seq 1 3`;
do
    ./nbody_basic 750 1000 0.05 1 g
done

for i in `seq 1 3`;
do
    ./nbody_basic 1000 1000 0.05 1 g
done

for i in `seq 1 3`;
do
    ./nbody_basic 1250 1000 0.05 1 g
done

for i in `seq 1 3`;
do
    ./nbody_basic 1500 1000 0.05 1 g
done

for i in `seq 1 3`;
do
    ./nbody_basic 1750 1000 0.05 1 g
done

for i in `seq 1 3`;
do
    ./nbody_basic 2000 1000 0.05 1 g
done
