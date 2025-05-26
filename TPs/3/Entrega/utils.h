#ifndef UTILS_H
#define UTILS_H

#define EPSILON 1e-6

int chequear_resultados(double * A, double * B, double * B_T, double * C, double * R, double * a_por_b, double * c_por_bt, int N, double cociente);
void imprimir_matriz_por_fila(double *A, int N);
void imprimir_matriz_por_columna(double *A, int N);

#endif