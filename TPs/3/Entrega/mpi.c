#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include "mpi.h" 

#define MASTER 0
#define EPSILON 1e-6

// Chequear que Cociente × [A×B] + [C×B_T] es correcto.
int chequear_resultados(double * R, double * a_por_b, double * c_por_bt, int N, double cociente) {
    int i, j;
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

void blkmul(double * a, double * b, double * c, int n, int bs) {
    int i, j, k;

    for (i = 0; i < bs; i++) {
        for (j = 0; j < bs; j++) {
            double aux = 0.0;
            for (k = 0; k < bs; k++) {
                aux += a[i * n + k] * b[j * n + k];
            }
            c[i * n + j] += aux;
        }
    }
}

void matmulblks(double *a, double *b, double *c, int n, int bs, int stripSize)
{
    int i, j, k;

    for (i = 0; i < stripSize; i += bs) {
        for (j = 0; j < n; j += bs) {
            for (k = 0; k < n; k += bs)
                blkmul(&a[i * n + k], &b[j * n + k], &c[i * n + j], n, bs);
        }
    }
}



int main(int argc, char * argv[]) {
    /* ARGUMENTOS */
    int N = -1; 						// Tamaño de las matrices cuadradas (N×N).
    int tam_bloque = 64;                // Tamaño del bloque para la multiplicación por bloques.

    /* MPI */
    int rank;
    int size;
    int stripSize;
    double commTimes[8], maxCommTimes[8], minCommTimes[8], commTime, totalTime;


    /* MATRICES */
    double * A, * B, * B_T, * C, * R, * a_por_b, * c_por_bt;   
    int i, j; 		
    double acumulador = 1.0;            // Acumulador para los valores de la matriz B.
    double cociente = 0; 				// Variable auxiliar que almacenará el resultado de la primer parte de la ecuación (la división).

    /* MÍNIMO, MÁXIMO, PROMEDIO */
    double max_AB[2] = {-999.0, -999.0};
    double min_AB[2] = {999.0, 999.0};
    double prom_AB[2] = {0.0, 0.0};

    double local_max[2] = {-999.0, -999.0};
    double local_min[2] = {999.0, 999.0};
    double local_prom[2] = {0.0, 0.0};

    double celdaA = 0.0;	
    double celdaB = 0.0;
    

    // Se debe enviar el N
    if ( (argc != 2) || ((N = atoi(argv[1])) <= 0)) {
        printf("\nError en los parametros. Uso: ./%s N\n", argv[0]);
        exit(1);
    }

    MPI_Init( &argc, &argv );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    if (N % size != 0) {
        printf("El tamaño de la matriz debe ser múltiplo del numero de procesos.\n");
        exit(1);
    }

    stripSize = N / size;
    tam_bloque = (stripSize < tam_bloque ? stripSize : tam_bloque);

    // Alocar memoria para las cuatro matrices principales y las dos auxiliares.
    if (rank == MASTER) {  
        A = (double *)malloc(N * N * sizeof(double));
        C = (double *)malloc(N * N * sizeof(double));
        a_por_b = (double *)malloc(N * N * sizeof(double));
        c_por_bt = (double *)malloc(N * N * sizeof(double));
        R = (double *)malloc(N * N * sizeof(double));
    }
    else {
        A = (double *)malloc(N * stripSize  * sizeof(double));
        C = (double *)malloc(N * stripSize * sizeof(double));
        a_por_b = (double *)malloc(N * stripSize * sizeof(double));
        c_por_bt = (double *)malloc(N * stripSize * sizeof(double));
        R = (double *)malloc(N * stripSize * sizeof(double));
    }

    B_T = (double *)malloc(N * N * sizeof(double));
    B = (double *)malloc(N * N * sizeof(double));

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
        for (i = 0; i < stripSize; i++) {
            for (j = 0; j < stripSize; j++) {
                a_por_b[i * N + j] = 0.0;
                c_por_bt[i * N + j] = 0.0;
                R[i * N + j] = 0.0;
            }
        }
    }
    
    MPI_Barrier(MPI_COMM_WORLD);

    commTimes[0] = MPI_Wtime();

    MPI_Scatter(A, N * stripSize, MPI_DOUBLE, A, N * stripSize, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    MPI_Scatter(C, N * stripSize, MPI_DOUBLE, C, N * stripSize, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    MPI_Bcast(B_T, N * N , MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    commTimes[1] = MPI_Wtime();

    // Calcular el valor máximo, mínimo y promedio de la matriz A.
    for(i = 0; i < stripSize; i++) {
        for(j = 0; j < N; j++) {

            celdaA = A[i * N + j];

            if(celdaA > local_max[0]) {
                local_max[0] = celdaA;
            }

            if(celdaA < local_min[0]) {
                local_min[0] = celdaA;
            }

            local_prom[0] += celdaA;
        }
    }

    // Calcular el valor máximo, mínimo y promedio de la matriz B.
    for(i = 0; i < stripSize; i++) {
        for(j = 0; j < N; j++) {

            celdaB = B[i * N + j];

            if(celdaB > local_max[1]) {
                local_max[1] = celdaB;
            }

            if(celdaB < local_min[1]) {
                local_min[1] = celdaB;
            }

            local_prom[1] += celdaB;
        }
    }

    commTimes[2] = MPI_Wtime();

    MPI_Reduce(&local_min, &min_AB, 2, MPI_DOUBLE, MPI_MIN, MASTER, MPI_COMM_WORLD);
    MPI_Reduce(&local_max, &max_AB, 2, MPI_DOUBLE, MPI_MAX, MASTER, MPI_COMM_WORLD);
    MPI_Reduce(&local_prom, &prom_AB, 2, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
    
    commTimes[3] = MPI_Wtime();

    // Se calcula el cociente 
    if (rank == MASTER) {
        prom_AB[0] = prom_AB[0] / (N * N);
        prom_AB[0] = prom_AB[1] / (N * N);
        cociente = ((max_AB[0] * max_AB[1]) - (min_AB[0] * min_AB[1])) / (prom_AB[0] * prom_AB[1]);
    }

    // Los workers se repartieron B_T y cada uno inicializa su parte
    for(i = 0; i < stripSize ; i++) {
        for(j = 0; j < N ; j++) B_T[j * N + i] = B[i * N + j];
    }
        
    // Resolver [A×B] y guardarlo en una matriz auxiliar a_por_b.
    matmulblks(A , B , a_por_b, N, tam_bloque, stripSize);

    // Resolver [C×B_T] y guardarlo en una matriz auxiliar c_por_bt.
    matmulblks(C , B_T , c_por_bt, N, tam_bloque, stripSize);

    commTimes[4] = MPI_Wtime();

    MPI_Bcast(&cociente, 1, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    commTimes[5] = MPI_Wtime();

    
    for (i = 0; i < stripSize; i++) {
        for (j = 0; j < N; j++) {
            R[i * N + j] = (a_por_b[i * N + j] * cociente) + (c_por_bt[i * N + j]);
        }
    }

    commTimes[6] = MPI_Wtime();

    MPI_Gather(R, stripSize * N, MPI_DOUBLE, R, stripSize * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    commTimes[7] = MPI_Wtime();

    // Fin de la medición del tiempo de comunicación.

    MPI_Reduce(commTimes, minCommTimes, 8, MPI_DOUBLE, MPI_MIN, MASTER, MPI_COMM_WORLD);
    MPI_Reduce(commTimes, maxCommTimes, 8, MPI_DOUBLE, MPI_MAX, MASTER, MPI_COMM_WORLD);

    MPI_Gather(a_por_b, stripSize * N, MPI_DOUBLE, a_por_b, stripSize * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    MPI_Gather(c_por_bt, stripSize * N, MPI_DOUBLE, c_por_bt, stripSize * N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    if (rank == MASTER) {
        totalTime = maxCommTimes[7] - minCommTimes[0];
        commTime = (maxCommTimes[1] - minCommTimes[0]) + (maxCommTimes[3] - minCommTimes[2]) + (maxCommTimes[5] - minCommTimes[4]) + (maxCommTimes[7] - minCommTimes[6]);

        printf("\nTiempo total=%lf\tTiempo comunicacion=%lf\n", totalTime, commTime);

        chequear_resultados(R, a_por_b, c_por_bt, N, cociente);
    }

    MPI_Finalize();

    free(A);
    free(B);
    free(B_T);
    free(C);
    free(R);
    free(a_por_b);
    free(c_por_bt);

    return (0);
}
