#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

void imprimir_matriz(double * A, int N) {
    for (int i = 0; i < N * N; i++) {
        printf("%.2f ", A[i]);
    }
    printf("\n\n");
}

double dwalltime() {
    double sec;
    struct timeval tv;
    gettimeofday( & tv, NULL);
    sec = tv.tv_sec + tv.tv_usec / 1000000.0;
    return sec;
}

void multiplicar_bloque(double * primer_bloque, double * segundo_bloque, double * bloque_resultado, int n, int block_size) {
    int i, j, k;

    for (i = 0; i < block_size; i++) {
        for (j = 0; j < block_size; j++) {
            double aux = 0.0;
            for (k = 0; k < block_size; k++) {
                aux += primer_bloque[i * n + k] * segundo_bloque[j * n + k];
            }
            bloque_resultado[i * n + j] += aux;
        }
    }
}

int main(int argc, char * argv[]) {
    /* ARGUMENTOS */
    int N = -1; 						// Tamaño de las matrices cuadradas (N×N).
    int block_size = -1;                // Tamaño del bloque para la multiplicación por bloques.
    int imprimir_matrices = -1;         // Indica si se deben imprimir las matrices o no.

    /* MATRICES */
    double * A, * B, * B_T, * C, * R, * a_por_b, * c_por_bt;   // Matrices A, B, B transpuesta, C, R, A×B, C×B_T.
    int i, j, k; 						// Índices para recorrer las matrices → i para fila; j para columna.
    double valorInicial = 1.0;          // Valor inicial de la matriz B.
    double cociente = 0; 				// Variable auxiliar que almacenará el resultado de la primer parte de la ecuación (la división).

    /* MÍNIMO, MÁXIMO, PROMEDIO */
    double maxA = -1.0;					// Valor máximo de la matriz A.
    double maxB = -1.0;					// Valor máximo de la matriz B.
    double minA = 999.0;				// Valor mínimo de la matriz A.
    double minB = 999.0;				// Valor mínimo de la matriz B.
    double promA = 0.0;					// Valor promedio de la matriz A.
    double promB = 0.0;					// Valor promedio de la matriz B.
    double sumaA = 0.0;					// Acumulador de los valores de la matriz A.
    double sumaB = 0.0;					// Acumulador de los valores de la matriz B.
    double celdaA = 0.0;				// Celda temporal de la matriz A.
    double celdaB = 0.0;				// Celda temporal de la matriz B.

    double timetick; 					// Se usa para medir el tiempo.

    // Se debe enviar el N como argumento. Si no se envía, alertar y terminar.
    if (
        (argc != 4) ||
        ((N = atoi(argv[1])) <= 0) ||
        ((block_size = atoi(argv[2])) <= 0) ||
        (block_size > N) ||
        (N % block_size != 0) ||
        (imprimir_matrices = atoi(argv[3])) < 0 ||
        (imprimir_matrices = atoi(argv[3])) > 1
    ) {
        printf("\nSe deben enviar 3 parámetros:\n");
        printf("El N de la dimensión de las matrices, el tamaño del bloque y un 0 o 1 indicando si se quiere imprimir las matrices o no.\n");
        printf("Ejemplo con N = 16, tamaño de bloque = 4 y sin imprimir: \n ./test 16 4 0\n");
        exit(1);
    }

    // Alocar memoria para las cuatro matrices principales y las dos auxiliares.
    A = (double * ) malloc(N * N * sizeof(double));
    B = (double * ) malloc(N * N * sizeof(double));
    B_T = (double * ) malloc(N * N * sizeof(double));
    C = (double * ) malloc(N * N * sizeof(double));
    R = (double * ) malloc(N * N * sizeof(double));
    a_por_b = (double * ) malloc(N * N * sizeof(double));
    c_por_bt = (double * ) malloc(N * N * sizeof(double));

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

            B[j * N + i] = valorInicial;
            R[i * N + j] = 0.0;
            a_por_b[i * N + j] = 0.0;
            c_por_bt[i * N + j] = 0.0;

            valorInicial += 1.0;
        }
    }

    // Imprimir las matrices.
  	if (imprimir_matrices) {
        printf("Matriz A (ordenada por filas e inicializada como matriz identidad):\n");
        imprimir_matriz(A, N);
        printf("Matriz B (ordenada por columnas e inicializada de forma incremental):\n");
        imprimir_matriz(B, N);
        printf("Matriz C (ordenada por filas e inicializada como matriz identidad):\n");
        imprimir_matriz(C, N);
    }

    // Comenzar a medir el tiempo.
    timetick = dwalltime();

    // Inicializar la matriz B_T (B transpuesta).
    for(i = 0; i < N ; i++)
        for(j = 0; j < N ; j++) B_T[j * N + i] = B[i * N + j];

    // Calcular el valor máximo, mínimo y promedio de la matriz A.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            celdaA = A[i * N + j];
            sumaA += celdaA;
            if (celdaA > maxA) maxA = celdaA;
            if (celdaA < minA) minA = celdaA;
        }
    }

    // Calcular el valor máximo, mínimo y promedio de la matriz B.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            celdaB = B[i * N + j];
            sumaB += celdaB;
            if (celdaB > maxB) maxB = celdaB;
            if (celdaB < minB) minB = celdaB;
        }
    }

    promA = sumaA / (N * N);
    promB = sumaB / (N * N);
    cociente = ((maxA * maxB) - (minA * minB)) / (promA * promB);

    // Resolver [A * B] y guardarlo en una matriz auxiliar a_por_b.
    for (i = 0; i < N; i += block_size) {
        for (j = 0; j < N; j += block_size) {
            for (k = 0; k < N; k += block_size) {
                multiplicar_bloque( & A[i * N + k], & B[j * N + k], & a_por_b[i * N + j], N, block_size);
            }
        }
    }

    // Resolver [C * B^T] y guardarlo en una matriz auxiliar c_por_bt.
    for (i = 0; i < N; i += block_size) {
        for (j = 0; j < N; j += block_size) {
            for (k = 0; k < N; k += block_size) {
                multiplicar_bloque( & C[i * N + k], & B_T[j * N + k], & c_por_bt[i * N + j], N, block_size);
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
    printf("Tiempo que llevó computar la ecuación con N = %d ---> %f.\n\n", N, dwalltime() - timetick);

  	if (imprimir_matrices) {
        printf("Matriz A×B:\n");
        imprimir_matriz(a_por_b, N);
        printf("Matriz C×B_T:\n");
        imprimir_matriz(c_por_bt, N);
        printf("Matriz R:\n");
        imprimir_matriz(R, N);
    }

    printf("Valor máximo de A: %f\n", maxA);
    printf("Valor máximo de B: %f\n", maxB);

    printf("Valor mínimo de A: %f\n", minA);
    printf("Valor mínimo de B: %f\n", minB);

    printf("Promedio de A: %f\n", promA);
    printf("Promedio de B: %f\n", promB);

    printf("Cociente: %f\n", cociente);

    // Liberar la memoria que alocamos a las matrices al inicio.
    free(A);
    free(B);
    free(C);
    free(R);
    free(a_por_b);
    free(c_por_bt);

    return (0);
}
