#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mi_semaforo;
int N = -1;
int cantidad_hilos = -1;
int* vector;
int min = 999;
int max = -999;
int suma = 0;

void* hilo_buscador(void* arg) {
    int id = (int)(size_t)arg;
    int i, inicio, fin;
    int suma_local = 0;
    int min_local = 999;
    int max_local = -999;

    inicio = (N / cantidad_hilos) * id;
    fin = (N / cantidad_hilos) * (id + 1);

    for(i = inicio; i < fin; i++) {
        suma_local += vector[i];
        if(vector[i] < min_local) {
            min_local = vector[i];
        }
        if(vector[i] > max_local) {
            max_local = vector[i];
        }
    }

    sem_wait(&mi_semaforo);
    suma += suma_local;
    min = (min < min_local) ? min : min_local;
    max = (max > max_local) ? max : max_local;
    sem_post(&mi_semaforo);
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
    double promedio = -1.0;

    if(argc != 3) {
        printf("Error: Se debe enviar el tamaño del vector (N), si se quiere imprimir o no (1 o 0) y la cantidad de hilos\n");
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
        printf("Error: La cantidad de hilos debe ser mayor o igual a 1\n");
        return 1;
    }
    if(atoi(argv[3]) % 2 != 0) {
        printf("Error: La cantidad de hilos debe ser par\n");
        return 1;
    }

    N = atoi(argv[1]);
    imprimir = atoi(argv[2]);
    cantidad_hilos = atoi(argv[3]);

    sem_init(&mi_semaforo, 0, 1);

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

    promedio = (double)suma / N;

    printf("Tiempo de ejecución: %f segundos\n", dwalltime() - timetick);
    printf("Mínimo: %d, máximo: %d, promedio: %f\n", min, max, promedio);

    free(vector);
    return 0;
}