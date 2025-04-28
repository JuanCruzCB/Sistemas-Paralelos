#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>
#include<sys/time.h>

double dwalltime() {
    double sec;
    struct timeval tv;
    gettimeofday( & tv, NULL);
    sec = tv.tv_sec + tv.tv_usec / 1000000.0;
    return sec;
}

int main(int argc, char * argv[]) {
    double x, scale, timetick;
    int i;
    int numThreads = atoi(argv[2]);
    int N = atoi(argv[1]);
    omp_set_num_threads(numThreads);
    scale = 2.78;
    x = 0.0;

    timetick = dwalltime();
    #pragma omp parallel for
    for (i = 1; i <= N; i++) {
        #pragma omp critical
        x = x + sqrt(i * scale) + 2 * x;
    }

    printf("Tiempo de ejecución: %f segundos\n", dwalltime() - timetick);
    printf("\n Resultado: %f \n", x);

    return (0);
}
