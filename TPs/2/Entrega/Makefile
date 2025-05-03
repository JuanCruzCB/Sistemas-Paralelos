all: secuencial pthreads openmp

secuencial:
	gcc -O3 -Wall -o secuencial secuencial.c

pthreads:
	gcc -O3 -Wall -pthread -o pthreads pthreads.c

openmp:
	gcc -O3 -Wall -fopenmp -o openmp openmp.c

clean:
	rm -f secuencial pthreads openmp
