#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>
#include "utils.h"

#define EPSILON 1e-6
#define TAM_BLOQUE 128

double dwalltime() {
    double sec;
    struct timeval tv;
    gettimeofday( & tv, NULL);
    sec = tv.tv_sec + tv.tv_usec / 1000000.0;
    return sec;
}

void multiplicar_bloque(double * primer_bloque, double * segundo_bloque, double * bloque_resultado, int n) {
    int i, j, k;

    for (i = 0; i < TAM_BLOQUE; i++) {
        for (j = 0; j < TAM_BLOQUE; j++) {
            double aux = 0.0;
            for (k = 0; k < TAM_BLOQUE; k++) {
                aux += primer_bloque[i * n + k] * segundo_bloque[j * n + k];
            }
            bloque_resultado[i * n + j] += aux;
        }
    }
}

int main(int argc, char * argv[]) {
    /* ARGUMENTOS */
    int N = -1; 						// Tamaño de las matrices cuadradas (N×N).

    /* MATRICES */
    double * A, * B, * B_T, * C, * R, * a_por_b, * c_por_bt;   // Matrices A, B, B transpuesta, C, R, A×B, C×B_T.
    int i, j, k; 						// Índices para recorrer las matrices → i para fila; j para columna.
    double acumulador = 1.0;            // Acumulador para los valores de la matriz B.
    double cociente = 0; 				// Variable auxiliar que almacenará el resultado de la primer parte de la ecuación (la división).

    /* MÍNIMO, MÁXIMO, PROMEDIO */
    double max_A = -1.0;					// Valor máximo de la matriz A.
    double max_B = -1.0;					// Valor máximo de la matriz B.
    double min_A = 999.0;				// Valor mínimo de la matriz A.
    double min_B = 999.0;				// Valor mínimo de la matriz B.
    double prom_A = 0.0;					// Valor promedio de la matriz A.
    double prom_B = 0.0;					// Valor promedio de la matriz B.
    double suma_A = 0.0;					// Acumulador de los valores de la matriz A.
    double suma_B = 0.0;					// Acumulador de los valores de la matriz B.
    double celda_A = 0.0;				// Celda temporal de la matriz A.
    double celda_B = 0.0;				// Celda temporal de la matriz B.

    double timetick; 					// Se usa para medir el tiempo.

    // Se debe enviar el N y la cantidad de hilos como argumento. Si no se envía, alertar y terminar.
    if (
        (argc != 3) ||
        ((N = atoi(argv[1])) <= 0) ||
        (N % TAM_BLOQUE != 0)
    ) {
        printf("\nSe deben enviar 2 parámetros:\n");
        printf("El N de la dimensión de las matrices y la cantidad de hilos.\n");
        printf("Ejemplo con N = 512 y 4 hilos: \n ./programa 512 4\n");
        exit(1);
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
            if(i == j) {
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

    // Imprimir las matrices si N es chico.
  	if (N <= 4) {
        printf("Matriz A (ordenada por filas e inicializada como matriz identidad):\n");
        imprimir_matriz_por_fila(A, N);
        printf("Matriz B (ordenada por columnas e inicializada de forma incremental):\n");
        imprimir_matriz_por_columna(B, N);
        printf("Matriz C (ordenada por filas e inicializada como matriz identidad):\n");
        imprimir_matriz_por_fila(C, N);
    }

    // Comenzar a medir el tiempo.
    timetick = dwalltime();

    // Inicializar la matriz B_T (B transpuesta).
    for(i = 0; i < N ; i++)
        for(j = 0; j < N ; j++) B_T[j * N + i] = B[i * N + j];

    // Calcular el valor máximo, mínimo y promedio de la matriz A.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            celda_A = A[i * N + j];
            suma_A += celda_A;
            if (celda_A > max_A) max_A = celda_A;
            if (celda_A < min_A) min_A = celda_A;
        }
    }

    // Calcular el valor máximo, mínimo y promedio de la matriz B.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            celda_B = B[i * N + j];
            suma_B += celda_B;
            if (celda_B > max_B) max_B = celda_B;
            if (celda_B < min_B) min_B = celda_B;
        }
    }

    prom_A = suma_A / (N * N);
    prom_B = suma_B / (N * N);
    cociente = ((max_A * max_B) - (min_A * min_B)) / (prom_A * prom_B);

    // Resolver [A×B] y guardarlo en una matriz auxiliar a_por_b.
    for (i = 0; i < N; i += TAM_BLOQUE) {
        for (j = 0; j < N; j += TAM_BLOQUE) {
            for (k = 0; k < N; k += TAM_BLOQUE) {
                multiplicar_bloque( & A[i * N + k], & B[j * N + k], & a_por_b[i * N + j], N);
            }
        }
    }

    // Resolver [C×B_T] y guardarlo en una matriz auxiliar c_por_bt.
    for (i = 0; i < N; i += TAM_BLOQUE) {
        for (j = 0; j < N; j += TAM_BLOQUE) {
            for (k = 0; k < N; k += TAM_BLOQUE) {
                multiplicar_bloque( & C[i * N + k], & B_T[j * N + k], & c_por_bt[i * N + j], N);
            }
        }
    }

    // Finalmente multiplicar la matriz auxiliar a_por_b por cociente y sumarle a eso la matriz c_por_bt.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            R[i * N + j] = (a_por_b[i * N + j] * cociente) + (c_por_bt[i * N + j]);
        }
    }

    // Terminar de medir el tiempo e imprimirlo.
    printf("Tiempo que llevó computar la ecuación con N = %d y tamaño de bloque = %d ---> %f.\n\n", N, TAM_BLOQUE, dwalltime() - timetick);

    chequear_resultados(A, B, B_T, C, R, a_por_b, c_por_bt, N, cociente);

    // Imprimir los resultados si N es chico.
  	if (N <= 4) {
        printf("Matriz A×B:\n");
        imprimir_matriz_por_fila(a_por_b, N);
        printf("Matriz C×B_T:\n");
        imprimir_matriz_por_fila(c_por_bt, N);
        printf("Matriz R:\n");
        imprimir_matriz_por_fila(R, N);
        printf("Valor máximo de A: %f\n", max_A);
        printf("Valor máximo de B: %f\n", max_B);

        printf("Valor mínimo de A: %f\n", min_A);
        printf("Valor mínimo de B: %f\n", min_B);

        printf("Promedio de A: %f\n", prom_A);
        printf("Promedio de B: %f\n", prom_B);

        printf("Cociente: %f\n", cociente);
    }

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
