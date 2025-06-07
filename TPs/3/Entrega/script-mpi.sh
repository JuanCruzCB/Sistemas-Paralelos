#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --tasks-per-node=8
#SBATCH -o mpi_resultados/output_N_?.txt
#SBATCH -e mpi_resultados/errors_N_?.txt
mpirun mpi $1