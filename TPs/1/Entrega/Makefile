all: opt no_opt

opt: ejercicio2-optimizado.c
	gcc -O3 -Wall -o optimizada ejercicio2-optimizado.c

no_opt: ejercicio2-sin-optimizar.c
	gcc -O3 -Wall -o no_optimizada ejercicio2-sin-optimizar.c

clean:
	rm -f optimizada no_optimizada
