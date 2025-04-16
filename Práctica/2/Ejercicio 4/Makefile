all: secuencial paralelo

secuencial: secuencial.c
	gcc -Wall -o secuencial secuencial.c

paralelo: paralelo.c
	gcc -Wall -pthread -o paralelo paralelo.c

clean:
	rm -f secuencial paralelo
