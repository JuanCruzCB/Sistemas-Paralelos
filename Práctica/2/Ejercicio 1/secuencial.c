#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

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

int main(int argc, char *argv[]) {
    double timetick;
    int N = -1;
    int rango = 15;
    int i;
    int imprimir;
    int* vectorA;
    int* vectorB;
    int* vectorC;

    if(argc != 3) {
        printf("Error: Se debe enviar el tamaño de los dos vectores (N) y si se quiere imprimir o no (1 o 0)\n");
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

    N = atoi(argv[1]);
    imprimir = atoi(argv[2]);

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

    timetick = dwalltime();

    for(i = 0; i < N; i++) {
        vectorC[i] = vectorA[i] + vectorB[i];
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