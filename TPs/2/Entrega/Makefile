all: secuencial pthreads openmp

secuencial:
	gcc -O3 -Wall -o secuencial secuencial-optimizado.c

pthreads:
	gcc -O3 -Wall -pthread -o pthreads paralelo-pthreads.c

openmp:
	gcc -O3 -Wall -fopenmp -o openmp paralelo-openmp.c

clean:
	rm -f secuencial pthreads openmp
