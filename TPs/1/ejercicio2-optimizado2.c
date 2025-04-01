#include <stdio.h>

#include <sys/time.h>

#include <stdlib.h>

int random_entre_a_b(int a, int b) {
    return a + rand() % (b - a + 1);
}

void imprimir_matriz(double * A, int N, int por_fila) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
          	if(por_fila) printf("%8.2f ", A[i * N + j]);
          	else printf("%8.2f ", A[j * N + i]);
        }
        printf("\n");
    }
    printf("\n");
}

// Para calcular tiempo
double dwalltime() {
    double sec;
    struct timeval tv;
    gettimeofday( & tv, NULL);
    sec = tv.tv_sec + tv.tv_usec / 1000000.0;
    return sec;
}

int main(int argc, char * argv[]) {  
  	int imprimir_matrices = -1;
    int N = -1; 						// Tamaño de las matrices cuadradas (N * N).
    int i, j; 							// Índices para recorrer las matrices → i para fila; j para columna.
    double * A, * B, * C, * R; 			// Matrices A B C R.
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
    
    double sumaAux1 = 0.0;
    double sumaAux2 = 0.0;
    double valorA, valorC = 0.0;

    // Se debe enviar el N como argumento. Si no se envía, alertar y terminar.
    if (
      (argc != 3) || 
      ((N = atoi(argv[1])) <= 0) || 
      (imprimir_matrices = atoi(argv[2])) < 0 ||
      (imprimir_matrices = atoi(argv[2])) > 1
    ) {
        printf("\nSe deben enviar 2 parámetros:\n");
        printf("El N de la dimensión de las matrices, y un 0 o 1 indicando si se quiere imprimir las matrices o no.\n");
      	printf("Ejemplo si se quiere imprimir: \n ./test 4 1\n");
        exit(1);
    }

    // Alocar memoria para las cuatro matrices principales y las dos auxiliares.
    A = (double * ) malloc(N * N * sizeof(double));
    B = (double * ) malloc(N * N * sizeof(double));
    C = (double * ) malloc(N * N * sizeof(double));
    R = (double * ) malloc(N * N * sizeof(double));
    mul1 = (double * ) malloc(N * N * sizeof(double));
    mul2 = (double * ) malloc(N * N * sizeof(double));

    // Inicializar las cuatro matrices principales y las dos auxiliares.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i * N + j] = random_entre_a_b(1, 64);
            B[i * N + j] = random_entre_a_b(1, 64);
            C[i * N + j] = random_entre_a_b(1, 64);
            R[i * N + j] = 0.0;
            mul1[i * N + j] = 0.0;
            mul2[i * N + j] = 0.0;
        }
    }

    // Imprimir las matrices
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

    // Obtener los mínimos, máximos y promedios, realizar la división y guardar el resultado en una variable auxiliar (cociente).
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            celdaA = A[i * N + j];
            celdaB = B[i * N + j];
            sumaA += celdaA;
            sumaB += celdaB;
            if (celdaA > maxA) {
                maxA = celdaA;
            }
            if (celdaA < minA) {
                minA = celdaA;
            }
            if (celdaB > maxB) {
                maxB = celdaB;
            }
            if (celdaB < minB) {
                minB = celdaB;
            }
        }
    }
    promA = sumaA / (N * N);
    promB = sumaB / (N * N);
    cociente = ((maxA * maxB) - (minA * minB)) / (promA * promB);

    // Resolver [A * B] y guardarlo en una matriz auxiliar mul1.
    // Resolver [C * B^T] y guardarlo en una matriz auxiliar mul2.
    for (i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
          	valorA = A[i * N + k];
          	valorC = C[i * N + k];
            for (j = 0; j < N; j++) {
              	mul1[i * N + j] += valorA * B[k * N + j];
              	mul2[i * N + j] += valorC * B[j * N + k];
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
