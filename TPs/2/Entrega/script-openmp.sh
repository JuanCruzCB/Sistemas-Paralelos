#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --partition=Blade
#SBATCH -o openmp/output_N_?_T_?.txt
#SBATCH -e openmp/errors_N_?_T_?.txt
./open $1 $2
