<h1 align="center">Práctica 3</h1>

## Pautas generales

- Para obtener el tiempo de ejecución de todos los algoritmos se debe utilizar la función provista por la cátedra (dwalltime).
- Por convención sólo deberá tomarse el tiempo de ejecución del procesamiento de datos. Esto significa excluir del tiempo de ejecución:
  - Reserva y liberación de memoria.
  - Inicialización de estructuras de datos.
  - Impresión y verificación de resultados.
  - Impresión en pantalla (printf).
- Las pruebas deben realizarse de forma aislada a la ejecución de otras aplicaciones. Se debe ejecutar desde consola, sin otras aplicaciones ejecutándose al mismo tiempo.
- Para todos los ejercicios se debe calcular el speedup y la eficiencia del algoritmo paralelo respecto al secuencial.

## Información útil para compilar y ejecutar

- Para compilar en Linux con gcc + OpenMP: `gcc -fopenmp -o salidaEjecutable archivoFuente.c`
- Para ejecutar: `./salidaEjecutable arg1 arg2 ... argN`

## 1. El programa ejercicio1.c inicializa una matriz de NxN de la siguiente manera: A[i, j] = i \* j, para todo i, j = 0..N-1. Compile y ejecute. ¿Qué problemas tiene el programa? Corríjalo.

- El programa **ejercicio1.c** recibe dos argumentos:
  - N (tamaño de la matriz)
  - T (cantidad de hilos).
- Luego, intenta inicializar una matriz en forma paralela usando OpenMP, haciendo que cada hilo inicialize una porción de la matriz.
- Cuando compilo **ejercicio1.c**, el compilador muestra una advertencia:

```bash
$ gcc -fopenmp -o ejercicio1 ejercicio1.c -Wall

ejercicio1.c: In function 'main._omp_fn.0':
ejercicio1.c:23:30: warning: 'i' may be used uninitialized [-Wmaybe-uninitialized]
  23 | A[i * N + j] = i * j;
     |
ejercicio1.c:7:9: note 'i' was declared here
   7 | int i, j;
     |
```

- Cuando intento ejecutar el programa ignorando la advertencia, se produce un error:

```bash
$ ./ejercicio1 16 2

Segmentation fault (core dumped)
```

- El problema que tiene el código es que la variable índice **i** se declara como privada de los hilos en la línea `#pragma omp parallel for shared(A) private (i, j)` pero dentro del bloque paralelo no se inicializa nunca i, por ende vale basura.
- Para solucionarlo, muevo la línea mencionada antes del for **externo** y no el interno.
- Código corregido en carpeta "Ejercicio 1".
- [Tiempos y análisis de rendimiento](https://docs.google.com/spreadsheets/d/1cv1V8cbV4wRPSSgeHD8KuhjBkWiOzVBkzNqe3Hzj1eg/edit?usp=sharing).

## 2. Analice y compile el programa ejercicio2.c. Ejecute varias veces y compare los resultados de salida para diferente número de threads. ¿Cuál es el problema? ¿Se le ocurre una solución? Nota: al compilar, agregue el flag -lm.

- El programa parece realizar una sumatoria de una ecuación, acumulando el resultado en la variable **x**.
- Compilo el programa:

```bash
$ gcc -fopenmp -o ejercicio2 ejercicio2.c -lm -Wall
```

- Resultados de ejecuciones para diferente número de threads pero mismo N (2):

```bash
./ejercicio2 2 1

7.359965

./ejercicio2 2 2

7.359965

./ejercicio2 2 4

8.741229

./ejercicio2 2 8

7.359965
```

- Se puede ver que el resultado a veces es erróneo cuando usamos más de un hilo.
- El problema de este programa es que tenemos una variable compartida **x** que no se está protegiendo adecuadamente, por lo cual los distintos hilos pueden fácilmente pisar el valor de esta variable entre ellos.
- No se me ocurre ninguna forma viable de solucionar el problema. Se podría usar un mutex para la línea `x = x + sqrt(i * scale) + 2 * x;` vía `#pragma omp critical`, pero mataría todo el paralelismo y la solución sería igual en performance que la secuencial.
  - Como el valor nuevo de x siempre depende del valor que tenía antes, hay demasiada dependencia como para que la solución se pueda paralelizar eficientemente.
- Solución alternativa (no eficiente) con mutex en carpeta "Ejercicio 2".
- [Tiempos y análisis de rendimiento](https://docs.google.com/spreadsheets/d/1cv1V8cbV4wRPSSgeHD8KuhjBkWiOzVBkzNqe3Hzj1eg/edit?usp=sharing).

## 3. El programa matrices.c realiza la multiplicación de 2 matrices cuadradas de NxN (C=AxB). Utilizando la directiva parallel for paralelice de dos formas:

### a. Repartiendo entre los threads el cálculo de las filas de C. Es decir, repartiendo el trabajo del primer for.

### b. Repartiendo el cálculo de las columnas de cada fila de C. Es decir, repartiendo el trabajo del segundo for.

### Compare los tiempos de ambas soluciones variando el número de threads.

- Código en carpeta "Ejercicio 3".
- Para repartir entre los threads el cálculo de las filas de C, uso `pragma omp parallel for private(j, k)` antes del primer for.
- Para repartir entre los threads el cálculo de las columnas de cada fila de C, uso `pragma omp parallel for private(j, k)` antes del segundo for.
- [Tiempos y análisis de rendimiento](https://docs.google.com/spreadsheets/d/1cv1V8cbV4wRPSSgeHD8KuhjBkWiOzVBkzNqe3Hzj1eg/edit?usp=sharing).

## 4. El programa traspuesta.c calcula la transpuesta de una matriz triangular de NxN. Compile y ejecute para 4 threads comparándolo con el algoritmo secuencial. Si bien el programa computa correctamente la transpuesta, éste tiene un problema desde el punto de vista del rendimiento. Analice las salidas y describa de qué problema se trata. ¿Qué cláusula se debe usar para corregir el problema? Describa brevemente la cláusula OpenMP que resuelve el problema y las opciones que tiene. Corrija el algoritmo y ejecute de nuevo comparando con los resultados anteriores.

## 5. El programa mxm.c realiza 2 multiplicaciones de matrices de MxM (D=AxB y E=CxB). Paralelizar utilizando sections de forma que cada una de las multiplicaciones se realice en una sección y almacenar el código paralelo como mxmSections.c. Compile y ejecute con 2 threads y luego con 4 threads, ¿se consigue mayor speedup al incrementar la cantidad de threads? ¿Por qué?
