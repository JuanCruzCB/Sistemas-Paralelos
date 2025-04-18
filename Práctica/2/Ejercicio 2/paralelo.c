#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

int tamaño_matriz = -1;
int tamaño_bloque = -1;
int cantidad_hilos = -1;
double *matriz_a, *matriz_b, *matriz_c;

void multiplicar_bloques(double* bloque_a, double* bloque_b, double* bloque_c, int tamaño, int tamaño_bloque) {
    int fila, columna, profundidad;

    for (fila = 0; fila < tamaño_bloque; fila++) {
        for (columna = 0; columna < tamaño_bloque; columna++) {
            for (profundidad = 0; profundidad < tamaño_bloque; profundidad++) {
                bloque_c[fila * tamaño + columna] += bloque_a[fila * tamaño + profundidad] * bloque_b[columna * tamaño + profundidad];
            }
        }
    }
}

void* hilo_multiplicador(void* arg) {
    int id = (int)(size_t)arg;
    int bloques = tamaño_matriz / tamaño_bloque;

    int bloques_por_hilo = bloques / cantidad_hilos;
    int bloque_inicio = id * bloques_por_hilo;
    int bloque_fin = (id + 1) * bloques_por_hilo;

    for (int bi = bloque_inicio; bi < bloque_fin; bi++) {
        for (int bj = 0; bj < bloques; bj++) {
            for (int bk = 0; bk < bloques; bk++) {
                multiplicar_bloques(
                    &matriz_a[(bi * tamaño_bloque) * tamaño_matriz + bk * tamaño_bloque],
                    &matriz_b[(bj * tamaño_bloque) * tamaño_matriz + bk * tamaño_bloque],
                    &matriz_c[(bi * tamaño_bloque) * tamaño_matriz + bj * tamaño_bloque],
                    tamaño_matriz,
                    tamaño_bloque
                );
            }
        }
    }

    pthread_exit(NULL);
}


double dwalltime() {
    double sec;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    sec = tv.tv_sec + tv.tv_usec / 1000000.0;
    return sec;
}

void inicializar_matriz_con_valor(double* matriz, int tamaño, double valor, int transponer) {
    int fila, columna;

    if (transponer == 0) {
        for (fila = 0; fila < tamaño; fila++) {
            for (columna = 0; columna < tamaño; columna++) {
                matriz[fila * tamaño + columna] = valor;
            }
        }
    } else {
        for (fila = 0; fila < tamaño; fila++) {
            for (columna = 0; columna < tamaño; columna++) {
                matriz[columna * tamaño + fila] = valor;
            }
        }
    }
}

void multiplicar_matrices_por_bloques(double* matriz_a, double* matriz_b, double* matriz_c, int tamaño, int tamaño_bloque) {
    int bloque_fila, bloque_columna, bloque_profundidad;

    for (bloque_fila = 0; bloque_fila < tamaño; bloque_fila += tamaño_bloque) {
        for (bloque_columna = 0; bloque_columna < tamaño; bloque_columna += tamaño_bloque) {
            for (bloque_profundidad = 0; bloque_profundidad < tamaño; bloque_profundidad += tamaño_bloque) {
                multiplicar_bloques(&matriz_a[bloque_fila * tamaño + bloque_profundidad],
                                  &matriz_b[bloque_columna * tamaño + bloque_profundidad],
                                  &matriz_c[bloque_fila * tamaño + bloque_columna],
                                  tamaño, tamaño_bloque);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int i, j;
    double tiempo_inicio, tiempo_ejecucion;

    if (argc != 4 || (tamaño_matriz = atoi(argv[1])) <= 0 ||
        (tamaño_bloque = atoi(argv[2])) <= 0 || (tamaño_matriz % tamaño_bloque) != 0 || (cantidad_hilos = atoi(argv[3])) <= 0 || (cantidad_hilos % 2 != 0)) {
        printf("\nParámetros inválidos. Uso: ./%s TAMAÑO_MATRIZ TAMAÑO_BLOQUE CANTIDAD_HILOS (TAMAÑO_MATRIZ debe ser múltiplo de TAMAÑO_BLOQUE)\n", argv[0]);
        exit(1);
    }

    pthread_t hilos[cantidad_hilos];

    matriz_a = (double*)malloc(tamaño_matriz * tamaño_matriz * sizeof(double));
    matriz_b = (double*)malloc(tamaño_matriz * tamaño_matriz * sizeof(double));
    matriz_c = (double*)malloc(tamaño_matriz * tamaño_matriz * sizeof(double));

    inicializar_matriz_con_valor(matriz_a, tamaño_matriz, 1.0, 0);
    inicializar_matriz_con_valor(matriz_b, tamaño_matriz, 1.0, 1);
    inicializar_matriz_con_valor(matriz_c, tamaño_matriz, 0.0, 0);

    tiempo_inicio = dwalltime();

    for(i = 0; i < cantidad_hilos; i++) {
        if(pthread_create(&hilos[i], NULL, hilo_multiplicador, (void*)(size_t)i)) {
            printf("Error al crear el hilo %d\n", i);
            return 1;
        }
    }

    for(i = 0; i < cantidad_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    tiempo_ejecucion = dwalltime() - tiempo_inicio;

    for (i = 0; i < tamaño_matriz; i++) {
        for (j = 0; j < tamaño_matriz; j++) {
            if (matriz_c[i * tamaño_matriz + j] != tamaño_matriz) {
                printf("Error en %d, %d, valor: %f\n", i, j, matriz_c[i * tamaño_matriz + j]);
            }
        }
    }

    printf("MMBLK-SEC;%d;%d;%lf;%lf\n", tamaño_matriz, tamaño_bloque, tiempo_ejecucion,
           ((double)2 * tamaño_matriz * tamaño_matriz * tamaño_matriz) / (tiempo_ejecucion * 1000000000));

    free(matriz_a);
    free(matriz_b);
    free(matriz_c);
    return 0;
}