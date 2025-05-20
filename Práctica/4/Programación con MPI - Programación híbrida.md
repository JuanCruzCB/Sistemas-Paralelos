<h1 align="center">Práctica 4</h1>

## Pautas generales

- Para obtener el tiempo de ejecución de todos los algoritmos se debe utilizar la función provista por la cátedra (dwalltime).
- Por convención sólo deberá tomarse el tiempo de ejecución del procesamiento y comunicación de datos (se recomienda medir ambos por separado). Esto significa excluir del tiempo de ejecución:
  - Reserva y liberación de memoria.
  - Inicialización de estructuras de datos.
  - Impresión y verificación de resultados.
  - Impresión en pantalla (printf).
- Las pruebas deben realizarse de forma aislada a la ejecución de otras aplicaciones. Se debe ejecutar desde consola, sin otras aplicaciones ejecutándose al mismo tiempo.
- Además del algoritmo paralelo, debe implementar el algoritmo secuencial en el caso que corresponda.
- Los ejercicios 4 - 6 deben probarse en las siguientes modalidades:
  1. Usando 1 único nodo con 1 proceso por núcleo.
  2. Usando 2 nodos con 1 proceso cada 2 núcleos.
  3. Usando 2 nodos con 1 proceso por núcleo.
  - Por ejemplo, si el cluster dispone de nodos quad-core, entonces debe generar 4 procesos para el caso i), 4 para el caso ii) y 8 para el caso iii).
- Para todos los ejercicios 4 - 7 se debe calcular el speedup y la eficiencia del algoritmo paralelo respecto al secuencial. Además, realice un análisis de escalabilidad y del overhead de las comunicaciones.

## Información útil para compilar y ejecutar:

- Para compilar con OpenMPI, abra una consola y use **mpicc** empleando la siguiente sintaxis: `mpicc archivofuente.c –o nombreBinario`
- Para compilar con OpenMPI + OpenMP, abra una consola y use mpicc empleando la siguiente sintaxis: `mpicc archivofuente.c –o nombreBinario -fopenmp`
- Para ejecutar un binario en una máquina local, emplee la siguiente sintaxis: `mpirun –np P ./nombreBinario arg1 arg2 ... argN` donde P representa el número de procesos a generar.
- Para ejecutar en el cluster de la cátedra, siga las instrucciones detalladas en el instructivo.

## 1. Revisar el código mpi-simple.c. Compile y ejecute el código. Modifíquelo para que los procesos se comuniquen en forma de anillo: el proceso i debe enviarle un mensaje al proceso i + 1 , a excepción del último que debe comunicarse con el 0.

El código **mpi-simple.c** funciona así:

- Cada proceso, excepto el maestro, envía (MPI_Send) un mensaje de saludo al proceso master, el cual está dentro del canal MPI_COMM_WORLD, indicando su rank (id).
- El proceso maestro, que es el proceso cuyo rank es 0, recibe (MPI_Recv) los mensajes de todos los otros procesos haciendo un receive en el id del proceso, el cual está dentro del canal MPI_COMM_WORLD nuevamente.
- Este proceso maestro, luego de recibir cada mensaje, lo imprime.
- El tipo de comunicación es punto a punto y se comporta como bloqueante en este contexto.

Compilo el programa con `mpicc -o ej1 mpi-simple.c`, lo ejecuto con `mpirun -np 2 ./ej1` y veo el siguiente output:

```
Mensajes recibido por el proceso 0 (master):
Mensaje recibido: Hola Mundo! Soy el proceso 1!
```

El código modificado con comunicación en forma de anillo se encuentra en la carpeta "Ejercicio 1".

## 2. Los códigos blocking.c y non-blocking.c siguen el patrón master-worker, donde los procesos worker le envían un mensaje de texto al master empleando operaciones de comunicación bloqueantes y no bloqueantes, respectivamente.

### Compile y ejecute ambos códigos usando P = {4, 8, 16} (no importa que el número de núcleos sea menor que la cantidad de procesos). ¿Cuál de los dos retorna antes el control?

- Compilo con: `mpicc -o blocking blocking.c`; `mpicc -o non-blocking non-blocking.c`
- Ejecuto con `mpirun --oversubscribe -np P ./programa`, uso --oversubscribe porque P > cantidad de cores.
- **Resultados de ejecución blocking**:

  - P = 4:

  ```
  Tiempo transcurrido 0.000002 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 1)
  Tiempo transcurrido 2.000301 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 1
  Tiempo transcurrido 2.000324 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 2)
  Tiempo transcurrido 4.000049 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 2
  Tiempo transcurrido 4.000075 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 3)
  Tiempo transcurrido 6.000082 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 3

  Tiempo total = 0.000000 (s)
  ```

  - P = 8:

  ```
  Tiempo transcurrido 0.000001 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 1)
  Tiempo transcurrido 2.000370 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 1
  Tiempo transcurrido 2.000394 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 2)
  Tiempo transcurrido 3.999977 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 2
  Tiempo transcurrido 4.000087 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 3)
  Tiempo transcurrido 6.000404 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 3
  Tiempo transcurrido 6.000436 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 4)
  Tiempo transcurrido 8.000167 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 4
  Tiempo transcurrido 8.000198 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 5)
  Tiempo transcurrido 10.000757 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 5
  Tiempo transcurrido 10.000782 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 6)
  Tiempo transcurrido 12.000491 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 6
  Tiempo transcurrido 12.000521 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 7)
  Tiempo transcurrido 14.000308 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 7

  Tiempo total = 0.000000 (s)
  ```

  - P = 16:

  ```
  Tiempo transcurrido 0.000002 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 1)
  Tiempo transcurrido 1.999977 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 1
  Tiempo transcurrido 1.999999 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 2)
  Tiempo transcurrido 3.999243 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 2
  Tiempo transcurrido 3.999274 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 3)
  Tiempo transcurrido 6.000519 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 3
  Tiempo transcurrido 6.000544 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 4)
  Tiempo transcurrido 8.000713 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 4
  Tiempo transcurrido 8.000733 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 5)
  Tiempo transcurrido 9.999863 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 5
  Tiempo transcurrido 9.999889 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 6)
  Tiempo transcurrido 11.999359 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 6
  Tiempo transcurrido 11.999387 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 7)
  Tiempo transcurrido 14.000396 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 7
  Tiempo transcurrido 14.000421 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 8)
  Tiempo transcurrido 16.000368 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 8
  Tiempo transcurrido 16.000390 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 9)
  Tiempo transcurrido 17.999948 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 9
  Tiempo transcurrido 17.999976 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 10)
  Tiempo transcurrido 19.999284 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 10
  Tiempo transcurrido 19.999311 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 11)
  Tiempo transcurrido 22.002684 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 11
  Tiempo transcurrido 22.002716 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 12)
  Tiempo transcurrido 24.004519 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 12
  Tiempo transcurrido 24.004550 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 13)
  Tiempo transcurrido 25.999697 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 13
  Tiempo transcurrido 25.999725 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 14)
  Tiempo transcurrido 28.004668 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 14
  Tiempo transcurrido 28.004697 (s):	proceso 0, llamando a MPI_Recv() [bloqueante] (fuente rank 15)
  Tiempo transcurrido 30.004313 (s):	proceso 0, MPI_Recv() devolvio control con mensaje: Hola Mundo! Soy el proceso 15

  Tiempo total = 0.000000 (s)
  ```

- **Resultados de ejecución non-blocking**:

  - P = 4:

  ```
  Tiempo transcurrido 0.000002 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 1)
  Tiempo transcurrido 0.000405 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 2.000148 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 1
  Tiempo transcurrido 2.000171 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 2)
  Tiempo transcurrido 2.000338 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 4.000219 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 2
  Tiempo transcurrido 4.000243 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 3)
  Tiempo transcurrido 4.000251 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 6.000454 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 3

  Tiempo total = 0.000000 (s)
  ```

  - P = 8:

  ```
  Tiempo transcurrido 0.000003 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 1)
  Tiempo transcurrido 0.000048 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 2.000041 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 1
  Tiempo transcurrido 2.000064 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 2)
  Tiempo transcurrido 2.000071 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 4.000523 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 2
  Tiempo transcurrido 4.000546 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 3)
  Tiempo transcurrido 4.000651 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 6.000608 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 3
  Tiempo transcurrido 6.000633 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 4)
  Tiempo transcurrido 6.000646 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 7.999872 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 4
  Tiempo transcurrido 7.999986 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 5)
  Tiempo transcurrido 7.999997 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 10.000350 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 5
  Tiempo transcurrido 10.000376 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 6)
  Tiempo transcurrido 10.000387 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 12.000290 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 6
  Tiempo transcurrido 12.000316 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 7)
  Tiempo transcurrido 12.000325 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 14.000108 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 7

  Tiempo total = 0.000000 (s)
  ```

  - P = 16:

  ```
  Tiempo transcurrido 0.000001 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 1)
  Tiempo transcurrido 0.000062 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 2.000794 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 1
  Tiempo transcurrido 2.000820 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 2)
  Tiempo transcurrido 2.000828 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 3.999979 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 2
  Tiempo transcurrido 4.000006 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 3)
  Tiempo transcurrido 4.000014 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 6.001143 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 3
  Tiempo transcurrido 6.001170 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 4)
  Tiempo transcurrido 6.001190 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 8.000338 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 4
  Tiempo transcurrido 8.000366 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 5)
  Tiempo transcurrido 8.000374 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 10.000747 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 5
  Tiempo transcurrido 10.000771 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 6)
  Tiempo transcurrido 10.000782 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 12.000951 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 6
  Tiempo transcurrido 12.000978 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 7)
  Tiempo transcurrido 12.000986 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 14.000786 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 7
  Tiempo transcurrido 14.000813 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 8)
  Tiempo transcurrido 14.000822 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 16.000592 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 8
  Tiempo transcurrido 16.000615 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 9)
  Tiempo transcurrido 16.000626 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 18.000679 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 9
  Tiempo transcurrido 18.000709 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 10)
  Tiempo transcurrido 18.000721 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 20.001049 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 10
  Tiempo transcurrido 20.001076 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 11)
  Tiempo transcurrido 20.001086 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 22.000922 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 11
  Tiempo transcurrido 22.000948 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 12)
  Tiempo transcurrido 22.000957 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 24.001239 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 12
  Tiempo transcurrido 24.001267 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 13)
  Tiempo transcurrido 24.001276 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 26.000055 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 13
  Tiempo transcurrido 26.000086 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 14)
  Tiempo transcurrido 26.000095 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 28.000912 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 14
  Tiempo transcurrido 28.000938 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 15)
  Tiempo transcurrido 28.000947 (s):	proceso 0, MPI_IRecv() devolvio el control..
            ..pero el mensaje no fue aun recibido..
  Tiempo transcurrido 29.999850 (s):	proceso 0, operacion receive completa con mensaje: Hola Mundo! Soy el proceso 15

  Tiempo total = 0.000000 (s)
  ```

La versión **no bloqueante (non-blocking.c) retorna el control al programa antes que la bloqueante (blocking.c)**, ya que las funciones `MPI_Isend` y `MPI_Irecv` no esperan a que la comunicación se complete y permiten continuar la ejecución. Sin embargo, para usar los datos correctamente, luego se debe llamar a `MPI_Wait`, que sí espera (como en el caso bloqueante).

### En el caso de la versión no bloqueante, ¿qué sucede si se elimina la operación `MPI_Wait()` (línea 52)? ¿Se imprimen correctamente los mensajes enviados? ¿Por qué?

Si elimino esa línea y ejecuto con P = 4, obtengo:

```
Tiempo transcurrido 0.000001 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 1)
Tiempo transcurrido 0.000039 (s):	proceso 0, MPI_IRecv() devolvio el control..
          ..pero el mensaje no fue aun recibido..
Tiempo transcurrido 0.000050 (s):	proceso 0, operacion receive completa con mensaje: No deberia estar leyendo esta frase.
Tiempo transcurrido 0.000144 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 2)
Tiempo transcurrido 0.000150 (s):	proceso 0, MPI_IRecv() devolvio el control..
          ..pero el mensaje no fue aun recibido..
Tiempo transcurrido 0.000157 (s):	proceso 0, operacion receive completa con mensaje: No deberia estar leyendo esta frase.
Tiempo transcurrido 0.000161 (s):	proceso 0, llamando a MPI_IRecv() [no bloqueante] (fuente rank 3)
Tiempo transcurrido 0.000166 (s):	proceso 0, MPI_IRecv() devolvio el control..
          ..pero el mensaje no fue aun recibido..
Tiempo transcurrido 0.000171 (s):	proceso 0, operacion receive completa con mensaje: No deberia estar leyendo esta frase.

Tiempo total = 0.000000 (s)
```

Si se elimina `MPI_Wait`, los mensajes no se reciben correctamente porque la operación no bloqueante `MPI_Irecv` **no espera** a que los datos lleguen. En su lugar, se imprime el contenido inicial del buffer (basura), ya que MPI **no garantiza que el buffer sea válido hasta que se complete la operación con `MPI_Wait`**"

El punto clave es que la ventaja de `MPI_Irecv` **no es evitar la espera, sino permitir hacer trabajo útil mientras se reciben los datos**.

## 3. Los códigos blocking-ring.c y non-blocking-ring.c comunican a los procesos en forma de anillo empleando operaciones bloqueantes y no bloqueantes, respectivamente. Compile y ejecute ambos códigos empleando P = {4, 8, 16} (no importa que el número de núcleos sea menor que la cantidad de procesos) y N = {10000000, 20000000, 40000000, ...}. ¿Cuál de los dos algoritmos requiere menos tiempo de comunicación? ¿Por qué?

### Nota: Para el caso de P = 16, agregue la línea `--overcommit` al script de SLURM y el flag `--oversubscribe` al comando mpirun.

## 4. El algoritmo mpi_matmul.c computa una multiplicación de matrices cuadradas empleando comunicaciones punto a punto:

### Compile y ejecute el código empleando N = {512, 1024, 2048} usando todos los núcleos de 1 y 2 nodos.

### Revise las secciones de código donde se realiza la comunicación de las matrices. Analice el patrón de comunicación y piense si es posible emplear comunicaciones colectivas en lugar de a punto a punto. En ese caso, modifique el código original, compile y ejecute la nueva versión. ¿Se mejora la legibilidad? ¿Se logra mejorar el rendimiento? ¿Por qué?

## 5. Desarrolle un algoritmo paralelo que resuelva la expresión R = AB + CD + EF, donde A, B, C, D, E y F son matrices cuadradas de NxN. Ejecute para N = {512, 1024, 2048} con P = {2, 4, 8, 16}.

## 6. Desarrolle un algoritmo paralelo que dado un vector V de tamaño N obtenga el valor máximo, el valor mínimo y valor promedio de sus elementos. Ejecute para P = {2, 4, 8, 16} variando el valor de N.

## 7. Desarrolle una versión híbrida (MPI + OpenMP) de la multiplicación de matrices. Replique el análisis realizado para el algoritmo puro MPI (ejercicio 4) y compare sus rendimientos. ¿Cuál es mejor? ¿Por qué?
