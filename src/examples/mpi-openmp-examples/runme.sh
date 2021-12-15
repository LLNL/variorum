#!/bin/bash

# Launch 8 tasks, 4 per node using Slurm. The result is 1 printout per node of
# the power usage.
srun -N 2 -n 8 ./variorum-print-power-mpi-example

#
