all: secuencial paralelo

secuencial: secuencial.c
	gcc -Wall -o secuencial secuencial.c -lm

paralelo: paralelo.c
	gcc -Wall -fopenmp -o paralelo paralelo.c -lm

clean:
	rm -f secuencial paralelo
