#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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
    int N = -1;
    int rango = 15;
    int i;
    int imprimir;
    int* vector;

    int min = 999;
    int max = -999;
    double promedio = -1.0;
    int suma = 0;

    if(argc != 3) {
        printf("Error: Se debe enviar el tamaño del vector (N) y si se quiere imprimir o no (1 o 0)\n");
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

    N = atoi(argv[1]);
    imprimir = atoi(argv[2]);

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

    for(i = 0; i < N; i++) {
        suma += vector[i];
        if(vector[i] < min) {
            min = vector[i];
        }
        if(vector[i] > max) {
            max = vector[i];
        }
    }

    promedio = (double)suma / N;

    printf("Tiempo de ejecución: %f segundos\n", dwalltime() - timetick);
    printf("Mínimo: %d, máximo: %d, promedio: %f\n", min, max, promedio);

    free(vector);
    return 0;
}