#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --partition=Blade
#SBATCH -o output-optimizado4096-128.txt
#SBATCH -e errors-optimizado4096-128.txt
./ejercicio2-optimizado $1 $2


