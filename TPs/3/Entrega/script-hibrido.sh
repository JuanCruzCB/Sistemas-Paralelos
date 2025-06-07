#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=8
#SBATCH -o hibrido_resultados/output_N_?_T_?.txt
#SBATCH -e hibrido_resultados/errors_N_?_T_?.txt
export OMP_NUM_THREADS=4
mpirun --bind-to none hibrido $1