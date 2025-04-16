#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

int *vectorA, *vectorB, *vectorC;
int N = -1;
int cantidad_hilos = -1;

void imprimir_vector(int *vector, int N, char nombre_vector) {
    printf("Vector %c:\n", nombre_vector);
    for(int i = 0; i < N; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}

int random_entre_a_b(int a, int b) {
    return a + rand() % (b - a + 1);
}

double dwalltime() {
    double sec;
    struct timeval tv;
    gettimeofday( & tv, NULL);
    sec = tv.tv_sec + tv.tv_usec / 1000000.0;
    return sec;
}

void* sumar_bloque(void* arg) {
    int id_hilo = (int)(size_t)arg;
    int bloque = N / cantidad_hilos;
    int inicio = id_hilo * bloque;
    int fin = (id_hilo + 1) * bloque;
    printf("Hilo %d: procesando elementos de %d a %d\n", id_hilo, inicio, fin);
    for(int i = inicio; i < fin; i++) {
        vectorC[i] = vectorA[i] + vectorB[i];
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    double timetick;
    int rango = 15;
    int i;
    int imprimir;

    if(argc != 4) {
        printf("Error: Se debe enviar el tamaño de los dos vectores (N), si se quiere imprimir o no (1 o 0), y la cantidad de hilos\n");
        return 1;
    }
    if(atoi(argv[1]) <= 0) {
        printf("Error: El tamaño de los vectores debe ser mayor que 0\n");
        return 1;
    }
    if(atoi(argv[2]) < 0) {
        printf("Error: 1 o 0\n");
        return 1;
    }
    if(atoi(argv[3]) <= 0 || atoi(argv[3]) % 2 != 0) {
        printf("Error: La cantidad de hilos debe ser mayor que 0 y par\n");
        return 1;
    }

    N = atoi(argv[1]);
    imprimir = atoi(argv[2]);
    cantidad_hilos = atoi(argv[3]);

    // Declarar los tres vectores
    vectorA = (int*)malloc(N * sizeof(int));
    vectorB = (int*)malloc(N * sizeof(int));
    vectorC = (int*)malloc(N * sizeof(int));

    // Inicializar los tres vectores
    for(i = 0; i < N; i++) {
        vectorA[i] = random_entre_a_b(1, rango);
        vectorB[i] = random_entre_a_b(1, rango);
        vectorC[i] = 0;
    }

    if(imprimir) {
        imprimir_vector(vectorA, N, 'A');
        imprimir_vector(vectorB, N, 'B');
    }

    pthread_t hilos[cantidad_hilos];
    timetick = dwalltime();

    for(i = 0; i < cantidad_hilos; i++) {
        if(pthread_create(&hilos[i], NULL, sumar_bloque, (void*)(size_t)i)) {
            printf("Error al crear el hilo %d\n", i);
            return 1;
        }
    }

    for(i = 0; i < cantidad_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Tiempo de ejecución: %f segundos\n", dwalltime() - timetick);

    if(imprimir) {
        imprimir_vector(vectorC, N, 'C');
    }

    free(vectorA);
    free(vectorB);
    free(vectorC);
    return 0;
}