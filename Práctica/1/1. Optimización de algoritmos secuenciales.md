<h1 align="center">Práctica 1</h1>

## Pautas generales

- Las pruebas se deben realizar en un equipo con sistema operativo Linux nativo (no virtualizado). La virtualización puede alterar el comportamiento y la toma de los tiempos de ejecución, imposibilitando la ocurrencia de los resultados y análisis esperados.
- Al momento de realizar las pruebas, deberá cerrar todo programa que tenga abierto (editores, navegadores etc). Mantenga abierta únicamente una consola/terminal para ejecutar las pruebas.
- En todos los ejercicios con matrices, pruebe con tamaños que sean potencias de 2 (512, 1024, 2048, etc).
- Tener en cuenta que, para poder notar cambios en la ejecución, los algoritmos deben ejecutarse al menos varios segundos (más de 15 segundos si es posible).

## Información útil para compilar y ejecutar

- Para compilar en Linux con gcc: `gcc –o salidaEjecutable archivoFuente.c`
- Para ejecutar: `./salidaEjecutable arg1 arg2 ... argN`

## 1. El algoritmo fib.c resuelve la serie de Fibonacci, para un número N dado, utilizando dos métodos: recursivo e iterativo. Analice los tiempos de ejecución de ambos métodos ¿Cuál es más rápido? ¿Por qué? Nota: ejecute con N=1..50.

Cuando N es pequeño, la solución recursiva es apenas más rápida. Cuando N crece, la solución iterativa escala bastante bien, pero la recursiva se vuelve inviable. Ya cuando N = 40, la solución recursiva tarda varios segundos en completarse.

Lo que sucede, para empezar, es que la solución iterativa es de O(N) mientras que la recursiva es de O(2^N), lo cual hace a esta última mucho peor.

En detalle, la solución recursiva usa más y más memoria debido a que cada llamada de función almacena la dirección de retorno, variables locales y datos relacionados en memoria. En cambio, la solución iterativa usa una cantidad fija de memoria sin importar el tamaño de N: como solo usa i, j y t, estos datos se mantienen en registros de la CPU o caché, evitando tener que acceder a memoria RAM. La solución recursiva no aprovecha la caché.

## 2. El algoritmo funcion.c resuelve, para un x dado, la siguiente sumatoria:

![Sumatoria](https://i.imgur.com/6cgJBK0.png)

El algoritmo compara dos alternativas de solución. ¿Cuál de las dos formas es más rápida? ¿Por qué?

La primer forma es un poco más rápida, porque en cada iteración de la sumatoria no es necesario ir a buscar el valor de x a memoria ni tampoco realizar todo el cálculo (sumas, productos y divisiones), simplemente se busca el valor de fx que ya está en memoria (seguramente en caché).

En la segunda forma, en cambio, se debe buscar en cada iteración el valor de x en memoria y también realizar todo el cálculo nuevamente.

## 3. Investigue en la documentación del compilador o a través de Internet qué opciones de optimización ofrece el compilador gcc (flag O). Compile y ejecute el algoritmo matrices.c, el cual resuelve una multiplicación de matrices de NxN. Explore los diferentes niveles de optimización para distintos tamaños de matrices. ¿Qué optimizaciones aplica el compilador? ¿Cuál es la ganancia respecto a la versión sin optimización del compilador? ¿Cuál es la ganancia entre los distintos niveles?

El compilador gcc ofrece varias opciones de optimización que se pueden activar usando el flag **-O**. Cada opción o nivel aplica estategias distintas para mejorar el rendimiento y/o reducir el tamaño del código generado. Las opciones son:

- **-O0**: Sin optimizaciones, por defecto.
- **-O1**: Optimización básica, como la eliminación de código inalcanzable, reducción de código redundante, eliminación de accesso innecesarios a memoria, reordenamiento de instrucciones para reducir dependencias, etc.
- **-O2**: Optimización más profunda, que realiza todas las del nivel anterior pero además otras como movilización de código invariante afuera de bucles, eliminación de sub-expresiones comunes, eliminación de variables que no se usan, desenrollado parcial de loops, mejoras de branch prediction, etc.
- **-O3**: Optimización máxima, que incluye todas las del nivel 1 y 2 pero además desenrollado completo de loops y vectorización automática para aprovechar instrucciones SIMD. Puede aumentar el tamaño del archivo binario y a veces ralentizar la ejecución.

Resultados en matrices.c:

| N    | Tiempo sin optimización | Tiempo con -O0 | Tiempo con -O1 | Tiempo con -O2 | Tiempo con -O3 |
| ---- | ----------------------- | -------------- | -------------- | -------------- | -------------- |
| 64   | 0.013208                | 0.012479       | 0.001006       | 0.000944       | 0.000799       |
| 128  | 0.054518                | 0.052563       | 0.008095       | 0.008153       | 0.007858       |
| 256  | 0.388905                | 0.380867       | 0.037743       | 0.037415       | 0.045507       |
| 512  | 16.990481               | 16.997709      | 2.541080       | 2.485843       | 2.450482       |
| 1024 | 145.628657              | 145.697443     | 20.201350      | 20.362543      | 19.558322      |

## 4. Dada la ecuación cuadrática: x2 − 4.0000000 x + 3.9999999 = 0, sus raíces son r1 = 2.000316228 y r2 = 1.999683772 (empleando 10 dígitos para la parte decimal).

### a. El algoritmo quadratic1.c computa las raíces de esta ecuación empleando los tipos de datos float y double. Compile y ejecute el código. ¿Qué diferencia nota en el resultado?

Puedo notar que al usar **float** perdemos muchísima precisión (obtenemos r1 = 2.00000 y r2 = 2.00000), mientras que al usar **double** esto no ocurre (obtenemos r1 = 2.00032 y r2 = 1.99968).

Esto se debe a que en C el tipo de dato **float** es de 32 bits, con lo cual puede almacenar hasta 6 o 7 dígitos de precisión, lo cual lleva a errores de redondeo, mientras que el tipo de dato **double** es de 64 bits, pudiendo almacenar 15 o 16 dígitos de precisión, lo cual es preferible para un problema como el presentado.

### b. El algoritmo quadratic2.c computa las raíces de esta ecuación, pero en forma repetida. Compile y ejecute el código variando la constante TIMES. ¿Qué diferencia nota en la ejecución?

**Ejecutando sin optimizaciones:**

| TIMES | Con Double | Con Float |
| ----- | ---------- | --------- |
| 25    | 17.095959  | 17.120130 |
| 50    | 32.939973  | 34.229064 |
| 75    | 48.529706  | 51.316531 |
| 100   | 64.805462  | 68.461127 |

Puedo notar que Double es un poco más rápido que Float en este programa, asumo que esto se debe a que, como las CPUs modernas y C mismo trabajan principamente con Double, los valores Float se deben convertir a Double explícita o implícitamente, y esto añade overhead.

**Ejecutando con optimización de nivel 3:**

| TIMES | Con Double | Con Float |
| ----- | ---------- | --------- |
| 25    | 0.749736   | 0.614030  |
| 50    | 1.457027   | 1.227465  |
| 75    | 2.183657   | 1.849212  |
| 100   | 2.913102   | 2.462341  |

Sin embargo, al usar la compilación optimizada, podemos ver que ahora Float es más rápido. Esto se debe (según Internet) a que en este caso el compilador vectoriza los loops usando instrucciones SIMD, lo cual favorece al tipo de dato Float por sobre el Double porque los registros del vector pueden almacenar más valores Float que Double, ya que Float es de 32 bits.

### c. El algoritmo quadratic3.c computa las raíces de esta ecuación, pero en forma repetida. Compile y ejecute el código variando la constante TIMES. ¿Qué diferencia nota en la ejecución? ¿Qué diferencias puede observar en el código con respecto a quadratic2.c?

**Ejecutando sin optimizaciones:**

| TIMES | Con Double | Con Float |
| ----- | ---------- | --------- |
| 25    | 16.152980  | 9.469951  |
| 50    | 32.310420  | 18.930253 |
| 75    | 48.465325  | 28.436711 |
| 100   | 65.796426  | 37.995905 |

**Ejecutando con optimización de nivel 3:**

| TIMES | Con Double | Con Float |
| ----- | ---------- | --------- |
| 25    | 0.717300   | 0.446042  |
| 50    | 1.433348   | 0.914508  |
| 75    | 2.170357   | 1.373198  |
| 100   | 2.879546   | 1.830206  |

Puedo ver que, tanto con como sin optimizaciones, los tiempos usando Float son significativamente mejores en este programa que los tiempos usando Float de quadratic2.

En cuanto a las diferencias, se ve que quadratic3 inicializa el vector de floats con valores Float, y el vector de doubles con valores Double. Quadratic2, en cambio, inicializaba ambos vectores con valores Double. Además de esto, noto que quadratic3 usa la función **powf y sqrtf** en vez de pow y sqrt para computar las potencias y raíces, respectivamente. Imagino que esto evita las conversiones de double a float completamente, minimizando mucho el overhead.

### Nota: agregue el flag -lm al momento de compilar. Pruebe con el nivel de optimización que mejor resultado le haya dado en el ejercicio anterior.

## 5. Analice el algoritmo matrices.c. ¿Dónde cree que se producen demoras? ¿Cómo podría optimizarse el código? Al menos, considere los siguientes aspectos:

- Explotación de localidad de datos a través de reorganización interna de matrices A, B o C (según corresponda).
- El uso de Setters y getters es una buena práctica en la programación orientada a objetos. ¿Tiene sentido usarlos en este caso? ¿cuál es su impacto en el rendimiento?
- ¿Hay expresiones en el cómputo que pueden refactorizarse para no ser computadas en forma repetida?
- En lugar de ir acumulando directamente sobre la posición C[i,j] de la matriz resultado (línea 72), pruebe usar una variable local individual y al finalizar el bucle más interno, asigne su valor a C[i,j]. ¿Esta modificación impacta en el rendimiento? ¿Por qué?

Combine las mejoras que haya encontrado para obtener una solución optimizada y compare los tiempos con la solución original para diferentes tamaños de matrices.

**Hay varios aspectos del código que se pueden mejorar**:

1. Se pueden eliminar las funciones getValor y setValor, ya que añaden overhead innecesario (se llama a estas funciones muchísimas veces, y cada llamada tiene un costo asociado).
2. Se puede usar una variable local acumuladora en vez de referenciar a C[i,j], lo cual aprovecha los registros de la CPU que son mucho más eficientes que buscar repetidamente el valor actual de C[i,j] a memoria principal.
3. Se pueden reordenar los loops de multiplicación para aprovechar la localidad de datos: por ejemplo, si usamos orden por filas, queremos asegurarnos que accedemos al arreglo de esa forma también, y si usamos orden por columnas, lo mismo. Queremos un acceso secuencial para así maximizar los hits de caché. Sin embargo, al hacer esto, debemos eliminar la variable local acumuladora.

**Tiempos de ejecución comparados**:

| N    | Tiempo sin mejoras | Tiempo con mejora 1 | Tiempo con mejoras 1 y 2 | Tiempo con mejoras 1 y 3 |
| ---- | ------------------ | ------------------- | ------------------------ | ------------------------ |
| 64   | 0.009879           | 0.004115            | 0.003332                 | 0.003132                 |
| 128  | 0.051383           | 0.024089            | 0.022032                 | 0.25269                  |
| 256  | 0.388294           | 0.141176            | 0.097588                 | 0.141245                 |
| 512  | 17.002334          | 6.754304            | 5.006866                 | 7.591046                 |
| 1024 | 146.145650         | 56.897177           | 42.528944                | 64.610861                |

## 6. Analice y describa brevemente cómo funciona el algoritmo mmblk.c que resuelve la multiplicación de matrices cuadradas de NxN utilizando una técnica de multiplicación por bloques. Luego, ejecute el algoritmo utilizando distintos tamaños de matrices y distintos tamaños de bloque (pruebe con valores que sean potencia de 2; p.e. N={512,1024,2048} y TB={16,32,64,128}). Finalmente, compare los tiempos con respecto a la multiplicación de matrices optimizada del ejercicio anterior. Según el tamaño de las matrices y de bloque elegido, responda: ¿Cuál es más rápido? ¿Por qué? ¿Cuál sería el tamaño de bloque óptimo para un determinado tamaño de matriz? ¿De qué depende el tamaño de bloque óptimo para un sistema?

El algoritmo mmblk.c funciona de la siguiente manera:

1. Al ejecutar el programa, se deben pasar dos argumentos: N y BS (Block Size), ambos enteros. Además, N debe ser potencia de 2 y BS también, pero BS debe ser <= N.
2. Se declaran 3 matrices de NxN, A B C. A será multiplicada por B y el resultado será almacenado en C.
3. A se inicializa **ordenada por filas**, mientras que B se inicializa **ordenada por columnas** (transpuesta). C se inicializa en 0.
4. Se multiplica A por B, almacenando el resultado en C, usando la técnica de multiplicación por bloques:
   1. Se dividen ambas matrices (A y B) en bloques pequeños, ambos de mismo tamaño.
   2. Se multiplican los bloques.
   3. Se suman los resultados parciales.
5. matmulblks coordina la multiplicación en general y blkmul realiza la multiplicación entre dos bloques.

Pruebas:

| N    | Tamaño de bloque | Tiempo   |
| ---- | ---------------- | -------- |
| 64   | 16               | 0.003930 |
| 128  | 32               | 0.024224 |
| 256  | 64               | 0.106095 |
| 512  | 128              | 0.766773 |
| 1024 | 256              | 6.622991 |

Podemos ver claramente que cuando N es pequeño, el tiempo de ejecución es prácticamente idéntico al del ejercicio anterior. Pero a medida que N crece, podemos ver que el tiempo es muchísimo mejor comparado al ej. anterior.

El tamaño de bloque debe caber en la caché, idealmente la de nivel 1 (L1) ya que es la más rapida. Esto va a variar, ya que el tamaño de caché L1 es distinto en cada modelo de CPU.

Intento encontrar el tamaño de bloque óptimo con N = 1024:

| N    | Tamaño de bloque | Tiempo   |
| ---- | ---------------- | -------- |
| 1024 | 16               | 6.984983 |
| 1024 | 32               | 6.181347 |
| 1024 | 64               | 6.249279 |
| 1024 | 128              | 5.988223 |
| 1024 | 256              | 6.613368 |
| 1024 | 512              | 6.747835 |

Se puede ver que, en el caso de esta CPU (AMD A9-9425), el tamaño de bloque óptimo es de **128**. Esto tiene sentido, ya que esta CPU posee una caché L1 de **32 KB** por core, y una caché L2 de **1 MB**compartida por 2 núcleos.

Como tenemos 3 matrices, donde cada bloque de matriz pesa 128 (filas) \* 128 (columnas) \* 8 bytes (tamaño de un double) = 131072 bytes = 128 KB → 128 KB \* 3 = 384 KB, lo cual cabe en nuestra caché L2 de 1 MB.

Sin embargo, si usamos tamaño de bloque 256: 256 \* 256 \* 8 = 524288 bytes = 524 KB → 524 KB \* 3 = 1572 KB, lo cual **no cabe** en nuestra caché L2 de 1 MB, lo cual provoca muchos caché misses y accesos lentos a RAM.

## 7. Analice el algoritmo triangular.c que resuelve la multiplicación de una matriz cuadrada por una matriz triangular inferior, ambas de NxN. ¿Cómo se podría optimizar el código? ¿Se pueden evitar operaciones? ¿Se puede reducir la memoria reservada? Implemente una solución optimizada y compare los tiempos probando con diferentes tamaños de matrices.
