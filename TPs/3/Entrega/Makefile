all: secuencial mpi hibrido

secuencial: secuencial.c
	gcc -O3 -Wall -o secuencial secuencial.c

mpi: mpi.c
	mpicc -O3 -Wall -o mpi mpi.c

hibrido: hibrido.c
	mpicc -O3 -Wall -fopenmp -o hibrido hibrido.c

clean:
	rm -f secuencial mpi hibrido
