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

## 1. Revisar el código `mpi-simple.c`. Compile y ejecute el código. Modifíquelo para que los procesos se comuniquen en forma de anillo: el proceso i debe enviarle un mensaje al proceso i + 1 , a excepción del último que debe comunicarse con el 0.

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

## 2. Los códigos `blocking.c` y `non-blocking.c` siguen el patrón master-worker, donde los procesos worker le envían un mensaje de texto al master empleando operaciones de comunicación bloqueantes y no bloqueantes, respectivamente.

### Compile y ejecute ambos códigos usando P = {4, 8, 16} (no importa que el número de núcleos sea menor que la cantidad de procesos). ¿Cuál de los dos retorna antes el control?

- Compilo con `mpicc -o blocking blocking.c`; `mpicc -o non-blocking non-blocking.c`
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

## 3. Los códigos `blocking-ring.c` y `non-blocking-ring.c` comunican a los procesos en forma de anillo empleando operaciones bloqueantes y no bloqueantes, respectivamente. Compile y ejecute ambos códigos empleando P = {4, 8, 16} (no importa que el número de núcleos sea menor que la cantidad de procesos) y N = {10000000, 20000000, 40000000, ...}. ¿Cuál de los dos algoritmos requiere menos tiempo de comunicación? ¿Por qué?

### Nota: Para el caso de P = 16, agregue la línea `--overcommit` al script de SLURM y el flag `--oversubscribe` al comando mpirun.

- Compilo con `mpicc -o blocking blocking-ring.c`; `mpicc -o non-blocking non-blocking-ring.c`
- Ejecuto con `mpirun --oversubscribe -np P ./programa N`, uso --oversubscribe porque P > cantidad de cores.
- **Resultados de ejecución blocking**:

  - P = 4, N = 10000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto bloqueante: MPI_Send, MPI_Recv

  Dimension del vector: 10000000
  Numero de procesos: 4

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 5.001990e+06
  Proceso 1 : Suma del vector enviado a 2 = 5.001024e+06
  Proceso 2 : Suma del vector enviado a 3 = 4.999329e+06
  Proceso 3 : Suma del vector enviado a 0 = 4.999591e+06

  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 4.999591e+06 : Tiempo=0.111965 segundos
  Proceso 1 : Suma del vector recibido = 5.001990e+06 : Tiempo=0.228316 segundos
  Proceso 2 : Suma del vector recibido = 5.001024e+06 : Tiempo=0.315251 segundos
  Proceso 3 : Suma del vector recibido = 4.999329e+06 : Tiempo=0.532021 segundos

  ##########################################################

  Tiempo de comunicacion : 0.380921 seconds

  ##########################################################
  ```

  - P = 4, N = 20000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto bloqueante: MPI_Send, MPI_Recv

  Dimension del vector: 20000000
  Numero de procesos: 4

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 1.000000e+07
  Proceso 1 : Suma del vector enviado a 2 = 9.997586e+06
  Proceso 2 : Suma del vector enviado a 3 = 9.999314e+06
  Proceso 3 : Suma del vector enviado a 0 = 9.999061e+06

  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 9.999061e+06 : Tiempo=0.240321 segundos
  Proceso 1 : Suma del vector recibido = 1.000000e+07 : Tiempo=0.472248 segundos
  Proceso 2 : Suma del vector recibido = 9.997586e+06 : Tiempo=0.684556 segundos
  Proceso 3 : Suma del vector recibido = 9.999314e+06 : Tiempo=0.883357 segundos

  ##########################################################

  Tiempo de comunicacion : 0.874133 seconds

  ##########################################################
  ```

  - P = 4, N = 40000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto bloqueante: MPI_Send, MPI_Recv

  Dimension del vector: 40000000
  Numero de procesos: 4

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 2.000202e+07
  Proceso 1 : Suma del vector enviado a 2 = 2.000221e+07
  Proceso 2 : Suma del vector enviado a 3 = 2.000079e+07
  Proceso 3 : Suma del vector enviado a 0 = 2.000075e+07

  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 2.000075e+07 : Tiempo=0.970481 segundos
  Proceso 1 : Suma del vector recibido = 2.000202e+07 : Tiempo=1.548100 segundos
  Proceso 2 : Suma del vector recibido = 2.000221e+07 : Tiempo=2.486647 segundos
  Proceso 3 : Suma del vector recibido = 2.000079e+07 : Tiempo=3.279802 segundos

  ##########################################################

  Tiempo de comunicacion : 2.890444 seconds

  ##########################################################
  ```

  - P = 8, N = 10000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto bloqueante: MPI_Send, MPI_Recv

  Dimension del vector: 10000000
  Numero de procesos: 8

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 5.000372e+06
  Proceso 1 : Suma del vector enviado a 2 = 4.999888e+06
  Proceso 2 : Suma del vector enviado a 3 = 4.999789e+06
  Proceso 3 : Suma del vector enviado a 4 = 5.000632e+06
  Proceso 4 : Suma del vector enviado a 5 = 5.000102e+06
  Proceso 5 : Suma del vector enviado a 6 = 4.999951e+06
  Proceso 6 : Suma del vector enviado a 7 = 4.998257e+06
  Proceso 7 : Suma del vector enviado a 0 = 4.999970e+06

  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 4.999970e+06 : Tiempo=0.239942 segundos
  Proceso 1 : Suma del vector recibido = 5.000372e+06 : Tiempo=0.483928 segundos
  Proceso 2 : Suma del vector recibido = 4.999888e+06 : Tiempo=0.531379 segundos
  Proceso 3 : Suma del vector recibido = 4.999789e+06 : Tiempo=0.721512 segundos
  Proceso 4 : Suma del vector recibido = 5.000632e+06 : Tiempo=0.908447 segundos
  Proceso 5 : Suma del vector recibido = 5.000102e+06 : Tiempo=1.165117 segundos
  Proceso 6 : Suma del vector recibido = 4.999951e+06 : Tiempo=1.202501 segundos
  Proceso 7 : Suma del vector recibido = 4.998257e+06 : Tiempo=1.402758 segundos

  ##########################################################

  Tiempo de comunicacion : 1.540633 seconds

  ##########################################################
  ```

  - P = 8, N = 20000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto bloqueante: MPI_Send, MPI_Recv

  Dimension del vector: 20000000
  Numero de procesos: 8

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 1.000020e+07
  Proceso 1 : Suma del vector enviado a 2 = 9.997027e+06
  Proceso 2 : Suma del vector enviado a 3 = 1.000089e+07
  Proceso 3 : Suma del vector enviado a 4 = 1.000123e+07
  Proceso 4 : Suma del vector enviado a 5 = 1.000005e+07
  Proceso 5 : Suma del vector enviado a 6 = 1.000151e+07
  Proceso 6 : Suma del vector enviado a 7 = 1.000130e+07
  Proceso 7 : Suma del vector enviado a 0 = 1.000002e+07

  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 1.000002e+07 : Tiempo=0.493593 segundos
  Proceso 1 : Suma del vector recibido = 1.000020e+07 : Tiempo=0.949825 segundos
  Proceso 2 : Suma del vector recibido = 9.997027e+06 : Tiempo=1.258591 segundos
  Proceso 3 : Suma del vector recibido = 1.000089e+07 : Tiempo=1.515195 segundos
  Proceso 4 : Suma del vector recibido = 1.000123e+07 : Tiempo=1.767769 segundos
  Proceso 5 : Suma del vector recibido = 1.000005e+07 : Tiempo=2.235169 segundos
  Proceso 6 : Suma del vector recibido = 1.000151e+07 : Tiempo=2.523771 segundos
  Proceso 7 : Suma del vector recibido = 1.000130e+07 : Tiempo=2.832238 segundos

  ##########################################################

  Tiempo de comunicacion : 2.875753 seconds

  ##########################################################
  ```

  - P = 8, N = 40000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto bloqueante: MPI_Send, MPI_Recv

  Dimension del vector: 40000000
  Numero de procesos: 8

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 1.999933e+07
  Proceso 1 : Suma del vector enviado a 2 = 2.000215e+07
  Proceso 2 : Suma del vector enviado a 3 = 2.000023e+07
  Proceso 3 : Suma del vector enviado a 4 = 2.000321e+07
  Proceso 4 : Suma del vector enviado a 5 = 2.000019e+07
  Proceso 5 : Suma del vector enviado a 6 = 1.999978e+07
  Proceso 6 : Suma del vector enviado a 7 = 2.000112e+07
  Proceso 7 : Suma del vector enviado a 0 = 2.000090e+07

  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 2.000090e+07 : Tiempo=0.946368 segundos
  Proceso 1 : Suma del vector recibido = 1.999933e+07 : Tiempo=1.910579 segundos
  Proceso 2 : Suma del vector recibido = 2.000215e+07 : Tiempo=2.765198 segundos
  Proceso 3 : Suma del vector recibido = 2.000023e+07 : Tiempo=3.636005 segundos
  Proceso 4 : Suma del vector recibido = 2.000321e+07 : Tiempo=4.453930 segundos
  Proceso 5 : Suma del vector recibido = 2.000019e+07 : Tiempo=5.480916 segundos
  Proceso 6 : Suma del vector recibido = 1.999978e+07 : Tiempo=6.440925 segundos
  Proceso 7 : Suma del vector recibido = 2.000112e+07 : Tiempo=10.089649 segundos

  ##########################################################

  Tiempo de comunicacion : 9.912438 seconds

  ##########################################################
  ```

  - P = 16, N = 10000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto bloqueante: MPI_Send, MPI_Recv

  Dimension del vector: 10000000
  Numero de procesos: 16

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 5.001012e+06
  Proceso 1 : Suma del vector enviado a 2 = 4.999919e+06
  Proceso 2 : Suma del vector enviado a 3 = 5.000933e+06
  Proceso 3 : Suma del vector enviado a 4 = 4.999743e+06
  Proceso 4 : Suma del vector enviado a 5 = 5.001675e+06
  Proceso 5 : Suma del vector enviado a 6 = 5.000329e+06
  Proceso 6 : Suma del vector enviado a 7 = 4.999982e+06
  Proceso 7 : Suma del vector enviado a 8 = 5.000009e+06
  Proceso 8 : Suma del vector enviado a 9 = 5.001624e+06
  Proceso 9 : Suma del vector enviado a 10 = 5.000967e+06
  Proceso 10 : Suma del vector enviado a 11 = 4.999103e+06
  Proceso 11 : Suma del vector enviado a 12 = 5.000943e+06
  Proceso 12 : Suma del vector enviado a 13 = 5.000667e+06
  Proceso 13 : Suma del vector enviado a 14 = 4.999420e+06
  Proceso 14 : Suma del vector enviado a 15 = 5.000212e+06
  Proceso 15 : Suma del vector enviado a 0 = 5.001246e+06

  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 5.001246e+06 : Tiempo=0.485780 segundos
  Proceso 1 : Suma del vector recibido = 5.001012e+06 : Tiempo=0.997863 segundos
  Proceso 2 : Suma del vector recibido = 4.999919e+06 : Tiempo=1.020523 segundos
  Proceso 3 : Suma del vector recibido = 5.000933e+06 : Tiempo=1.106391 segundos
  Proceso 4 : Suma del vector recibido = 4.999743e+06 : Tiempo=1.260797 segundos
  Proceso 5 : Suma del vector recibido = 5.001675e+06 : Tiempo=1.325176 segundos
  Proceso 6 : Suma del vector recibido = 5.000329e+06 : Tiempo=1.759931 segundos
  Proceso 7 : Suma del vector recibido = 4.999982e+06 : Tiempo=1.976243 segundos
  Proceso 8 : Suma del vector recibido = 5.000009e+06 : Tiempo=2.342805 segundos
  Proceso 9 : Suma del vector recibido = 5.001624e+06 : Tiempo=2.547353 segundos
  Proceso 10 : Suma del vector recibido = 5.000967e+06 : Tiempo=2.664158 segundos
  Proceso 11 : Suma del vector recibido = 4.999103e+06 : Tiempo=3.206066 segundos
  Proceso 12 : Suma del vector recibido = 5.000943e+06 : Tiempo=3.208927 segundos
  Proceso 13 : Suma del vector recibido = 5.000667e+06 : Tiempo=3.401447 segundos
  Proceso 14 : Suma del vector recibido = 4.999420e+06 : Tiempo=3.616208 segundos
  Proceso 15 : Suma del vector recibido = 5.000212e+06 : Tiempo=4.105224 segundos

  ##########################################################

  Tiempo de comunicacion : 4.056052 seconds

  ##########################################################
  ```

  - P = 16, N = 20000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto bloqueante: MPI_Send, MPI_Recv

  Dimension del vector: 20000000
  Numero de procesos: 16

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 9.999180e+06
  Proceso 1 : Suma del vector enviado a 2 = 9.997454e+06
  Proceso 2 : Suma del vector enviado a 3 = 1.000323e+07
  Proceso 3 : Suma del vector enviado a 4 = 1.000051e+07
  Proceso 4 : Suma del vector enviado a 5 = 9.999209e+06
  Proceso 5 : Suma del vector enviado a 6 = 9.999670e+06
  Proceso 6 : Suma del vector enviado a 7 = 9.997444e+06
  Proceso 7 : Suma del vector enviado a 8 = 9.999219e+06
  Proceso 8 : Suma del vector enviado a 9 = 1.000027e+07
  Proceso 9 : Suma del vector enviado a 10 = 9.998839e+06
  Proceso 10 : Suma del vector enviado a 11 = 1.000146e+07
  Proceso 11 : Suma del vector enviado a 12 = 9.999262e+06
  Proceso 12 : Suma del vector enviado a 13 = 1.000205e+07
  Proceso 13 : Suma del vector enviado a 14 = 1.000224e+07
  Proceso 14 : Suma del vector enviado a 15 = 1.000232e+07
  Proceso 15 : Suma del vector enviado a 0 = 1.000074e+07

  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 1.000074e+07 : Tiempo=0.558219 segundos
  Proceso 1 : Suma del vector recibido = 9.999180e+06 : Tiempo=1.563507 segundos
  Proceso 2 : Suma del vector recibido = 9.997454e+06 : Tiempo=2.379732 segundos
  Proceso 3 : Suma del vector recibido = 1.000323e+07 : Tiempo=2.994512 segundos
  Proceso 4 : Suma del vector recibido = 1.000051e+07 : Tiempo=3.791136 segundos
  Proceso 5 : Suma del vector recibido = 9.999209e+06 : Tiempo=4.262406 segundos
  Proceso 6 : Suma del vector recibido = 9.999670e+06 : Tiempo=4.966584 segundos
  Proceso 7 : Suma del vector recibido = 9.997444e+06 : Tiempo=5.609398 segundos
  Proceso 8 : Suma del vector recibido = 9.999219e+06 : Tiempo=6.123480 segundos
  Proceso 9 : Suma del vector recibido = 1.000027e+07 : Tiempo=6.519926 segundos
  Proceso 10 : Suma del vector recibido = 9.998839e+06 : Tiempo=7.447812 segundos
  Proceso 11 : Suma del vector recibido = 1.000146e+07 : Tiempo=8.097953 segundos
  Proceso 12 : Suma del vector recibido = 9.999262e+06 : Tiempo=8.935675 segundos
  Proceso 13 : Suma del vector recibido = 1.000205e+07 : Tiempo=9.561219 segundos
  Proceso 14 : Suma del vector recibido = 1.000224e+07 : Tiempo=10.347883 segundos
  Proceso 15 : Suma del vector recibido = 1.000232e+07 : Tiempo=11.231180 segundos

  ##########################################################

  Tiempo de comunicacion : 11.280881 seconds

  ##########################################################
  ```

  - P = 16, N = 40000000: Tiró error.

- **Resultados de ejecución non-blocking**:

  - P = 4, N = 10000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto no bloqueante: MPI_Isend, MPI_Irecv

  Dimension del vector: 10000000
  Numero de procesos: 4

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 5.000138e+06
  Proceso 1 : Suma del vector enviado a 2 = 5.000349e+06
  Proceso 2 : Suma del vector enviado a 3 = 5.000111e+06
  Proceso 3 : Suma del vector enviado a 0 = 4.998548e+06
  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 4.998548e+06 : Tiempo=0.170355 segundos
  Proceso 1 : Suma del vector recibido = 5.000138e+06 : Tiempo=0.000018 segundos
  Proceso 2 : Suma del vector recibido = 5.000349e+06 : Tiempo=0.142692 segundos
  Proceso 3 : Suma del vector recibido = 5.000111e+06 : Tiempo=0.000020 segundos

  ##########################################################

  Tiempo de comunicacion : 0.170362 seconds

  ##########################################################
  ```

  - P = 4, N = 20000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto no bloqueante: MPI_Isend, MPI_Irecv

  Dimension del vector: 20000000
  Numero de procesos: 4

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 9.999550e+06
  Proceso 1 : Suma del vector enviado a 2 = 9.998950e+06
  Proceso 2 : Suma del vector enviado a 3 = 1.000163e+07
  Proceso 3 : Suma del vector enviado a 0 = 9.997980e+06
  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 9.997980e+06 : Tiempo=0.262146 segundos
  Proceso 1 : Suma del vector recibido = 9.999550e+06 : Tiempo=0.000016 segundos
  Proceso 2 : Suma del vector recibido = 9.998950e+06 : Tiempo=0.000010 segundos
  Proceso 3 : Suma del vector recibido = 1.000163e+07 : Tiempo=0.000018 segundos

  ##########################################################

  Tiempo de comunicacion : 0.262153 seconds

  ##########################################################
  ```

  - P = 4, N = 40000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto no bloqueante: MPI_Isend, MPI_Irecv

  Dimension del vector: 40000000
  Numero de procesos: 4

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 1.999850e+07
  Proceso 1 : Suma del vector enviado a 2 = 1.999655e+07
  Proceso 2 : Suma del vector enviado a 3 = 1.999877e+07
  Proceso 3 : Suma del vector enviado a 0 = 2.000114e+07
  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 2.000114e+07 : Tiempo=0.572240 segundos
  Proceso 1 : Suma del vector recibido = 1.999850e+07 : Tiempo=0.000017 segundos
  Proceso 2 : Suma del vector recibido = 1.999655e+07 : Tiempo=0.587965 segundos
  Proceso 3 : Suma del vector recibido = 1.999877e+07 : Tiempo=0.000019 segundos

  ##########################################################

  Tiempo de comunicacion : 0.572247 seconds

  ##########################################################
  ```

  - P = 8, N = 10000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto no bloqueante: MPI_Isend, MPI_Irecv

  Dimension del vector: 10000000
  Numero de procesos: 8

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 4.999254e+06
  Proceso 1 : Suma del vector enviado a 2 = 4.999467e+06
  Proceso 2 : Suma del vector enviado a 3 = 4.998405e+06
  Proceso 3 : Suma del vector enviado a 4 = 5.001335e+06
  Proceso 4 : Suma del vector enviado a 5 = 5.000774e+06
  Proceso 5 : Suma del vector enviado a 6 = 5.000749e+06
  Proceso 6 : Suma del vector enviado a 7 = 5.001210e+06
  Proceso 7 : Suma del vector enviado a 0 = 5.000747e+06
  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 5.000747e+06 : Tiempo=0.188113 segundos
  Proceso 1 : Suma del vector recibido = 4.999254e+06 : Tiempo=0.000019 segundos
  Proceso 2 : Suma del vector recibido = 4.999467e+06 : Tiempo=0.228847 segundos
  Proceso 3 : Suma del vector recibido = 4.998405e+06 : Tiempo=0.000022 segundos
  Proceso 4 : Suma del vector recibido = 5.001335e+06 : Tiempo=0.000021 segundos
  Proceso 5 : Suma del vector recibido = 5.000774e+06 : Tiempo=0.000016 segundos
  Proceso 6 : Suma del vector recibido = 5.000749e+06 : Tiempo=0.297089 segundos
  Proceso 7 : Suma del vector recibido = 5.001210e+06 : Tiempo=0.000018 segundos

  ##########################################################

  Tiempo de comunicacion : 0.188131 seconds

  ##########################################################
  ```

  - P = 8, N = 20000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto no bloqueante: MPI_Isend, MPI_Irecv

  Dimension del vector: 20000000
  Numero de procesos: 8

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 1.000191e+07
  Proceso 1 : Suma del vector enviado a 2 = 1.000013e+07
  Proceso 2 : Suma del vector enviado a 3 = 9.998568e+06
  Proceso 3 : Suma del vector enviado a 4 = 1.000095e+07
  Proceso 4 : Suma del vector enviado a 5 = 9.998897e+06
  Proceso 5 : Suma del vector enviado a 6 = 9.998472e+06
  Proceso 6 : Suma del vector enviado a 7 = 1.000257e+07
  Proceso 7 : Suma del vector enviado a 0 = 1.000069e+07
  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 1.000069e+07 : Tiempo=0.548603 segundos
  Proceso 1 : Suma del vector recibido = 1.000191e+07 : Tiempo=0.000020 segundos
  Proceso 2 : Suma del vector recibido = 1.000013e+07 : Tiempo=0.581225 segundos
  Proceso 3 : Suma del vector recibido = 9.998568e+06 : Tiempo=0.000019 segundos
  Proceso 4 : Suma del vector recibido = 1.000095e+07 : Tiempo=0.476753 segundos
  Proceso 5 : Suma del vector recibido = 9.998897e+06 : Tiempo=0.000018 segundos
  Proceso 6 : Suma del vector recibido = 9.998472e+06 : Tiempo=0.426510 segundos
  Proceso 7 : Suma del vector recibido = 1.000257e+07 : Tiempo=0.000019 segundos

  ##########################################################

  Tiempo de comunicacion : 0.570880 seconds

  ##########################################################
  ```

  - P = 8, N = 40000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto no bloqueante: MPI_Isend, MPI_Irecv

  Dimension del vector: 40000000
  Numero de procesos: 8

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 1.999849e+07
  Proceso 1 : Suma del vector enviado a 2 = 2.000103e+07
  Proceso 2 : Suma del vector enviado a 3 = 1.999996e+07
  Proceso 3 : Suma del vector enviado a 4 = 2.000094e+07
  Proceso 4 : Suma del vector enviado a 5 = 1.999941e+07
  Proceso 5 : Suma del vector enviado a 6 = 2.000111e+07
  Proceso 6 : Suma del vector enviado a 7 = 1.999968e+07
  Proceso 7 : Suma del vector enviado a 0 = 1.999650e+07
  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 1.999650e+07 : Tiempo=0.982577 segundos
  Proceso 1 : Suma del vector recibido = 1.999849e+07 : Tiempo=0.000021 segundos
  Proceso 2 : Suma del vector recibido = 2.000103e+07 : Tiempo=1.101665 segundos
  Proceso 3 : Suma del vector recibido = 1.999996e+07 : Tiempo=0.000018 segundos
  Proceso 4 : Suma del vector recibido = 2.000094e+07 : Tiempo=1.090869 segundos
  Proceso 5 : Suma del vector recibido = 1.999941e+07 : Tiempo=0.000019 segundos
  Proceso 6 : Suma del vector recibido = 2.000111e+07 : Tiempo=1.113853 segundos
  Proceso 7 : Suma del vector recibido = 1.999968e+07 : Tiempo=0.000022 segundos

  ##########################################################

  Tiempo de comunicacion : 1.114677 seconds

  ##########################################################
  ```

  - P = 16, N = 10000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto no bloqueante: MPI_Isend, MPI_Irecv

  Dimension del vector: 10000000
  Numero de procesos: 16

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 4.999907e+06
  Proceso 1 : Suma del vector enviado a 2 = 4.999678e+06
  Proceso 2 : Suma del vector enviado a 3 = 5.001376e+06
  Proceso 3 : Suma del vector enviado a 4 = 4.999242e+06
  Proceso 4 : Suma del vector enviado a 5 = 4.999724e+06
  Proceso 5 : Suma del vector enviado a 6 = 5.001582e+06
  Proceso 6 : Suma del vector enviado a 7 = 4.998360e+06
  Proceso 7 : Suma del vector enviado a 8 = 4.999940e+06
  Proceso 8 : Suma del vector enviado a 9 = 5.002241e+06
  Proceso 9 : Suma del vector enviado a 10 = 4.998516e+06
  Proceso 10 : Suma del vector enviado a 11 = 4.999794e+06
  Proceso 11 : Suma del vector enviado a 12 = 4.999881e+06
  Proceso 12 : Suma del vector enviado a 13 = 4.999154e+06
  Proceso 13 : Suma del vector enviado a 14 = 4.999654e+06
  Proceso 14 : Suma del vector enviado a 15 = 4.998968e+06
  Proceso 15 : Suma del vector enviado a 0 = 5.000206e+06
  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 5.000206e+06 : Tiempo=0.125531 segundos
  Proceso 1 : Suma del vector recibido = 4.999907e+06 : Tiempo=0.000023 segundos
  Proceso 2 : Suma del vector recibido = 4.999678e+06 : Tiempo=0.071256 segundos
  Proceso 3 : Suma del vector recibido = 5.001376e+06 : Tiempo=0.000026 segundos
  Proceso 4 : Suma del vector recibido = 4.999242e+06 : Tiempo=0.071779 segundos
  Proceso 5 : Suma del vector recibido = 4.999724e+06 : Tiempo=0.000023 segundos
  Proceso 6 : Suma del vector recibido = 5.001582e+06 : Tiempo=0.578193 segundos
  Proceso 7 : Suma del vector recibido = 4.998360e+06 : Tiempo=0.000022 segundos
  Proceso 8 : Suma del vector recibido = 4.999940e+06 : Tiempo=0.597312 segundos
  Proceso 9 : Suma del vector recibido = 5.002241e+06 : Tiempo=0.000018 segundos
  Proceso 10 : Suma del vector recibido = 4.998516e+06 : Tiempo=0.508106 segundos
  Proceso 11 : Suma del vector recibido = 4.999794e+06 : Tiempo=0.000042 segundos
  Proceso 12 : Suma del vector recibido = 4.999881e+06 : Tiempo=0.430872 segundos
  Proceso 13 : Suma del vector recibido = 4.999154e+06 : Tiempo=0.000018 segundos
  Proceso 14 : Suma del vector recibido = 4.999654e+06 : Tiempo=0.580997 segundos
  Proceso 15 : Suma del vector recibido = 4.998968e+06 : Tiempo=0.000020 segundos

  ##########################################################

  Tiempo de comunicacion : 0.407453 seconds

  ##########################################################
  ```

  - P = 16, N = 20000000:

  ```
  ##########################################################

  Comunicacion punto-a-punto no bloqueante: MPI_Isend, MPI_Irecv

  Dimension del vector: 20000000
  Numero de procesos: 16

  ##########################################################

                  --> ANTES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector enviado a 1 = 1.000027e+07
  Proceso 1 : Suma del vector enviado a 2 = 1.000157e+07
  Proceso 2 : Suma del vector enviado a 3 = 9.999494e+06
  Proceso 3 : Suma del vector enviado a 4 = 1.000085e+07
  Proceso 4 : Suma del vector enviado a 5 = 1.000119e+07
  Proceso 5 : Suma del vector enviado a 6 = 1.000080e+07
  Proceso 6 : Suma del vector enviado a 7 = 1.000178e+07
  Proceso 7 : Suma del vector enviado a 8 = 9.999919e+06
  Proceso 8 : Suma del vector enviado a 9 = 9.998535e+06
  Proceso 9 : Suma del vector enviado a 10 = 9.999277e+06
  Proceso 10 : Suma del vector enviado a 11 = 9.997051e+06
  Proceso 11 : Suma del vector enviado a 12 = 1.000191e+07
  Proceso 12 : Suma del vector enviado a 13 = 1.000016e+07
  Proceso 13 : Suma del vector enviado a 14 = 9.998761e+06
  Proceso 14 : Suma del vector enviado a 15 = 9.999958e+06
  Proceso 15 : Suma del vector enviado a 0 = 9.999072e+06
  ##########################################################

                  --> DESPUES DE LA COMUNICACION <--

  Proceso 0 : Suma del vector recibido = 9.999072e+06 : Tiempo=0.371424 segundos
  Proceso 1 : Suma del vector recibido = 1.000027e+07 : Tiempo=0.000017 segundos
  Proceso 2 : Suma del vector recibido = 1.000157e+07 : Tiempo=0.000013 segundos
  Proceso 3 : Suma del vector recibido = 9.999494e+06 : Tiempo=0.000014 segundos
  Proceso 4 : Suma del vector recibido = 1.000085e+07 : Tiempo=1.143835 segundos
  Proceso 5 : Suma del vector recibido = 1.000119e+07 : Tiempo=0.000027 segundos
  Proceso 6 : Suma del vector recibido = 1.000080e+07 : Tiempo=0.000013 segundos
  Proceso 7 : Suma del vector recibido = 1.000178e+07 : Tiempo=0.000021 segundos
  Proceso 8 : Suma del vector recibido = 9.999919e+06 : Tiempo=0.648480 segundos
  Proceso 9 : Suma del vector recibido = 9.998535e+06 : Tiempo=0.000021 segundos
  Proceso 10 : Suma del vector recibido = 9.999277e+06 : Tiempo=0.000005 segundos
  Proceso 11 : Suma del vector recibido = 9.997051e+06 : Tiempo=0.000022 segundos
  Proceso 12 : Suma del vector recibido = 1.000191e+07 : Tiempo=1.171962 segundos
  Proceso 13 : Suma del vector recibido = 1.000016e+07 : Tiempo=0.000018 segundos
  Proceso 14 : Suma del vector recibido = 9.998761e+06 : Tiempo=0.859572 segundos
  Proceso 15 : Suma del vector recibido = 9.999958e+06 : Tiempo=0.000018 segundos

  ##########################################################

  Tiempo de comunicacion : 0.837314 seconds

  ##########################################################
  ```

  - P = 16, N = 40000000: Tiró error.

**Resumen de los tiempos de comunicación blocking**:

| P / N | 10000000 | 20000000  | 40000000 |
| ----- | -------- | --------- | -------- |
| 4     | 0.380921 | 0.874133  | 2.890444 |
| 8     | 1.540633 | 2.875753  | 9.912438 |
| 16    | 4.056052 | 11.280881 | Error    |

**Resumen de los tiempos de comunicación non-blocking**:

| P / N | 10000000 | 20000000 | 40000000 |
| ----- | -------- | -------- | -------- |
| 4     | 0.170362 | 0.262153 | 0.572247 |
| 8     | 0.188131 | 0.570880 | 1.114677 |
| 16    | 0.407453 | 0.837314 | Error    |

Los tiempos de comunicación de la versión no-bloqueante son significativamente mejores. Esto se debe a que la versión no bloqueante reduce el tiempo total al permitir que todas las comunicaciones ocurran en paralelo al mismo tiempo, mientras que la bloqueante las fuerza a ocurrir de forma secuencial.

## 4. El algoritmo `mpi_matmul.c` computa una multiplicación de matrices cuadradas empleando comunicaciones punto a punto:

### Compile y ejecute el código empleando N = {512, 1024, 2048} usando todos los núcleos de 1 y 2 nodos.

- Compilo con `mpicc -o mpi_matmul mpi_matmul.c`
- Ejecuto en el cluster con el script `mpi_matmul.sh`, al cual le modifico el N y los núcleos.

**Resultados con un (1) nodo**:

| N        | Tiempo total | Tiempo comunicación |
| -------- | ------------ | ------------------- |
| **512**  | 0.219915     | 0.034666            |
| **1024** | 1.761465     | 0.171872            |
| **2048** | 13.575914    | 0.730290            |

**Resultados con dos (2) nodos**:

| N        | Tiempo total | Tiempo comunicación |
| -------- | ------------ | ------------------- |
| **512**  | 0.283076     | 0.350365            |
| **1024** | 1.543307     | 1.383034            |
| **2048** | 9.447583     | 5.537866            |

### Revise las secciones de código donde se realiza la comunicación de las matrices. Analice el patrón de comunicación y piense si es posible emplear comunicaciones colectivas en lugar de a punto a punto. En ese caso, modifique el código original, compile y ejecute la nueva versión. ¿Se mejora la legibilidad? ¿Se logra mejorar el rendimiento? ¿Por qué?

Se pueden usar comunicaciones colectivas:

1. Para distribuir la matriz A en partes iguales entre los distintos procesos, se puede usar `MPI_Scatter`.
2. Para que todos los procesos tengan la matriz entera B, se puede usar `MPI_Bcast`.
3. Para que el proceso coordinador re-arme la matriz resultado C, se puede usar `MPI_Gather`.

- Una vez creada la versión modificada (`mpi_matmul_mejorado.c`) con estos cambios:
- Compilo con `mpicc -o mpi_matmul_mejorado mpi_matmul_mejorado.c`
- Ejecuto en el cluster con el script `mpi_matmul_mejorado.sh`, al cual le modifico el N y los núcleos.

**Resultados con un (1) nodo**:

| N        | Tiempo total | Tiempo comunicación |
| -------- | ------------ | ------------------- |
| **512**  | 0.215996     | 0.016239            |
| **1024** | 1.703909     | 0.058804            |
| **2048** | 13.489222    | 0.376488            |

**Resultados con dos (2) nodos**:

| N        | Tiempo total | Tiempo comunicación |
| -------- | ------------ | ------------------- |
| **512**  | 0.145825     | 0.054083            |
| **1024** | 1.011738     | 0.200021            |
| **2048** | 7.312100     | 0.780110            |

**Conclusión**:

- La legibilidad mejora sin duda, ya que hay menos sentencias if con send/receive y el código se hace mucho más corto.
- El rendimiento también mejora drásticamente, sobre todo los tiempos de comunicación.

## 5. Desarrolle un algoritmo paralelo que resuelva la expresión R = AB + CD + EF, donde A, B, C, D, E y F son matrices cuadradas de NxN. Ejecute para N = {512, 1024, 2048} con P = {2, 4, 8, 16}.

## 6. Desarrolle un algoritmo paralelo que dado un vector V de tamaño N obtenga el valor máximo, el valor mínimo y valor promedio de sus elementos. Ejecute para P = {2, 4, 8, 16} variando el valor de N.

## 7. Desarrolle una versión híbrida (MPI + OpenMP) de la multiplicación de matrices. Replique el análisis realizado para el algoritmo puro MPI (ejercicio 4) y compare sus rendimientos. ¿Cuál es mejor? ¿Por qué?
