#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --partition=Blade
#SBATCH -o ejercicio1c/outputTIMES600.txt
#SBATCH -e ejercicio1c/errorsTIMES600.txt
./quadatric3TIMES600
