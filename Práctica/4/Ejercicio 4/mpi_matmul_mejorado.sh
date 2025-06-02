#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --tasks-per-node=8
#SBATCH -o ./output_N_?.txt
#SBATCH -e ./errors_N_?.txt
mpirun mpi_matmul_mejorado $1