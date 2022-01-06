#!/bin/bash

# Launch 4 threads on a single node using Slurm. The result is 1 printout of
# the power usage from thread 0.
OMP_NUM_THREADS=4 srun -N 1 ./variorum-print-power-openmp-example

#
