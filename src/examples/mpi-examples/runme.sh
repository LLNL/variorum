#!/bin/bash

# Launch 8 tasks, 4 tasks per node using Slurm. The result is 1 printout per node of
# the power usage.
srun -N 2 -n 8 ./variorum-print-power-mpi-example

# Launch 8 tasks, 4 tasks per node using Slurm. Only the first rank on each
# node will set a cap of 100W.
srun -N 2 -n 8 ./variorum-cap-socket-power-limit-mpi-example -l 100

#
