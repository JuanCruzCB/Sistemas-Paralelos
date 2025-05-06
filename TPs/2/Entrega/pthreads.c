#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>
#include "utils.h"
#include <semaphore.h>

#define EPSILON 1e-6

/* VARIABLES GLOBALES */

double * A, * B, * B_T, * C, * R, * a_por_b, * c_por_bt;
int N, cantidad_hilos;                      // Tamaño de las matrices y cantidad de hilos a crear.
double cociente = 0;                        // Variable auxiliar que almacenará el resultado de la primer parte de la ecuación (la división).
int TAM_BLOQUE = 128;                       // Tamaño del bloque para la multiplicación de matrices.

double max_A = -1.0;					    // Valor máximo de la matriz A.
double max_B = -1.0;					    // Valor máximo de la matriz B.
double min_A = 999.0;				        // Valor mínimo de la matriz A.
double min_B = 999.0;				        // Valor mínimo de la matriz B.
double prom_A = 0.0;					    // Valor promedio de la matriz A.
double prom_B = 0.0;					    // Valor promedio de la matriz B.
double suma_A = 0.0;					    // Acumulador de los valores de la matriz A.
double suma_B = 0.0;				        // Acumulador de los valores de la matriz B.

pthread_barrier_t barrier[2];
sem_t semA, semB;

/* VARIABLES GLOBALES */

void multiplicar_bloque(double * primer_bloque, double * segundo_bloque, double * bloque_resultado) {
    int i, j, k;

    for (i = 0; i < TAM_BLOQUE; i++) {
        for (j = 0; j < TAM_BLOQUE; j++) {
            double aux = 0.0;
            for (k = 0; k < TAM_BLOQUE; k++) {
                aux += primer_bloque[i * N + k] * segundo_bloque[j * N + k];
            }
            bloque_resultado[i * N + j] += aux;
        }
    }
}

void * computo_general(void * ptr) {
    int id = * (int *) ptr;

    int porcion = N / cantidad_hilos;
    int inicio = id * porcion;
    int fin = inicio + porcion;

    double max_local_A = -1.0;
    double min_local_A = 999.0;
    double suma_local_A = 0.0;
    double celda_A = 0.0;

    double max_local_B = -1.0;
    double min_local_B = 999.0;
    double suma_local_B = 0.0;
    double celda_B = 0.0;

    int i, j, k;

    // Inicializar la matriz B_T (B transpuesta).
    for (i = inicio; i < fin; i++) {
        for (j = 0; j < N; j++) {
            B_T[j * N + i] = B[i * N + j];
        }
    }

    // Calcular el valor máximo, mínimo y promedio de la matriz A.
    for (i = inicio; i < fin; i++) {
        for (j = 0; j < N; j++) {
            celda_A = A[i * N + j];
            suma_local_A += celda_A;
            if (celda_A > max_local_A) max_local_A = celda_A;
            if (celda_A < min_local_A) min_local_A = celda_A;
        }
    }

    // Calcular el valor máximo, mínimo y promedio de la matriz B.
    for (i = inicio; i < fin; i++) {
        for (j = 0; j < N; j++) {
            celda_B = B[i * N + j];
            suma_local_B += celda_B;
            if (celda_B > max_local_B) max_local_B = celda_B;
            if (celda_B < min_local_B) min_local_B = celda_B;
        }
    }

    // Actualizar variables globales con exclusión mutua.
    sem_wait(&semA);
    suma_A += suma_local_A;
    if (max_local_A > max_A) max_A = max_local_A;
    if (min_local_A < min_A) min_A = min_local_A;
    sem_post(&semA);

    sem_wait(&semB);
    suma_B += suma_local_B;
    if (max_local_B > max_B) max_B = max_local_B;
    if (min_local_B < min_B) min_B = min_local_B;
    sem_post(&semB);

    // Barrera 1.
    pthread_barrier_wait(&barrier[0]);

    if (id == 0) {
        prom_A = suma_A / (N * N);
        prom_B = suma_B / (N * N);
        cociente = (max_A * max_B - min_A * min_B) / (prom_A * prom_B);
    }

    // Resolver [A×B] y guardarlo en una matriz auxiliar a_por_b.
    for (i = inicio; i < fin; i += TAM_BLOQUE) {
        for (j = 0; j < N; j += TAM_BLOQUE) {
            for (k = 0; k < N; k += TAM_BLOQUE) {
                multiplicar_bloque(
                    &A[i * N + k],
                    &B[j * N + k],
                    &a_por_b[i * N + j]
                );
            }
        }
    }

    // Resolver [C×B_T] y guardarlo en una matriz auxiliar c_por_bt.
    for (i = inicio; i < fin; i += TAM_BLOQUE) {
        for (j = 0; j < N; j += TAM_BLOQUE) {
            for (k = 0; k < N; k += TAM_BLOQUE) {
                multiplicar_bloque(
                    &C[i * N + k],
                    &B_T[j * N + k],
                    &c_por_bt[i * N + j]
                );
            }
        }
    }

    // Barrera 2.
    pthread_barrier_wait(&barrier[1]);

    // Finalmente multiplicar la matriz auxiliar a_por_b por cociente y sumarle a eso la matriz c_por_bt.
    for (i = inicio; i < fin; i++) {
        for (j = 0; j < N; j++) {
            R[i * N + j] = (a_por_b[i * N + j] * cociente) + (c_por_bt[i * N + j]);
        }
    }

    pthread_exit(0);
}

double dwalltime() {
    double sec;
    struct timeval tv;
    gettimeofday( & tv, NULL);
    sec = tv.tv_sec + tv.tv_usec / 1000000.0;
    return sec;
}

int main(int argc, char * argv[]) {
    int i, j; 						              // Índices para recorrer las matrices → i para fila; j para columna.
    double acumulador = 1.0;            // Acumulador para los valores de la matriz B.
    double timetick; 					          // Se usa para medir el tiempo.

    // Se debe enviar el N y la cantidad de hilos como argumento. Si no se envía, alertar y terminar.
    if (
        (argc != 3) ||
        ((N = atoi(argv[1])) <= 0) ||
        ((cantidad_hilos = atoi(argv[2])) <= 0) ||
        (cantidad_hilos > N) ||
        (N % cantidad_hilos != 0) ||
        (N % TAM_BLOQUE != 0)
    ) {
        printf("\nSe deben enviar 2 parámetros:\n");
        printf("El N de la dimensión de las matrices y la cantidad de hilos.\n");
        printf("Ejemplo con N = 512 y 4 hilos: \n ./programa 512 4\n");
        exit(1);
    }

    // Ajustar el tamaño de bloque si la porción que le corresponde a cada hilo es menor que el tamaño de bloque.
    if (N / cantidad_hilos < TAM_BLOQUE) {
        TAM_BLOQUE = N / cantidad_hilos;
    }

    // Alocar memoria para las cuatro matrices principales y las dos auxiliares.
    A = (double * ) malloc(N * N * sizeof(double));
    B = (double * ) malloc(N * N * sizeof(double));
    B_T = (double * ) malloc(N * N * sizeof(double));
    C = (double * ) malloc(N * N * sizeof(double));
    a_por_b = (double * ) malloc(N * N * sizeof(double));
    c_por_bt = (double * ) malloc(N * N * sizeof(double));
    R = (double * ) malloc(N * N * sizeof(double));

    // Inicializar las cuatro matrices principales y las dos auxiliares.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (i == j) {
                A[i * N + j] = 1.0;
                C[i * N + j] = 1.0;
            }
            else {
                A[i * N + j] = 0.0;
                C[i * N + j] = 0.0;
            }

            B[j * N + i] = acumulador;
            R[i * N + j] = 0.0;
            a_por_b[i * N + j] = 0.0;
            c_por_bt[i * N + j] = 0.0;

            acumulador += 1.0;
        }
    }

    // Inicializaciones de Threading
    int ids[cantidad_hilos];

    sem_init(&semA, 0, 1);
    sem_init(&semB, 0, 1);

    pthread_barrier_init(&barrier[0], NULL, cantidad_hilos);
    pthread_barrier_init(&barrier[1], NULL, cantidad_hilos);

    pthread_t threads[cantidad_hilos];

    // Comenzar a medir el tiempo.
    timetick = dwalltime();

    for (i = 0; i < cantidad_hilos; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, computo_general, &ids[i]);
    }

    for (i = 0; i < cantidad_hilos; i++) {
        pthread_join(threads[i], NULL);
    }

    // Terminar de medir el tiempo e imprimirlo.
    printf("Tiempo que llevó computar la ecuación con N = %d, tamaño de bloque = %d, cantidad de hilos = %d ---> %f.\n\n", N, TAM_BLOQUE, cantidad_hilos, dwalltime() - timetick);

    chequear_resultados(A, B, B_T, C, R, a_por_b, c_por_bt, N, cociente);

    // Liberar la memoria que alocamos a las matrices al inicio.
    free(A);
    free(B);
    free(B_T);
    free(C);
    free(R);
    free(a_por_b);
    free(c_por_bt);

    return (0);
}
