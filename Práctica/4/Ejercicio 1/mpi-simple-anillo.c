#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

#define TAG 0

int main(int argc, char * argv[]) {
    int my_rank, size;
    int siguiente, anterior;
    MPI_Status status;
    char message[BUFSIZ];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Vecinos en el anillo.
    // Por ejemplo, si tenemos 8 procesos (0..7 inclusive), el proceso 7 tendrá de siguiente al 0, y de anterior al 6.
    siguiente = (my_rank + 1) % size;
    anterior = (my_rank - 1 + size) % size;

    if (my_rank == 0) {
        // El proceso 0 inicia la cadena.
        sprintf(message, "Hola mundo desde el proceso: %d", my_rank);
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, siguiente, TAG, MPI_COMM_WORLD);
        MPI_Recv(message, BUFSIZ, MPI_CHAR, anterior, TAG, MPI_COMM_WORLD, &status);
        printf("Proceso %d recibió de %d: %s\n", my_rank, anterior, message);
    }
    else {
        MPI_Recv(message, BUFSIZ, MPI_CHAR, anterior, TAG, MPI_COMM_WORLD, &status);
        printf("Proceso %d recibió de %d: %s\n", my_rank, anterior, message);
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, siguiente, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}