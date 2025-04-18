<h1 align="center">Práctica 2</h1>

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

- Para compilar en Linux con gcc + Pthreads: `gcc –pthread -o salidaEjecutable archivoFuente.c`
- Para ejecutar: `./salidaEjecutable arg1 arg2 ... argN`

## 1. Desarrolle un algoritmo paralelo que compute una suma de vectores: 𝐴𝑖 = 𝐵𝑖 + 𝐶𝑖. Para ello, considere lo siguiente:

### - Identifique las regiones de código que pueden ejecutarse en paralelo y las que no. ¿Existen dependencias?

### - Analice cómo distribuir el trabajo entre los hilos. ¿El cómputo es regular? ¿La arquitectura es homogénea?

### Mida el tiempo de ejecución para diferentes valores de N y T = {2, 4, 8}. Analice el rendimiento.

- Código en carpeta "Ejercicio 1".
- El cómputo **es regular**, ya que se conoce la cantidad de iteraciones del for para cada hilo. La arquitectura es homogénea, ya que cada core es igual en mi máquina.
- [Tiempos y análisis de rendimiento](https://docs.google.com/spreadsheets/d/1cv1V8cbV4wRPSSgeHD8KuhjBkWiOzVBkzNqe3Hzj1eg/edit?usp=sharing).

## 2. Desarrolle un algoritmo paralelo que compute la multiplicación de matrices cuadradas de NxN. Primero, considere a la versión optimizada del ejercicio 6 de la práctica anterior como algoritmo base. Luego, paralelice la versión que computa por bloques. Mida el tiempo de ejecución para N = {512, 1024, 2048, 4096} y T = {2, 4, 8}. Analice el rendimiento.

- Código en carpeta "Ejercicio 2".
- Para este ejercicio usaré tamaño de bloque (BS) = 128, ya que es el que mejor resultados me dio en el ejercicio 6 de la práctica 1.
- [Tiempos y análisis de rendimiento](https://docs.google.com/spreadsheets/d/1cv1V8cbV4wRPSSgeHD8KuhjBkWiOzVBkzNqe3Hzj1eg/edit?usp=sharing).

## 3. Desarrolle un algoritmo paralelo que cuente la cantidad de veces que un elemento X aparece dentro de un vector de N elementos enteros. Al finalizar, la cantidad de ocurrencias del elemento X debe quedar en una variable llamada ocurrencias. Para la sincronización emplee mutex-locks. Pruebe con diversos tamaños de N y T = {2, 4, 8}. Analice el rendimiento.

- Código en carpeta "Ejercicio 3".
- Uso el mutex lock una vez que cada hilo termina de chequear su parte del vector y no cada vez que chequean cada posición de su porción del vector, ya que no hace falta.
- [Tiempos y análisis de rendimiento](https://docs.google.com/spreadsheets/d/1cv1V8cbV4wRPSSgeHD8KuhjBkWiOzVBkzNqe3Hzj1eg/edit?usp=sharing).

## 4. Desarrolle un algoritmo paralelo que calcule el valor promedio, mínimo y máximo de los números almacenados en un vector de tamaño N. Para la sincronización emplee semáforos. Pruebe con diversos tamaños de N y T = {2, 4, 8}. Analice el rendimiento.

- Código en carpeta "Ejercicio 4".
- ???
- [Tiempos y análisis de rendimiento](https://docs.google.com/spreadsheets/d/1cv1V8cbV4wRPSSgeHD8KuhjBkWiOzVBkzNqe3Hzj1eg/edit?usp=sharing).
