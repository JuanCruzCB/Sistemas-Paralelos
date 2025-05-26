#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --partition=Blade
#SBATCH -o secuencial/output_N_?.txt
#SBATCH -e secuencial/errors_N_?.txt
./sec $1
