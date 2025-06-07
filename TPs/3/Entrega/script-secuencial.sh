#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --partition=Blade
#SBATCH -o secuencial_resultados/output_N_?.txt
#SBATCH -e secuencial_resultados/errors_N_?.txt
./secuencial $1
