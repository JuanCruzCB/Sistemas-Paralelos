#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --partition=Blade
#SBATCH -o pthreads/output_N_?_T_?.txt
#SBATCH -e pthreads/errors_N_?_T_?.txt
./pth $1 $2
