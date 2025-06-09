#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "mpi.h"

#define MASTER 0
#define EPSILON 1e-6

int chequear_resultados(double * A, double * B, double * B_T, double * C, double * R, double * a_por_b, double * c_por_bt, int N, double cociente) {
    int i, j;

    printf("Chequeando resultados...\n");
    // Chequear que A×B es correcto.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (a_por_b[i * N + j] != B[j * N + i]) {
                printf("Error en la multiplicación A×B en la posición [%d][%d]\n", i, j);
                return 1;
            }
        }
    }
    printf("Multiplicación A×B correcta ya que A×B = B.\n");

    // Chequear que C×B_T es correcto.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (c_por_bt[i * N + j] != B_T[j * N + i]) {
                printf("Error en la multiplicación C×B_T en la posición [%d][%d]\n", i, j);
                return 1;
            }
        }
    }
    printf("Multiplicación C×B_T correcta ya que C×B_T = B_T.\n");

    // Chequear que Cociente es correcto.
    double numerador = (1 * (N * N)) - (0 * 1);
    double denominador = (1.0 / N) * ((N * N + 1) / 2.0);
    double valor_esperado = numerador / denominador;
    double diferencia = (cociente - valor_esperado >= 0) ? (cociente - valor_esperado) : -(cociente - valor_esperado);
    if(diferencia > EPSILON) {
        printf("Error en el cálculo del cociente.\n");
        return 1;
    }
    printf("Cociente correcto.\n");

    // Chequear que Cociente × [A×B] es correcto.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (a_por_b[i * N + j] * cociente != B[j * N + i] * cociente) {
                printf("Error en la multiplicación Cociente × [A×B] en la posición [%d][%d]\n", i, j);
                return 1;
            }
        }
    }
    printf("Multiplicación Cociente × [A×B] correcta ya que Cociente × [A×B] = B × Cociente.\n");

    // Chequear que Cociente × [A×B] + [C×B_T] es correcto.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            double valor_esperado = (a_por_b[i * N + j] * cociente) + (c_por_bt[i * N + j]);
            double diferencia = (R[i * N + j] - valor_esperado >= 0) ? (R[i * N + j] - valor_esperado) : -(R[i * N + j] - valor_esperado);
            if (diferencia > EPSILON) {
                printf("Error en la multiplicación Cociente × [A×B] + [C×B_T] en la posición [%d][%d]\n", i, j);
                return 1;
            }
        }
    }
    printf("Multiplicación Cociente × [A×B] + [C×B_T] correcta.\n");
    return 0;
}

void multiplicar_bloque(double * primer_bloque, double * segundo_bloque, double * bloque_resultado, int n, int tam_bloque) {
    int i, j, k;

    for (i = 0; i < tam_bloque; i++) {
        for (j = 0; j < tam_bloque; j++) {
            double aux = 0.0;
            for (k = 0; k < tam_bloque; k++) {
                aux += primer_bloque[i * n + k] * segundo_bloque[j * n + k];
            }
            bloque_resultado[i * n + j] += aux;
        }
    }
}

void multiplicar_matrices(double *A, double *B, double *resultado, int n, int tam_bloque, int porcion)
{
    int i, j, k;

    for (i = 0; i < porcion; i += tam_bloque) {
        for (j = 0; j < n; j += tam_bloque) {
            for (k = 0; k < n; k += tam_bloque)
                multiplicar_bloque(&A[i * n + k], &B[j * n + k], &resultado[i * n + j], n, tam_bloque);
        }
    }
}

int main(int argc, char * argv[]) {
    /* ARGUMENTOS */
    int N = -1; 						// Tamaño de las matrices cuadradas (N×N).

    /* MPI */
    int rank;                           // ID de cada proceso.
    int size;                           // Cantidad de procesos.
    int porcion;                        // Porción de las matrices que trabaja cada proceso.
    double tiempos_comunicacion[8];     // Timestamps de inicio y fin de cada comunicación MPI para luego calcularle la diferencia.
    double tiempos_comunicacion_max[8]; //
    double tiempos_comunicacion_min[8]; //
    double tiempo_comunicacion_total;   //
    double tiempo_total;                //

    /* MATRICES */
    double * A, * B, * B_T, * C, * R, * a_por_b, * c_por_bt;
    int i, j;
    double acumulador = 1.0;            // Acumulador para los valores de la matriz B.
    double cociente = 0; 				// Variable auxiliar que almacenará el resultado de la primer parte de la ecuación (la división).
    int tam_bloque = 128;               // Tamaño del bloque para la multiplicación por bloques.

    /* MÍNIMO, MÁXIMO, PROMEDIO */
    double max_AB[2] = {-999.0, -999.0};
    double min_AB[2] = {999.0, 999.0};
    double prom_AB[2] = {0.0, 0.0};
    double local_max[2] = {-999.0, -999.0};
    double local_min[2] = {999.0, 999.0};
    double local_prom[2] = {0.0, 0.0};
    double celdaA = 0.0;
    double celdaB = 0.0;

    // Se debe enviar el N de tamaño de las matrices.
    if ((argc != 2) || ((N = atoi(argv[1])) <= 0)) {
        printf("\nError en los parametros. Uso: ./%s N\n", argv[0]);
        exit(1);
    }

    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (N % size != 0) {
        printf("El tamaño de la matriz debe ser múltiplo del numero de procesos.\n");
        exit(1);
    }

    porcion = N / size;
    tam_bloque = (N / (size * OMP_NUM_THREADS) < tam_bloque ? N / (size * OMP_NUM_THREADS) : tam_bloque);

    // Alocar memoria para las cuatro matrices principales y las dos auxiliares.
    // El master tiene que repartir las matrices A y C entre los workers, por eso las tiene completas.
    if (rank == MASTER) {
        printf("El tamaño de bloque a usar será: %d", tam_bloque);
        A = (double *)malloc(N * N * sizeof(double));
        C = (double *)malloc(N * N * sizeof(double));
        a_por_b = (double *)malloc(N * N * sizeof(double));
        c_por_bt = (double *)malloc(N * N * sizeof(double));
        R = (double *)malloc(N * N * sizeof(double));
    }
    else {
        A = (double *)malloc(N * porcion * sizeof(double));
        C = (double *)malloc(N * porcion * sizeof(double));
        a_por_b = (double *)malloc(N * porcion * sizeof(double));
        c_por_bt = (double *)malloc(N * porcion * sizeof(double));
        R = (double *)malloc(N * porcion * sizeof(double));
    }

    B = (double *)malloc(N * N * sizeof(double));
    B_T = (double *)malloc(N * N * sizeof(double));


    // Inicializar las cuatro matrices principales y las dos auxiliares.
    if (rank == MASTER) {
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
    }
    else {
        for (i = 0; i < porcion; i++) {
            for (j = 0; j < N; j++) {
                a_por_b[i * N + j] = 0.0;
                c_por_bt[i * N + j] = 0.0;
                R[i * N + j] = 0.0;
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    tiempos_comunicacion[0] = MPI_Wtime();
    // El master reparte la matriz A y la matriz C en partes iguales entre los procesos workers.
    // El master reparte la matriz B entera entre los procesos workers.
    MPI_Scatter(A, N * porcion, MPI_DOUBLE, A, N * porcion, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    MPI_Scatter(C, N * porcion, MPI_DOUBLE, C, N * porcion, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    MPI_Bcast(B, N * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    tiempos_comunicacion[1] = MPI_Wtime();

    #pragma omp parallel private(i, j, k, celdaA, celdaB)

        // Calcular el valor máximo, mínimo y promedio de la matriz A.
        #pragma omp for reduction(min : local_min[0]) reduction(max : local_max[0]) reduction(+ : local_prom[0]) nowait schedule(static)
            for (i = 0; i < stripSize; i++) {
                for (j = 0; j < N; j++) {
                    celdaA = A[i * N + j];

                    if (celdaA < local_min[0]) {
                        local_min[0] = celdaA;
                    }

                    if (celdaA > local_max[0]){
                        local_max[0] = celdaA;
                    }

                    local_prom[0] += celdaA;
                }
            }


        // Calcular el valor máximo, mínimo y promedio de la matriz B.
        #pragma omp for reduction(min : local_min[1]) reduction(max : local_max[1]) reduction(+ : local_prom[1]) schedule(static)
            for (i = rank * porcion; i < rank * porcion + porcion; i++) {
                for (j = 0; j < N; j++) {
                    celdaB = B[i * N + j];

                    if (celdaB < local_min[1]) {
                        local_min[1] = celdaB;
                    }

                    if (celdaB > local_max[1]) {
                        local_max[1] = celdaB;
                    }

                    local_prom[1] += celdaB;
                }
            }

        #pragma omp single
        {
            local_prom[0] = local_prom[0] / (N * N);
            local_prom[1] = local_prom[1] / (N * N);
            cociente = ((local_max[0] * local_max[1]) - (local_min[0] * local_min[1])) / (local_prom[0] * local_prom[1]);
        }

            
        #pragma omp master
        {    
            for(i = 0; i < N ; i++)
                for(j = 0; j < N ; j++) B_T[j * N + i] = B[i * N + j];

            tiempos_comunicacion[2] = MPI_Wtime();
            MPI_Bcast(B_T, N * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
            MPI_Bcast(cociente, 1, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
            tiempos_comunicacion[3] = MPI_Wtime();
        }

        #pragma omp for schedule(static) nowait
            for (i = 0; i < stripSize; i += tam_bloque) {
                for (j = 0; j < N; j += tam_bloque) {
                    for (k = 0; k < N; k += tam_bloque) {
                        multiplicar_bloque(&A[i * N + k], &B[j * N + k], &a_por_b[i * N + j], N, tam_bloque);
                    }
                }
            }

        #pragma omp for nowait schedule(static) nowait
            for (i = 0; i < stripSize; i += tam_bloque) {
                for (j = 0; j < N; j += tam_bloque) {
                    for (k = 0; k < N; k += tam_bloque) {
                        multiplicar_bloque(&C[i * N + k], &B_T[j * N + k], &c_por_bt[i * N + j], N, tam_bloque);
                    }
                }
            }

        #pragma omp for nowait schedule(static) nowait
            for (i = 0; i < porcion; i++) {
                for (j = 0; j < N; j++) {
                    R[i * N + j] = (a_por_b[i * N + j] * cociente) + (c_por_bt[i * N + j]);
                }
            }


    tiempos_comunicacion[6] = MPI_Wtime();
    // Cada worker le envía al master su porción de R y el master lo combina todo en R.
    MPI_Gather(R, porcion * N, MPI_DOUBLE, R, porcion * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    tiempos_comunicacion[7] = MPI_Wtime();

    // Fin de la medición de los tiempos de comunicación.

    // El master obtiene los 8 mínimos y 8 máximos de cada medición.
    MPI_Reduce(tiempos_comunicacion, tiempos_comunicacion_min, 8, MPI_DOUBLE, MPI_MIN, MASTER, MPI_COMM_WORLD);
    MPI_Reduce(tiempos_comunicacion, tiempos_comunicacion_max, 8, MPI_DOUBLE, MPI_MAX, MASTER, MPI_COMM_WORLD);
    // El master obtiene las matrices a_por_b y c_por_bt completas que las necesita para chequear que los resultados
    // son correctos.
    MPI_Gather(a_por_b, porcion * N, MPI_DOUBLE, a_por_b, porcion * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    MPI_Gather(c_por_bt, porcion * N, MPI_DOUBLE, c_por_bt, porcion * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    if (rank == MASTER) {
        // El tiempo total del programa es desde que empieza la primera comunicación hasta que termina la última.
        tiempo_total = tiempos_comunicacion_max[7] - tiempos_comunicacion_min[0];
        // El tiempo de comunicación total es la suma de los tiempos de todas las comunicaciones individuales que hubo.
        tiempo_comunicacion_total =
            (tiempos_comunicacion_max[1] - tiempos_comunicacion_min[0]) +
            (tiempos_comunicacion_max[3] - tiempos_comunicacion_min[2]) +
            (tiempos_comunicacion_max[5] - tiempos_comunicacion_min[4]) +
            (tiempos_comunicacion_max[7] - tiempos_comunicacion_min[6]);

        printf("Tiempo total = %lf\nTiempo de comunicación total = %lf\n\n", tiempo_total, tiempo_comunicacion_total);
        chequear_resultados(A, B, B_T, C, R, a_por_b, c_por_bt, N, cociente);
    }

    MPI_Finalize();

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
