#include "utils.h"
#include <stdio.h>

int chequear_resultados(double * A, double * B, double * B_T, double * C, double * R, double * a_por_b, double * c_por_bt, int N, double cociente) {
    int i, j;

    printf("Chequeando resultados...\n");
    // Chequear que A×B es correcto.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (a_por_b[i * N + j] != B[j * N + i]) {
                printf("Error en la multiplicación A×B en la posición [%d][%d]\n", i, j);
                return 1;
            }
        }
    }
    printf("Multiplicación A×B correcta ya que A×B = B.\n");

    // Chequear que C×B_T es correcto.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (c_por_bt[i * N + j] != B_T[j * N + i]) {
                printf("Error en la multiplicación C×B_T en la posición [%d][%d]\n", i, j);
                return 1;
            }
        }
    }
    printf("Multiplicación C×B_T correcta ya que C×B_T = B_T.\n");

    // Chequear que Cociente es correcto.
    double numerador = (1 * (N * N)) - (0 * 1);
    double denominador = (1.0 / N) * ((N * N + 1) / 2.0);
    double valor_esperado = numerador / denominador;
    double diferencia = (cociente - valor_esperado >= 0) ? (cociente - valor_esperado) : -(cociente - valor_esperado);
    if(diferencia > EPSILON) {
        printf("Error en el cálculo del cociente.\n");
        return 1;
    }
    printf("Cociente correcto.\n");

    // Chequear que Cociente × [A×B] es correcto.
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (a_por_b[i * N + j] * cociente != B[j * N + i] * cociente) {
                printf("Error en la multiplicación Cociente × [A×B] en la posición [%d][%d]\n", i, j);
                return 1;
            }
        }
    }
    printf("Multiplicación Cociente × [A×B] correcta ya que Cociente × [A×B] = B × Cociente.\n");

    // Chequear que Cociente × [A×B] + [C×B_T] es correcto.
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

void imprimir_matriz_por_fila(double *A, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", A[i * N + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void imprimir_matriz_por_columna(double *A, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", A[j * N + i]);
        }
        printf("\n");
    }
    printf("\n");
}