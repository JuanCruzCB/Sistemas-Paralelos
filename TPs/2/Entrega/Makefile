all: secuencial pthreads openmp

secuencial:
	gcc -O3 -Wall -o sec secuencial.c

pthreads:
	gcc -O3 -Wall -pthread -o pth pthreads.c

openmp:
	gcc -O3 -Wall -fopenmp -o open openmp.c

clean:
	rm -f sec pth open
