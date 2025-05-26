#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=8
#SBATCH -o hibrido/output_N_?_T_?.txt
#SBATCH -e hibrido/errors_N_?_T_?.txt
export OMP_NUM_THREADS=4
mpirun --bind-to none hibrido $1