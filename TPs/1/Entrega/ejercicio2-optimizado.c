#include <stdio.h>

#include <sys/time.h>

#include <stdlib.h>


int random_entre_a_b(int a, int b) {
    return a + rand() % (b - a + 1);
}

void imprimir_matriz(double * A, int N, int por_fila) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
          	if (por_fila) printf("%8.2f ", A[i * N + j]);
          	else printf("%8.2f ", A[j * N + i]);
        }
        printf("\n");
    }
    printf("\n");
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
            for (k = 0; k < block_size; k++) {
                bloque_resultado[i * n + j] += primer_bloque[i * n + k] * segundo_bloque[j * n + k];
            }
        }
    }
}

int main(int argc, char * argv[]) {
  	int imprimir_matrices = -1;         // Indica si se deben imprimir las matrices o no.
    int N = -1; 						// Tamaño de las matrices cuadradas (N * N).
    int i, j, k; 						// Índices para recorrer las matrices → i para fila; j para columna.
    double * A, * B, * B_T, * C, * R;   // Matrices A, B, B^T, C, R.
    double cociente = 0; 				// Variable auxiliar que almacenará el resultado de la primer parte de la ecuación (la división).
    double * mul1; 						// Matriz auxiliar que almacenará el resultado de A * B.
    double * mul2; 						// Matriz auxiliar que almacenará el resultado de C * B^T.
    double timetick; 					// Se usa para medir el tiempo.
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

    int block_size = -1;                // Tamaño del bloque para la multiplicación por bloques.

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
    mul1 = (double * ) malloc(N * N * sizeof(double));
    mul2 = (double * ) malloc(N * N * sizeof(double));

    // Inicializar las cuatro matrices principales y las dos auxiliares.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i * N + j] = random_entre_a_b(1, 4);
            B[i * N + j] = random_entre_a_b(1, 4);
            C[i * N + j] = random_entre_a_b(1, 4);
            R[i * N + j] = 0.0;
            mul1[i * N + j] = 0.0;
            mul2[i * N + j] = 0.0;
        }
    }

    // Imprimir las matrices.
  	if (imprimir_matrices) {
      printf("Matriz A:\n");
      imprimir_matriz(A, N, 1);

      printf("Matriz B:\n");
      imprimir_matriz(B, N, 1);

      printf("Matriz B transpuesta:\n");
      imprimir_matriz(B, N, 0);

      printf("Matriz C:\n");
      imprimir_matriz(C, N, 1);
    }

    // Comenzar a medir el tiempo.
    timetick = dwalltime();

    for(i = 0; i < N ; i++)
        for(j = 0; j < N ; j++) B_T[j * N + i] = B[i * N + j];

    // Calcular el valor máximo, mínimo y promedio de la matriz A
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            celdaA = A[i * N + j];
            sumaA += celdaA;
            if (celdaA > maxA) maxA = celdaA;
            if (celdaA < minA) minA = celdaA;
        }
    }

    // Calcular el valor máximo, mínimo y promedio de la matriz B
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

    // Resolver [A * B] y guardarlo en una matriz auxiliar mul1.
    for (i = 0; i < N; i += block_size) {
        for (j = 0; j < N; j += block_size) {
            for (k = 0; k < N; k += block_size) {
                multiplicar_bloque( & A[i * N + k], & B_T[j * N + k], & mul1[i * N + j], N, block_size);
            }
        }
    }

    // Resolver [C * B^T] y guardarlo en una matriz auxiliar mul2.
    for (i = 0; i < N; i += block_size) {
        for (j = 0; j < N; j += block_size) {
            for (k = 0; k < N; k += block_size) {
                multiplicar_bloque( & C[i * N + k], & B_T[k * N + j], & mul2[i * N + j], N, block_size);
            }
        }
    }


    // Finalmente multiplicar la matriz auxiliar mul1 por cociente y sumarle a eso la matriz mul2.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            R[i * N + j] = (mul1[i * N + j] * cociente) + (mul2[i * N + j]);
        }
    }

    // Terminar de medir el tiempo.
    printf("Tiempo que llevó computar la ecuación con N = %d ---> %f.\n\n", N, dwalltime() - timetick);

  	if (imprimir_matrices) {
      printf("Impresion de matriz A*B\n");
      imprimir_matriz(mul1, N, 1);
      printf("Impresion de matriz C*B^t\n");
      imprimir_matriz(mul2, N, 1);
      printf("Impresion de matriz R resultado\n");
      imprimir_matriz(R, N, 1);
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
    free(mul1);
    free(mul2);

    return (0);
}
