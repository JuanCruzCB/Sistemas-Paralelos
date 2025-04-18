#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

pthread_mutex_t lock;
int N = -1;
int cantidad_hilos = -1;
int* vector;
int ocurrencias = 0;
int x = -1;

void* hilo_buscador(void* arg) {
    int id = (int)(size_t)arg;
    int i, inicio, fin;
    int local_ocurrencias = 0;

    inicio = (N / cantidad_hilos) * id;
    fin = (N / cantidad_hilos) * (id + 1);

    for(i = inicio; i < fin; i++) {
        if(vector[i] == x) {
            local_ocurrencias++;
        }
    }

    pthread_mutex_lock(&lock);
    ocurrencias += local_ocurrencias;
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

void imprimir_vector(int *vector, int N) {
    printf("Vector:\n");
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

int main(int argc, char *argv[]) {
    double timetick;
    int rango = 15;
    int i;
    int imprimir;

    if(argc != 5) {
        printf("Error: Se debe enviar el tamaño del vector (N), si se quiere imprimir o no (1 o 0), el número a buscar en el vector, y la cantidad de hilos\n");
        return 1;
    }
    if(atoi(argv[1]) <= 0) {
        printf("Error: El tamaño del vector debe ser mayor que 0\n");
        return 1;
    }
    if(atoi(argv[2]) < 0) {
        printf("Error: 1 o 0\n");
        return 1;
    }
    if(atoi(argv[3]) < 1) {
        printf("Error: El número a buscar debe ser mayor o igual a 1\n");
        return 1;
    }
    if(atoi(argv[4]) < 1) {
        printf("Error: La cantidad de hilos debe ser mayor o igual a 1\n");
        return 1;
    }
    if(atoi(argv[4]) % 2 != 0) {
        printf("Error: La cantidad de hilos debe ser par\n");
        return 1;
    }

    N = atoi(argv[1]);
    imprimir = atoi(argv[2]);
    x = atoi(argv[3]);
    cantidad_hilos = atoi(argv[4]);

    pthread_t hilos[cantidad_hilos];

    // Declarar el vector
    vector = (int*)malloc(N * sizeof(int));

    // Inicializar el vector
    for(i = 0; i < N; i++) {
        vector[i] = random_entre_a_b(1, rango);
    }

    if(imprimir) {
        imprimir_vector(vector, N);
    }

    timetick = dwalltime();

    for(i = 0; i < cantidad_hilos; i++) {
        if(pthread_create(&hilos[i], NULL, hilo_buscador, (void*)(size_t)i)) {
            printf("Error al crear el hilo %d\n", i);
            return 1;
        }
    }

    for(i = 0; i < cantidad_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Tiempo de ejecución: %f segundos\n", dwalltime() - timetick);
    printf("Número de ocurrencias de %d: %d\n", x, ocurrencias);

    free(vector);
    return 0;
}