#!/bin/bash
#SBATCH -N 2
#SBATCH --exclusive
#SBATCH --tasks-per-node=1
#SBATCH -o hibrido_resultados/output_N_?.txt
#SBATCH -e hibrido_resultados/errors_N_?.txt
export OMP_NUM_THREADS=8
mpirun --bind-to none hibrido $1