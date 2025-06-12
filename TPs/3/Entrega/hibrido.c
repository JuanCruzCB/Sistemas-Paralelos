#include <stdio.h>
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
    double aux;
    for (i = 0; i < tam_bloque; i++) {
        for (j = 0; j < tam_bloque; j++) {
            aux = 0.0;
            for (k = 0; k < tam_bloque; k++) {
                aux += primer_bloque[i * n + k] * segundo_bloque[j * n + k];
            }
            bloque_resultado[i * n + j] += aux;
        }
    }
}

int main(int argc, char * argv[]) {
    /* ARGUMENTOS */
    int N; 						                    // Tamaño de las matrices cuadradas (N×N).

    /* MPI */
    int rank;                                       // ID de cada proceso.
    int cantidad_procesos;                          // Cantidad de procesos.
    int tam_submatriz;                              // Porción de las matrices que trabaja cada proceso.
    double tiempos_comunicacion[8];                 // Timestamps de inicio y fin de cada comunicación MPI para luego calcularle la diferencia.
    double tiempos_comunicacion_max[8];             //
    double tiempos_comunicacion_min[8];             //
    double tiempo_comunicacion_total;               //
    double tiempo_total;                            //

    /* MATRICES */
    double * A, * B, * B_T, * C, * R, * a_por_b, * c_por_bt;
    int i, j, k;
    double acumulador = 1.0;                        // Acumulador para los valores de la matriz B.
    double cociente = 0; 				            // Variable auxiliar que almacenará el resultado de la primer parte de la ecuación (la división).
    int tam_bloque = 128;                           // Tamaño del bloque para la multiplicación por bloques.

    /* MÍNIMO, MÁXIMO, PROMEDIO */
    double max_AB[2] = {-999.0, -999.0};
    double min_AB[2] = {999.0, 999.0};
    double prom_AB[2] = {0.0, 0.0};
    double max[2] = {-999.0, -999.0};
    double min[2] = {999.0, 999.0};
    double prom[2] = {0.0, 0.0};
    double celda_A = 0.0;
    double celda_B = 0.0;

    /* HILOS */
    int T = atoi(getenv("OMP_NUM_THREADS"));        // Cantidad de hilos a usar por proceso.
    int provisto;

    // Se debe enviar el N de tamaño de las matrices.
    if ((argc != 2) || ((N = atoi(argv[1])) <= 0)) {
        printf("\nError en los parametros. Uso: ./%s N\n", argv[0]);
        exit(1);
    }

    // Inicializar entorno MPI.
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provisto);
    MPI_Comm_size(MPI_COMM_WORLD, &cantidad_procesos);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Validación de N.
    if (N % cantidad_procesos != 0) {
        printf("El tamaño de la matriz debe ser múltiplo del numero de procesos.\n");
        exit(1);
    }

    // Se define el tamaño de cada submatriz para los procesos workers y se ajusta el tamaño de bloque si es necesario.
    tam_submatriz = N / cantidad_procesos;
    tam_bloque = (N / (cantidad_procesos * T) < tam_bloque ? N / (cantidad_procesos * T) : tam_bloque);

    // Alocar memoria para las cuatro matrices principales y las dos auxiliares.
    // El master tiene que repartir las matrices A y C entre los workers, por eso las tiene completas.
    if (rank == MASTER) {
        printf("El tamaño de bloque a usar será: %d\n", tam_bloque);
        A = (double *)malloc(N * N * sizeof(double));
        C = (double *)malloc(N * N * sizeof(double));
        a_por_b = (double *)malloc(N * N * sizeof(double));
        c_por_bt = (double *)malloc(N * N * sizeof(double));
        R = (double *)malloc(N * N * sizeof(double));
    }
    else {
        A = (double *)malloc(N * tam_submatriz * sizeof(double));
        C = (double *)malloc(N * tam_submatriz * sizeof(double));
        a_por_b = (double *)malloc(N * tam_submatriz * sizeof(double));
        c_por_bt = (double *)malloc(N * tam_submatriz * sizeof(double));
        R = (double *)malloc(N * tam_submatriz * sizeof(double));
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
        for (i = 0; i < tam_submatriz; i++) {
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
    // El master le pasa una copia de la matriz B entera a los procesos workers.
    MPI_Scatter(A, N * tam_submatriz, MPI_DOUBLE, A, N * tam_submatriz, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    MPI_Scatter(C, N * tam_submatriz, MPI_DOUBLE, C, N * tam_submatriz, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    MPI_Bcast(B, N * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    tiempos_comunicacion[1] = MPI_Wtime();

    #pragma omp parallel private(i, j, k, celda_A, celda_B)
    {
        // Calcular el valor máximo, mínimo y promedio de la submatriz A de cada proceso.
        #pragma omp for reduction(min : min_AB[0]) reduction(max : max_AB[0]) reduction(+ : prom_AB[0]) nowait schedule(static)
        for (i = 0; i < tam_submatriz; i++) {
            for (j = 0; j < N; j++) {
                celda_A = A[i * N + j];

                if (celda_A < min_AB[0]) min_AB[0] = celda_A;

                if (celda_A > max_AB[0]) max_AB[0] = celda_A;

                prom_AB[0] += celda_A;
            }
        }

        // Calcular el valor máximo, mínimo y promedio de la matriz B.
        #pragma omp for reduction(min : min_AB[1]) reduction(max : max_AB[1]) reduction(+ : prom_AB[1]) nowait schedule(static)
        for (i = rank * tam_submatriz; i < rank * tam_submatriz + tam_submatriz; i++) {
            for (j = 0; j < N; j++) {
                celda_B = B[i * N + j];

                if (celda_B < min_AB[1]) min_AB[1] = celda_B;

                if (celda_B > max_AB[1]) max_AB[1] = celda_B;

                prom_AB[1] += celda_B;
            }
        }

        // Se multiplica la submatriz de A por B.
        #pragma omp for nowait schedule(static)
        for (i = 0; i < tam_submatriz; i += tam_bloque) {
            for (j = 0; j < N; j += tam_bloque) {
                for (k = 0; k < N; k += tam_bloque) {
                    multiplicar_bloque(&A[i * N + k], &B[j * N + k], &a_por_b[i * N + j], N, tam_bloque);
                }
            }
        }

        #pragma omp master
        {
            // Lo ejecuta el hilo master de cada proceso, el mínimo, máximo y promedio final de A y B se lo queda el PROCESO MASTER.
            tiempos_comunicacion[2] = MPI_Wtime();
            MPI_Reduce(&min_AB, &min, 2, MPI_DOUBLE, MPI_MIN, MASTER, MPI_COMM_WORLD);
            MPI_Reduce(&max_AB, &max, 2, MPI_DOUBLE, MPI_MAX, MASTER, MPI_COMM_WORLD);
            MPI_Reduce(&prom_AB, &prom, 2, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
            tiempos_comunicacion[3] = MPI_Wtime();

            if(rank == MASTER) { // Lo ejecuta UNICAMENTE el proceso master
                prom[0] = prom[0] / (N * N);
                prom[1] = prom[1] / (N * N);
                cociente = ((max[0] * max[1]) - (min[0] * min[1])) / (prom[0] * prom[1]);

                // Inicialización de la matriz B_T (B transpuesta).
                for(i = 0; i < N ; i++)
                    for(j = 0; j < N ; j++) B_T[j * N + i] = B[i * N + j];
            }

            tiempos_comunicacion[4] = MPI_Wtime();
            MPI_Bcast(B_T, N * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
            MPI_Bcast(&cociente, 1, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
            tiempos_comunicacion[5] = MPI_Wtime();
        }

        #pragma omp barrier // Los hilos esperan a que el hilo master haya inicializado B_T y cociente

        #pragma omp for nowait schedule(static)
        for (i = 0; i < tam_submatriz; i += tam_bloque) {
            for (j = 0; j < N; j += tam_bloque) {
                for (k = 0; k < N; k += tam_bloque) {
                    multiplicar_bloque(&C[i * N + k], &B_T[j * N + k], &c_por_bt[i * N + j], N, tam_bloque);
                }
            }
        }

        #pragma omp for nowait schedule(static)
        for (i = 0; i < tam_submatriz; i++) {
            for (j = 0; j < N; j++) {
                R[i * N + j] = (a_por_b[i * N + j] * cociente) + (c_por_bt[i * N + j]);
            }
        }
    }

    tiempos_comunicacion[6] = MPI_Wtime();
    // Cada worker le envía al master su porción de R y el master lo combina todo en R.
    MPI_Gather(R, tam_submatriz * N, MPI_DOUBLE, R, tam_submatriz * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    tiempos_comunicacion[7] = MPI_Wtime();

    // Fin de la medición de los tiempos de comunicación.

    // El master obtiene los 8 mínimos y 8 máximos de cada medición.
    MPI_Reduce(tiempos_comunicacion, tiempos_comunicacion_min, 8, MPI_DOUBLE, MPI_MIN, MASTER, MPI_COMM_WORLD);
    MPI_Reduce(tiempos_comunicacion, tiempos_comunicacion_max, 8, MPI_DOUBLE, MPI_MAX, MASTER, MPI_COMM_WORLD);
    // El master obtiene las matrices a_por_b y c_por_bt completas que las necesita para chequear que los resultados
    // son correctos.
    MPI_Gather(a_por_b, tam_submatriz * N, MPI_DOUBLE, a_por_b, tam_submatriz * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    MPI_Gather(c_por_bt, tam_submatriz * N, MPI_DOUBLE, c_por_bt, tam_submatriz * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

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
