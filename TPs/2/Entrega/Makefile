all: secuencial pthreads openmp

secuencial:
	gcc -O3 -Wall -o sec secuencial.c utils.c

pthreads:
	gcc -O3 -Wall -pthread -o pth pthreads.c utils.c

openmp:
	gcc -O3 -Wall -fopenmp -o open openmp.c utils.c

clean:
	rm -f sec pth open
