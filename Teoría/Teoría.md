<h1 align="center">Clase 1 - 14 de marzo, 2025</h1>

## Fundamentos de procesamiento paralelo

### Introducción

- El procesamiento paralelo es el **uso de múltiples unidades de procesamiento para resolver un problema computacional**.
- El problema se divide en partes separadas que pueden ser resueltas en forma **concurrente**.
- Cada parte es luego dividida en una serie de instrucciones.
- Las instrucciones de cada parte se ejecutan **simultáneamente** sobre **diferentes procesadores**.
- Un mecanismo de control/coordinación global es necesario.

### Importancia

El procesamiento paralelo es fundamental por varias razones:

- Resolver problemas más grandes o más complejos que no son factibles en un solo procesador.
- Proveer concurrencia.
- Ahorrar tiempo y/o dinero.
- Explotar mejor los recursos de hardware (sobre todo con lo populares que son los multicore hoy en día).

## Procesamiento concurrente, paralelo y distribuido

- **Programa concurrente**: múltiples tareas pueden estar avanzando en cualquier instante de tiempo.
- **Programa paralelo**: múltiples tareas que se **ejecutan simultáneamente cooperan** para resolver un problema.
- **Programa distribuido**: múltiples tareas que se ejecutan **físicamente en diferentes lugares** cooperan para resolver uno o más problemas.
- Los programas paralelos y distribuidos SON concurrentes.
- La concurrencia es un concepto de software, por ende especificar la concurrencia implica especificar los procesos concurrentes, su comunicación y sincronización.
- El procesamiento paralelo busca reducir el tiempo de ejecución de un programa empleando múltiples procesadores al mismo tiempo.

## Plataformas de cómputo paralelo

- Para alcanzar buena performance, es fundamental conocer las características de la arquitectura de hardware subyacente.
- Existen muchas formas de clasificar a las plataformas de cómputo paralelo. Entre las más usadas se encuentran la clasificación por mecanismo de control y la clasificación por organización física.

### Por el mecanismo de control

- Es una taxonomía diseñada por Flynn que clasifica a las computadoras en 4 tipos según el número de instrucciones y datos que se pueden procesar en simultáneo:
- **Single Instruction Single Data**:
  - Secuencial, una instrucción por ciclo de reloj.
  - Los datos afectados en cada ciclo de reloj son aquellos que hace referencia la instrucción en cuestión.
  - La ejecución es determinística.
  - Es el más antiguo.
- **Single Instruction Multiple Data**:
  - Todas las CPUs ejecutan la misma instrucción sobre diferentes datos.
  - La ejecución es sincrónica y determinística.
- **Multiple Instruction Single Data**:
  - Las CPUs ejecutan diferentes instrucciones sobre el mismo dato.
  - No existen máquinas que usen este modelo.
- **Multiple Instruction Multiple Data**:
  - Las CPUs ejecutan diferentes instrucciones sobre diferentes datos.
  - La ejecución puede ser sincrónica o asincrónica, y determinística o no determinística.
  - Puede usarse en máquinas de memoria compartida o en máquinas de memoria distribuida.
  - Es el tipo más común hoy en día.

### Por la organización física

- Se clasifican de acuerdo al espacio de direcciones que posee cada procesador:
- **Memoria compartida**:
  - Los procesadores acceden a toda la memoria como un **único espacio de direcciones global**.
  - Cambios realizados en la memoria por un procesador son visibles para el resto.
  - Se necesita un mecanismo de coherencia de caché.
  - Pueden acceder de dos formas a memoria: de forma uniforme o de forma no uniforme.
  - La comunicación de datos entre procesadores es rápida.
  - La programación en estos sistemas suele ser más fácil.
  - Se debe asegurar un acceso correcto a los datos por parte de los procesadores.
  - Poco escalable.
- **Memoria distribuida**:
  - Se usa una red de interconexión (Ethernet, Infiniband, Myrinet, etc) para poder conectar a los procesadores.
  - Cada procesador es independiente y tiene **su propia memoria**, no hay un espacio de direcciones global.
  - Cambios realizados en la memoria por un procesador NO son visibles para el resto.
  - Cuando un procesador requiere un dato de otro procesador, el programador es responsable de definir cómo y cuándo será comunicado.
  - No se requiere un mecanismo de coherencia de caché.
  - La memoria escala con el número de procesadores de forma proporcional.
  - Cada procesador accede de forma más rápida a los datos, por no haber un protocolo de coherencia de caché.
  - Buena relación costo-rendimiento: se pueden construir a partir de máquinas convencionales.
  - Los datos en nodos remotos toman más tiempo.
  - El manejo de la comunicación es explícito.
  - El espacio de direcciones distribuido puede afectar a la forma de programación (estructuras de datos globales por ejemplo).
- **Memoria híbrida**:
  - Es el más usado por los sistemas más grandes y rápidos hoy en día.
  - Múltiples máquinas de memoria compartida son inter-conectadas entre sí para permitir que sus procesadores puedan comunicarse.
  - Soluciona la falta de escalabilidad de la MC.
  - Aumenta la complejidad en la programación.

## Modelos de programación paralela

- Hay diferentes modelos para desarrollar programas paralelos, los cuales difieren en la forma en que las tareas de un programa comparten datos y se sincronizan.
- Existen 2 alternativas:
- **Memoria compartida**:
  - Múltiples tareas se ejecutan en forma concurrente.
  - Todas las tareas acceden a un lugar de memoria común pero además cada una puede tener su memoria local propia.
  - La comunicación y sincronización de estas tareas se realiza escribiendo y leyendo áreas de memoria compartida.
  - Generalmente usado en plataformas de memoria compartida como multiprocesadores o multicores.
  - El programador no maneja la distribución de los datos ni lo relacionado a la comunicación de los mismos.
  - Es transparente para el programador.
  - A veces es necesario trabajar sobre los datos para mejorar la performance.
- **Pasaje de mensajes**:
  - Hay P procesos (eventualmente procesadores), cada uno con su propio espacio de direcciones.
  - Cada dato pertenece a un espacio de direcciones particular.
  - Toda interacción requiere la cooperación de dos procesos.
  - Los mensajes sirven para 1) intercambio explícito de datos 2) sincronizar procesos.
  - Se suele usar en plataformas de memoria distribuida como los clusters.
  - El programador tiene control total.
  - Puede implementarse eficientemente en muchas arquitecturas paralelas.
  - Fácil de predecir el rendimiento.
  - Complejidad al implementar estos algoritmos.

## Evolución del poder computacional

### Pipelining

- Consiste en dividir la ejecución de cada instrucción en varias etapas, donde cada etapa se encarga de una función específica.
- Permite que múltiples instrucciones progresen simultáneamente estando en diferentes etapas.
- La idea es la "línea de montaje": Si un auto toma 100 horas para construirse y lleva 10 etapas, se puede dividir su construcción en un pipeline de 10 horas cada una → se arma un auto en 10 horas.
- Para aumentar la velocidad del pipeline, se lo puede dividir en etapas más pequeñas haciéndolo más profundo.
  - Esto permite a su vez mayores clock speeds.
  - La velocidad del pipeline está **limitada** por la duración de su etapa más lenta.
- Hay un límite práctico de cuánto se puede dividir un pipeline, hacerlo infinitamente no sirve.

### Consecuencias de saltos condicionales

- Las bifurcaciones (branches) provocan atascos en el pipeline porque la CPU no sabe qué instrucción buscar a continuación hasta que se evalúe la condición.
- Esto puede llevar a penalizaciones de predicciones incorrectas donde la pipeline debe vaciarse y reiniciarse, desperdiciando ciclos de CPU.
- Cuanto más profundo el pipeline, peor es la penalización en las predicciones incorrectas.

### Ejecución especulativa

- Para reducir los atascos en el pipeline mencionados, las CPUs usan ejecución especulativa, donde "adivinan" el resultado de una branch y empiezan a ejecutar instrucciones de forma adelantada.
- Si adivina bien, la ejecución continúa de forma óptima.
- Si adivina mal, se descartan los resultados especulativos y se ejecuta el camino correcto, causando un retraso.

### Múltiples pipelines

- Algunas CPUs usan varios pipelines paralelos, permitiendo que varias instrucciones se procesen en diferentes pipelines simultáneamente.
- Es decir, permiten que dos o más instrucciones (dependiendo la cantidad de pipelines paralelos) se ejecuten **por ciclo**.
- No todas las instrucciones se pueden ejecutar en paralelo, debido a dependencias.

### Ejecución superescalar

- Las arquitecturas superescalares expanden en los pipelines múltiples ejecutando varias instrucciones por ciclo de forma dinámica.

### Ejecución fuera de orden

- Las CPUs pueden re-ordenar las instrucciones a ejecutar de forma dinámica para evitar stalls (penalizaciones previamente mencionadas).
- En vez de ejecutar las instrucciones de cada programa en el orden en que aparecen, la CPU ejecuta instrucciones que están ready-to-run, mientras espera que las dependencias se resuelvan.

### Walls

- A principios de los 2000, la mejora en performance de las CPUs se empezó a estancar, debido a 3 barreras o "walls".
- **Memory Wall**:
  - El número de ciclos de reloj por acceso a memoria se incrementa año a año (~50%).
  - De no mediar cambios disruptivos, llegará un momento en que el rendimiento estará dominado por la velocidad de memoria.
- **ILP Wall**:
  - Si bien es posible agregar más unidades funcionales al chip, no se alcanzaría una mejora de rendimiento para la mayoría de las aplicaciones debido a que **no es posible extraer más ILP de los programas**.
  - Los compiladores tienen sus limitaciones.
  - Las dependencias entre las instrucciones.
  - Imposibilidad de predecir saltos.
  - Cantidad limitada de paralelismo intrínseco.
- **Power Wall**:
  - La energía que usa el procesador se transforma en calor, que debe disiparse.
  - Cuando la temperatura aumenta, se reduce la velocidad de los transistores, lo que puede llevar a fallos o incluso la destrucción de la CPU.
  - Este calor debe disiparse de forma eficiente, pero usando métodos convencionales.
  - Para aumentar la frecuencia y mantener el circuito digital funcionando correctamente, se debe incrementar al mismo tiempo la tensión.
  - Incrementar la tensión trae aparejado un aumento de la potencia → El problema es que la relación no es lineal, si no cúbica.
  - Es decir, aumentar la velocidad de reloj, a su vez aumenta muchísimo el uso de energía del CPU.

### Cambio de paradigma en la mejora de los procesadores

- En lugar de seguir aumentando la compleja organización interna del chip, las empresas fabricantes optaron por integrar dos o más **núcleos** computacionales (también llamados **cores**) más simples en un sólo chip (multicores).
- Los procesadores multicore proveen el potencial de mejorar el rendimiento sin necesidad de aumentar la frecuencia de reloj, lo que los vuelve más **eficientes energéticamente**.
- Al bajar la frecuencia de reloj un 30%, el consumo de potencia se reduce a un 35% de su valor original, debido a la relación cúbica (0.7 × 0.7 × 0.7 ≈ 0.35).
- Pero esto también implica una reducción en el rendimiento del 30%.
- Sin embargo, al operar dos núcleos computacionales al 70% de su frecuencia de reloj original, se cuenta con 140% del poder de cómputo original usando sólo 70% del consumo de potencia inicial (2 × 35%).
- **Resumiendo, se optó por usar multicores con clock speeds "bajas" vs monocores con clock speeds altas, lo cual reduce bastante el uso de energía mientras que permite una performance equivalente si se explotan esos cores vía paralelismo**.

### Procesadores multicore

- Los primeros multicore eran básicamente dos procesadores monocore en una misma oblea.
- Las siguientes generaciones han incrementado el número de núcleos e incorporado niveles de caché L2 y L3 (con o sin compartición).

### Multithreading a nivel hardware

- La técnica Simultaneous Multi-Threading (SMT) consiste en mantener más de un hilo de ejecución al mismo tiempo en el procesador.
- Los recursos asociados al estado del procesador son replicados una o más veces (contador de programa, registros, punteros, pila, etc) manteniendo el número original de recursos de ejecución (unidades funcionales, cachés, interfaces de memoria, etc) → sólo requiere un pequeño incremento en el tamaño del chip.
- Con esta replicación, el procesador **parece** tener múltiples núcleos (a veces llamados procesadores lógicos) y por lo tanto puede ejecutar múltiples flujos (hilos) en paralelo, sin importar si pertenecen al mismo programa o a diferentes programas.
- El número de replicaciones de estados determina el número de procesadores lógicos del procesador.
- Sin SMT, solo un hilo de ejecución puede estar activo en un determinado momento.
- Los distintos hilos **no pueden usar la misma unidad funcional al mismo tiempo**.
- SMT puede mejorar la productividad del procesador (en términos de la cantidad de instrucciones ejecutadas por ciclo) siempre y cuando sea posible intercalar instrucciones de múltiples hilos entre los pipelines.
- Desventaja de SMT: Si los hilos usan exactamente los mismos recursos, podría haber perdida de rendimiento por la competencia entre ellos. Por ejemplo, programas que sólo computan en punto flotante.
- **La ganancia en performance por el uso de SMT depende fuertemente del programa a ejecutar. Generalmente, lo que conviene es hacer pruebas con y sin uso de SMT para evaluar su posible beneficio**.

### Multicore + multithread

- Los núcleos de un multicore pueden tener múltiples hilos de hardware.
- Las posibles combinaciones son:
  - Monocore sin SMT.
  - Monocore con SMT.
  - Multicore sin SMT.
  - Multicore con SMT.
- El número de hilos por núcleo suele ser 2 o 4, rara vez más.

### Jerarquía de memoria en multicores

- Los procesadores multicore suelen tener memoria caches de múltiples niveles.
- El nivel 1 siempre es privado.
- Los siguientes niveles dependen de cada arquitectura.
- Las cachés privadas son más rápidas por estar más cerca de los cores.
- En las cachés compartidas, hilos en diferentes núcleos pueden compartir datos que están en la misma caché. Además, hay más espacio de caché disponible si un único hilo (o pocos) se ejecuta en el procesador.

## Clusters

- Se pueden definir como una colección de computadoras individuales interconectadas vía algún tipo de red (Ethernet, Infiniband, Myrinet, etc), que trabajan en conjunto como un único recurso integrado de cómputo.
- Cada nodo de procesamiento es un sistema de cómputo en sí mismo, con hardware y sistema operativo **propio**.
- Puede ser homogéneo o heterogéneo.
- Ofrecen una buena relación costo-rendimiento y son escalables.
- En la actualidad, la mayoría de los grandes sistemas de cómputo se basan en clusters de nodos multi/many-core, conformando una arquitectura híbrida.
- Dado que los clusters son arquitecturas distribuidas, el modelo de programación más utilizado suele ser pasaje de mensajes.
- También es posible emplear el modelo de memoria compartida, aunque a costo de menor rendimiento.
- Con la incorporación de los procesadores multicore a los clusters, surgió un nuevo modelo de programación que combina pasaje de mensajes con memoria compartida.

---

<h1 align="center">Clase 2 - 21 de marzo, 2025</h1>

## Memory Wall

### Limitaciones

- En muchas apps, el factor limitante es la memoria y el manejo de la misma y no la velocidad de la CPU.
- Hay dos parámetros fundamentales en el sistema de memoria:
  - **La latencia**: tiempo que transcurre desde que se solicita el dato hasta que está disponible.
  - **El ancho de banda**: velocidad con la cual el sistema puede alimentar al procesador.
- Para contrarrestar estas limitaciones, es importante explotar la localidad espacial y temporal de los datos: es decir, usar estructuras de datos correctamente para realizar la menor cantidad de accesos a RAM posible y realizar la mayor cantidad de caché hits posibles.

## Arreglos multidimensionales y su organización en memoria

### Concepto

- Los arreglos multidimensionales son la estructura de datos más usada en High Performance Computing. Las demás estructuras de datos (árboles, listas, grafos, etc) no se usan seguido ya que son menos eficientes en el contexto del paralelismo comparados a los arreglos.
- Los datos de un arreglo multidimensional pueden ser almacenados en memoria por filas o por columnas.
- Es ideal un arreglo que:
  - No tenga tamaño máximo al ser declarado.
  - Pueda organizarse por filas o columnas según el programador requiera.
  - Sus datos estén en posiciones de memoria contiguas.
  - Idealmente permita cambiar su tamaño en tiempo de ejecución.

### Definiciones de arreglo erróneas

```c
#define N 100

float matriz[N][N];
```

- Tiene tamaño máximo.
- No se puede elegir su organización.
- No permite cambiar su tamaño en tiempo de ejecución.

```c
int n = 100;
float matriz[N][N];
```

- Tiene tamaño máximo.
- No se puede elegir su organización.
- No permite cambiar su tamaño en tiempo de ejecución.

```c
#define N 100

float ** matriz = malloc(N * sizeof(float*));
for (i = 0; i < N; i++) {
  matriz[i] = malloc..
}
```

- No todos los datos están contiguos en memoria.

### Definición de arreglo correcta

```c
#define N 100

float * matriz = malloc(N * N * sizeof(float));
```

- Esta definición cumple las 4 condiciones.
- Básicamente, se tiene un arreglo común (unidimensional), donde ciertas partes representan filas y otras columnas.
- Aprovecha la localidad de datos.
- Hace posible el uso de instrucciones SIMD.
- Facilita el intercambio de arreglos entre programas escritos con distintos lenguajes.
- Si la matriz está ordenada por filas, accedemos a la posición [i, j] con: `matriz[i * N + j]`
- Si la matriz está ordenada por columnas, accedemos a la posición [i, j] con: `matriz[j * N + i]`

## Coherencia de caché en arquitecturas multiprocesador

### Concepto

- En las máquinas de memoria compartida pueden existir muchas copias de un mismo dato, y resulta necesario mantener consistencia entre estas copias.
- Un mecanismo de coherencia asegura que todas las operaciones realizadas sobre las múltiples copias son **serializables**, por ende debe haber algún orden de ejecución secuencial que se corresponde con la planificación paralela.

### Protocolos de coherencia

- Hay 2 protocolos de coherencia principales:
- Asumimos que varios procesadores tienen una copia de la variable X en memoria principal de forma local, por ende inicialmente todos tienen el mismo valor.
- **De invalidación**:
  - Cuando un procesador modifica su X local, le dice a todos los demás procesadores que sus X's locales ya no son válidas, para que borren su valor y no lo usen.
  - Además, se invalida el valor original de X de la memoria principal.
  - Es el más usado.
  - Se puede producir ocio ante la espera de actualización de un dato.
- **De actualización**:
  - Cuando un procesador modifica su X local, le envía a todos los demás procesadores el nuevo valor actualizado para que ellos actualicen su X local.
  - Además, se actualiza el valor original de X en la memoria principal.
  - Si un procesador lee un dato una vez y no lo vuelve a usar, este protocolo produce overheard innecesario.
  - Si dos procesadores trabajan sobre la misma variable en forma alternada, este protocolo es la mejor opción, debido a que se evita/reduce el ocio por espera del dato actualizado.

### Ejemplo de protocolo basado en invalidación

- Cada copia puede tener uno de tres estados:
- **Shared**: hay múltiples copias válidas del dato en distintas memorias. Ante una escritura, pasa a estado **dirty** donde se produjo, mientras que en el resto se marca como **invalid**.
- **Dirty**: la copia es válida y se puede trabajar con la misma.
- **Invalid**: la copia es inválida. Ante una lectura, se actualiza a partir de la copia **dirty**.

### Implementación de protocolos de coherencia de caché

#### Sistemas Snoopy

- **Funcionamiento**:
  - La caché de cada procesador mantiene un conjunto de tags asociados a sus bloques, los cuales determinan su estado.
  - Todos los procesadores monitorean (snooping) el bus, lo cual permite realizar las transiciones de estado de sus bloques.
  - Cuando el hardware snoopy detecta una **lectura** sobre un bloque dirty, toma el control del bus y cumple el pedido.
  - Cuando el hardware snoopy detecta una **escritura** sobre un bloque de datos del cual tiene copia, entonces la marca como inválida.
  - Si cada procesador opera sobre datos **disjuntos**, entonces los mismos pueden ser cacheados:
    - Ante operaciones de escritura, el dato es marcado como dirty. Al no haber operaciones de otros procesadores, las siguientes peticiones se satisfacen localmente.- Ante operaciones de lectura, el dato es marcado como compartido (aún cuando sean varios procesadores). Las peticiones siguientes se satisfacen localmente en todos los casos.
    - En ambos casos, el protocolo **no agrega overhead adicional**.
  - Si diferentes procesadores realizan lecturas y escrituras sobre el mismo dato, se genera tráfico en el bus para poder mantener la coherencia de los datos.
    - Tener en cuenta que al basarse en redes broadcast, el mensaje de coherencia les llegará todos los procesadores, aun cuando no tengan el dato en cuestión.
    - Como el bus a su vez tiene un ancho de banda limitado, se convierte en un cuello de botella.
- Se suele usar en sistemas multiprocesador interconectados vía red broadcast, como bus o anillo.
- Se usa ampliamente en sistemas comerciales por ser un esquema simple, low cost y con buen rendimiento para operaciones locales.

#### Sistemas basados en directorios

- Se incorpora un directorio en memoria principal que guarda info de estado (bits de presencia + estado) sobre los bloques de caché y los procesadores donde están cacheados.
- La información contenida en el directorio permite que **sólo aquellos procesadores que tienen un determinado dato queden involucrados en las operaciones de coherencia, mejorando la eficiencia**.
- Al igual que con Snoopy, si los procesadores operan sobre datos disjuntos, las peticiones pueden cumplirse localmente, lo cual no agrega overhead.
- Cuando múltiples procesadores leen y escriben los mismos datos, se generan operaciones de coherencia, lo cual provoca **overhead adicional por la necesidad de mantener actualizado el directorio**.
  - Como el directorio está en memoria, si un programa paralelo requiere un gran número de operaciones de coherencia, se genera **overhead por la competencia en el acceso al recurso** (la memoria sólo puede satisfacer un número limitado de operaciones por unidad de tiempo).
  - La cantidad de memoria requerida por el directorio **puede convertirse en un cuello de botella** a medida que el número de procesadores crece.
- Como el directorio representa un punto centralizado de acceso (overhead por competencia), una solución posible es particionarlo → Sistemas basados en directorios distribuidos.

#### Sistemas basados en directorios distribuidos

- Se da en arquitecturas escalables, donde la memoria se encuentra **físicamente distribuida**.
- Cada procesador es responsable de mantener la coherencia de sus propios bloques (mantiene su **propio directorio**).
  - Cada bloque tiene un dueño.
  - Cuando un procesador desea leer un bloque por primera vez, debe pedírselo al dueño, quien redirige el pedido de acuerdo a la información del directorio.
  - Cuando un procesador escribe un bloque de memoria, **envía una invalidación al dueño**, quien luego la propaga a todos aquellos que tienen una copia.
- Como el directorio está distribuido, la competencia en el acceso al mismo se alivia, por ende es un sistema **más escalable**.
- Ahora el **cuello de botella** pasa a ser la latencia y el ancho de banda de la red de interconexión.

## Costos de comunicación

- Uno de los mayores **overheads en los programas paralelos proviene de la comunicación entre unidades de procesamiento**.
- El costo de la comunicación depende de múltiples factores y no sólo del medio físico:
  - Modelo de programación.
  - Topología de red.
  - Manejo y ruteo de datos.
  - Protocolos de software asociados.
- Los costos son diferentes según la **forma de comunicación**:
  - Para pasaje de mensajes, el costo es: `t_comm = t_s + m * t_w` donde:
    - t_s es el tiempo requerido para preparar el mensaje.
    - m es el tamaño del mensaje en words.
    - t_w es el tiempo requerido para transmitir una word.
  - Para memoria compartida resulta difícil modelar el costo debido a varios factores fuera del control del programador:
    - Los tiempos de acceso dependen de la ubicación del dato.
    - El overhead de los protocolos de coherencia son difíciles de estimar.
    - La localidad espacial es difícil de modelar.
    - Competencia generada por accesos compartidos depende de la planificación en ejecución.
    - Etc.

---

<h1 align="center">Clase 3 - 28 de marzo, 2025</h1>

## Programación en memoria compartida

### Concepto de las plataformas de memoria compartida

- Los procesadores usan un espacio de datos común.
- Los módulos de memoria pueden ser locales o globales.
- El acceso a memoria puede ser UMA (todos los procesadores están a la misma distancia de la memoria) o NUMA (algunos procesadores están "más cerca" de la memoria).
- Se necesita un mecanismo de coherencia de caché.
- Forma de programar: memoria compartida o pasaje de mensajes.

### Sincronización

- Es responsabilidad del programador.
- Toda sincronización reduce la eficiencia.
- La localidad de datos es clave en el rendimiento, sobre todo en NUMA.
- En algunos lenguajes el programador puede actuar sobre la localidad de los datos, en otros tendrá que re-estructurar el código.
- El programador no suele manejar la distribución de los datos ni lo relacionado a la comunicación de éstos:
  - La bueno de esto es la transparencia, la ubicación de los datos, su replicación y su migración son transparentes.
  - Lo malo de esto es que a veces es necesario trabajar sobre esos aspectos para mejorar el rendimiento. Además, resulta difícil la predicción de la performance a partir de leer el algoritmo.

### Soporte para concurrencia y sincronización

- Los distintos modelos de programación proveen un soporte para expresar la concurrencia y sincronización:
  - Los modelos basados en **procesos** suponen datos locales (privados) de cada proceso.
  - Los modelos basados en threads o procesos “livianos" suponen que toda la memoria es global → **Pthreads**.
  - Los modelos basados en directivas extienden el modelo basado en threads para facilitar su manejo (creación, sincronización, etc) → **OpenMP**

### Hilos

- Un thread es un único hilo de control en el flujo de un programa.
- Todos los hilos tienen acceso a una memoria compartida global.
- Los hilos a su vez tienen su propio espacio de memoria privada.

#### Ventajas de los hilos por sobre los procesos

- **Liviandad → Rendimiento**:
  - Los hilos son más livianos que los procesos.
  - Su intercomunicación es más rápida por compartir memoria y su cambio de contexto resulta menos costoso.
- **Ocultamiento de latencia → Multi-tasking**:
  - Múltiples hilos en ejecución contribuyen a reducir la latencia ocasionada por los accesos a memoria, la E/S y la comunicación.
- **Planificación y balance de carga**:
  - Las APIs de hilos suelen permitir la creación de una gran cantidad de tareas concurrentes, que luego pueden ser mapeadas dinámicamente a través de primitivas a nivel de sistema → minimiza el overhead por ociosidad.
  - Al mismo tiempo, facilita la distribución de trabajo ante cargas irregulares.
- **Facilidad de programación y uso extendido**:
  - Más fácil de programar que pasaje de mensajes (no requiere el manejo de la comunicación de
    datos).
- **Portabilidad**:
  - Permite migrar aplicaciones entre arquitecturas.
  - Útil para desarrollo.

## POSIX Threads (PThreads)

### Concepto

- Hasta mediados de los años 90, existían numerosas APIs para el manejo de hilos (incompatibles entre ellas).
- En 1995, IEEE especifica el estándar POSIX Threads (normalmente llamado Pthreads). Básicamente, un conjunto de tipos de datos y funciones para el lenguaje de programación C.
- POSIX se ha establecido como una API estándar para manejo de Threads, provista por la mayoría de los desarrolladores de sistemas operativos.
- Los conceptos que se discutirán son independientes de la API y son mayormente válidos para utilizar hilos en Java, Python, Go, etc.

### Rutinas

Pthreads usa 3 tipos de rutinas distintas:

- Manejo de threads.
- Mutexes.
- Variables condición.

### Creación de hilos

- Al inicio hay un solo hilo, el cual se suele llamar hilo main o master.
- Todos los demás los crea el programador.
- Los hilos pueden crear otros hilos.
- La creación se realiza con `pthread_create()`:

```c
int pthread_create(pthread_t *thread_handle, const pthread_attr_t *attribute, void * (*thread_function)(void *), void *arg);
```

- **thread_handle** es la dirección de un objeto pthread_t, que representa al hilo en sí.
- **attribute** es la dirección de un objeto pthread_attr. Es NULL para valores por defecto.
- **thread_function** es la función que ejecutará el hilo creado una vez comience su ejecución.
- **arg** es el único argumento que se le puede pasar de forma directa al hilo creado.

### Terminación de hilos

- Para terminar su ejecución, los hilos invocan a `pthread_exit()`.
- Esta función finaliza la ejecución del hilo y retorna un valor que puede ser posteriormente leído por otro hilo (en general el hilo que lo creó).

```c
int pthread_exit(void *res);
```

### Join de hilos

- Para sincronizar al hilo padre con el hilo hijo una vez que el hijo termina, se usa `pthread_join()`.
- Esta función bloquea al hilo llamador hasta que el hilo especificado como argumento termine su ejecución.
- Se debe usar una vez por cada hilo hijo. No una vez para todos.

### Ejemplo básico de Hello World

```c
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 10

void * hello_world(void *ptr);

void * hello_world(void *ptr) {
  int *p, id;
  p = (int *) ptr;
  id = *p;

  printf("\n¡Hola mundo! Soy el hilo %d", id);

  pthread_exit(0);
}

int main() {
  int i, ids[NUM_THREADS];
  pthread_attr_t attr;
  pthread_t threads[NUM_THREADS];

  pthread_attr_init(&attr);

  for(i = 0; i < NUM_THREADS; i++) {
    ids[i] = i;
    pthread_create(&threads[i], &attr, hello_world, &ids[i]);
  }

  for(i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL)
  }

  return 0;
}
```

### Pasaje de parámetros

- Si se quieer pasar varios parámetros a cada hilo, hay 2 alternativas:
  - Pasarle un **struct** a cada hilo que contenga todos los argumentos que necesita.
  - Mantener uno o más arreglos globales y pasarle el id a cada hilo para que sepa a qué posición debe acceder.

### Exclusión mutua

- Las secciones críticas en Pthreads se implementan usando **mutex_locks**, los cuales realizan bloqueo por exclusión mutua.
- Los **mutex_locks** tienen dos estados: locked y unlocked. En cualquier instante, solo un hilo puede bloquear un mutex_lock: lock es una operación atómica.
- Para poder entrar en la sección crítica un hilo debe lograr tener control del mutex_lock, es decir bloquearlo.
- Cuando un hilo sale de la sección crítica debe desbloquear el mutex_lock.
- Todos los mutex_locks deben inicializarse como desbloqueados.

```c
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *lock_attr);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

#### Tipos de locks

Pthreads soporta tres tipos de locks (este tipo se especifica en los atributos del mutex en su inicialización):

- **Normal lock**: no permite que un hilo que lo tienen bloqueado vuelva a hacer un lock sobre él, produce deadlock.
- **Recursive lock**: sí permite que un hilo que lo tienen bloqueado vuelva a hacer un lock sobre él, incrementa una cuenta de control.
- **Error Check lock**: responde con un reporte de error al intento de un segundo bloqueo por el mismo hilo.

#### Overhead en el uso de locks

- Si dentro de las secciones críticas se incluyen porciones de código costosas se tendrá una gran degradación del rendimiento.
- Esto se puede contrarrestar usando la función **pthread_mutex_trylock**, la cual retorna el control informando si pudo hacer o no el lock: `int pthread_mutex_trylock(pthread_mutex_t *mutex_lock);`
- Esta función evita tiempos ociosos pero no siempre es viable usarlo.

### Sincronización por condición

#### Concepto

- Los locks pueden provocar busy-waiting.
- Una solución posible a esto es usar variables condición, las cuales usan sincronización por condición.
- Estas variables permiten que uno o más hilos se **autobloqueen** hasta que se alcance un cierto estado del programa.
- Cada variable condición tiene un predicado (estado). Cuando este predicado se convierte en True, la variable se usa para avisar a el o los hilos que están esperando por el cambio de estado de la condición.
- Una variable condición puede asociarse a varios predicados, aunque no es lo recomendado.
- Una variable condición siempre tiene un lock asociado. Cada hilo bloquea este lock y evalúa el predicado.
  - Si el predicado es falso, el hilo espera en la variable condición: se "duerme" por lo que no usa CPU y evita busy-waiting por completo.

#### Wait y Timedwait

```c
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
```

- Llamar a la función bloquea al hilo hasta que reciba una señal de otro hilo o sea interrumpido por el SO.
- Para poder invocarla, el hilo debe tener el control del mutex asociado.
- Una vez dormido en la vc, el mutex se libera.
- Cuando el hilo recibe una señal y se despierta, espera a que el mutex esté disponible para continuar su ejecución.
- Timedwait duerme al hilo una determinada cantidad de tiempo como máximo.

#### Signal y Broadcast

```c
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
```

- Llamar a la función despierta a un hilo que esté dormido en la vc (cuál hilo depende de las políticas de scheduling).
- Para poder invocarla, el hilo debe tener el control del mutex asociado.
- El mutex asociado se libera.
- Broadcast **despierta a todos** los hilos que están dormidos en la vc.

#### Init y Destroy

```c
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);
```

### Barreras

```c
int pthread_barrier_init(pthread_barrier_t *restrict barrier, const pthread_barrierattr_t *restrict attr, unsigned count);
int pthread_barrier_wait(pthread_barrier_t *barrier);
int pthread_barrier_destroy(pthread_barrier_t *barrier);
```

- Las barreras son puntos de sincronización que involucran a múltiples hilos.
- El hilo llamador se bloquea hasta que el número de hilos implicados en la barrera hayan alcanzado este punto.
- La cantidad de hilos asociados a la barrera se especifica en su inicialización.
-

### Semáforos

- Un semáforo es una estructura de datos que permite sincronizar hilos (tanto para exclusión mutua como para sinc. por cond.).
- Los tipos de datos y funciones para usar semáforos se encuentran en semaphore.h.
- Para declarar un semáforo se usa el tipo sem_t.
- Se pueden usar tanto para mutex como para sincronización por condición.
- Inicialización:

```c
int sem_init(sem_t *sem, int pshared, unsigned int value);
```

- Decrementar un semáforo (P):

```c
int sem_wait(sem_t *sem);
```

- Incrementar un semáforo (V):

```c
int sem_post(sem_t *sem);
```

### Planificación de hilos

- Si bien el SO es responsable de planificar la ejecución de los hilos, el programador puede influenciar esto usando los **atributos de planificación**.
- El scheduler mantiene una cola separada de hilos por cada prioridad definida.
- Al momento de seleccionar un hilo para ejecutar, se elige alguno que esté listo de la cola que tenga mayor prioridad.
- Si hay varios hilos posibles en la cola seleccionada, se elige uno de ellos de acuerdo a la política de scheduling.
- Para asignar y recuperar los **atributos de planificación**, tenemos las funciones:

```c
int pthread_attr_setschedparam(pthread attr_t *attr, const struct sched param *param);
int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched param *param);
```

- Para asignar y recuperar la prioridad min y max de una determinada política de scheduling:

```c
int sched_get_priority_min(int policy);
int sched_get_priority_max(int policy);
```

- La política de scheduling determina cómo se ejecutan y comparten recursos los hilos de una misma prioridad.
- Pthreads soporta tres políticas diferentes:
  - **SCHED_FIFO (First In First Out)**: una vez en ejecución, el hilo se ejecuta hasta que termina, se bloquea o hasta que un hilo de mayor prioridad pueda ejecutarse. Los hilos de la misma prioridad son ejecutados **en orden**.
  - **SCHED_RR (Round Robin)**: similar al anterior pero los hilos se ejecutan a lo sumo una determinada cantidad de tiempo (configurable).
  - **SCHED_OTHER**: política adicional no definida en el estándar. Su funcionamiento depende enteramente de la implementación.

---

<h1 align="center">Clase 4 - 4 de abril, 2025</h1>

## Diseño de algoritmos paralelos

### Idea

- Un algoritmo paralelo se puede ver como una receta que **indica cómo resolver un problema usando varias unidades de procesamiento**.
- Para desarrollar algoritmos de este tipo se suelen seguir los siguientes pasos:
  - Identificar porciones de trabajo (tareas) que pueden resolverse en paralelo.
  - Asignar tareas a procesos que se ejecutan en diferentes procesadores.
  - Distribuir datos de entrada, de salida e intermedios asociados con el programa.
  - Administrar accesos a datos compartidos.
  - Sincronizar procesos en diferentes etapas del programa.
- Los pasos **fundamentales** son:
  - Descomposición en tareas.
  - Mapeo de tareas a procesos.

## Etapa de descomposición en tareas

### Proceso

- Consiste en dividir el cómputo en partes pequeñas, **tareas**, de las cuales algunas o todas podrán ser ejecutadas en paralelo.
- Inicialmente se quiere definir un gran número de tareas pequeñas (grano fino) ya que provee mayor flexibilidad.
- Más adelante en el desarrollo, la partición original puede ser revisada y sus tareas aglomeradas para aumentar la granularidad.
- La descomposición se puede realizar de varias formas.
  - Tareas de igual código → paralelismo de datos o de dominio.
  - Tareas de código diferente → paralelismo funcional.

### Descomposición de datos

- Consiste en dividir los datos asociados al problema en porciones pequeñas, usualmente del mismo tamaño, y luego asociar el cómputo relacionado a las mismas para generar las tareas.
- Es decir, se llega a un número determinado de tareas donde cada una se ocupa de algunos datos y operaciones a realizar sobre éstos.
- Una operación puede necesitar datos de diferentes tareas, en cuyo caso hay comunicación y sincronización.
- Según la estructura de datos que se tenga, puede haber diferentes divisiones.

### Descomposición funcional

- Se enfoca en el cómputo a realizar más que en los datos.
- Divide el cómputo en tareas disjuntas y luego se examinan los datos.
- Los requerimientos de datos pueden ser disjuntos (caso ideal) o superponerse significativamente (peor caso, porque implica comunicación para evitar replicación de datos).
- La descomposición de datos es la más antigua y, a su vez, la más usada.
- De todas formas, la descomposición funcional tiene valor como una forma diferente de pensar los problemas:
  - Enfocarse en el cómputo a realizar facilita la estructuración del programa y el descubrimiento de oportunidades de optimización (situación que no es tan obvia cuando uno se enfoca en los datos).

### Grafo de Dependencias de Tareas (GDT)

- Si el problema lo permite, todas las tareas serán independientes.Este es el caso ideal ya que todas podrían computarse a la vez.
- En general esto no ocurre y existe algún tipo de dependencia entre las tareas.
- Un Grafo de Dependencias de Tareas (GDT) puede ser útil para expresar las dependencias entre las tareas y su orden relativo.
  - Es un grafo acíclico dirigido en el que los nodos representan las tareas y las aristas indican las dependencias entre las mismas.
  - El grafo puede ser disconexo e inclusive no tener aristas (en este último caso no habría dependencias).

### Granularidad

- La granularidad de la descomposición en tareas indica el número y tamaño de las tareas:
  - **Grano fino**: muchas tareas pero pequeñas.
  - **Grano grueso**: pocas tareas pero grandes.

### Métricas de concurrencia

- El **grado de concurrencia** de la descomposición indica cuántas tareas se ejecutan en paralelo.
- Dado que este número puede variar durante la ejecución, se suele preferir analizar el **máximo grado de concurrencia** alcanzable por una determina descomposición.
- También tenemos el **grado de concurrencia promedio** que representa el número promedio de tareas que pueden ejecutarse en simultáneo **durante todo el programa**.
  - Se relaciona directamente con la cantidad de procesadores activos y con el balance de carga del programa.
  - Cuanto mayor sea, mejor.
- Una característica del grafo de dependencias de tareas que determina el grado de concurrencia promedio para una determinada granularidad es el **camino crítico**.
  - Se trata del camino dirigido más largo entre un nodo inicial (nodo que no recibe aristas) y un nodo final (nodo del que no salen aristas).
  - La suma de los pesos de los nodos que integran el camino crítico se conoce como **longitud del camino crítico**. Si los pesos indican el tiempo requerido por una tarea, entonces representa el tiempo mínimo requerido para resolver el problema.
  - Un camino crítico más corto favorece a un mayor grado de concurrencia.
- `Grado de concurrencia promedio = Peso total / Longitud del camino crítico`

### Aglomeración de tareas

- Consiste en analizar si conviene combinar/aglomerar varias tareas en una para obtener una granularidad de grano más grueso.
- En general, el número final de tareas como resultado de la aglomeración debería ser **igual al número de procesadores a usar**.
- Hay 3 objetivos que guían las decisiones de aglomeración:
  - **Incremento de la granularidad**: al combinar varias tareas relacionadas, se elimina la necesidad de comunicar datos entre ellas.
  - **Preservación de la flexibilidad**: al combinar varias tareas se puede limitar la escalabilidad del algoritmo. Si un algoritmo es capaz de crear un número variable de tareas, entonces posee un mayor grado de portabilidad y escalabilidad.
  - **Reducción de costos de desarrollo**: en ocasiones, el costo desde el punto de vista del proceso de ingenería de software, puede ser muy elevado para la ganancia asociada.

### Técnicas de descomposición en tareas

#### Recursiva

- En general se ajusta muy bien a los problemas que se pueden resolver mediante la estrategia **divide and conquer**.
- El problema inicial es dividido en un conjunto de subproblemas independientes. Luego, cada uno de estos subproblemas son recursivamente descompuestos en otros subproblemas independientes más pequeños hasta alcanzar una determinada granularidad.
- En ocasiones, puede requerirse alguna fase de combinación de resultados parciales.
- Ejemplo: Ordenación de arreglo por método Quicksort.
- Es una técnica de propósito general.

#### Basada en los datos

- Se suele usar en problemas que operan sobre grandes estructuras de datos.
- Posee dos pasos:
  - Particionar los datos a procesar.
  - Usar esta partición para inducir una descomposición del cómputo en tareas.
- El particionamiento de los datos se puede realizar de diferentes maneras: se debe analizar las diferentes variantes y elegir la que lleve a una descomposición natural y de buen rendimiento.
- Es una técnica de propósito general.
- Posee dos subtipos: **datos de salida** y **datos de entrada**.

##### Datos de salida

- Resulta natural cuando cada elemento de la salida del programa, los resultados, se puede calcular en forma independiente como función de los datos de entrada.
- Una partición de los datos de salida lleva inmediatamente a una descomposicíón en tareas, donde a cada tarea se le asocia el cómputo relacionado a la porción asignada.
- En general, una determinada descomposición de datos lleva a una dada descomposición del cómputo en tareas, pero puede haber más de una opción.

##### Datos de entrada

- Particionar los datos de salida no siempre es posible. Por ejemplo:
  - Cuando se computa el máximo, el mínimo o la suma de una lista de números: la salida es un **único número**.
  - Cuando se ordena un vector de números, los elementos individuales de la salida no se pueden determinar de antemano.
- En estos casos, resulta natural particionar los datos de entrada e inducir concurrencia a partir de ellos.
- A cada tarea se le asigna una porción de los datos de entradas y será responsable de realizar todos los cómputos asociados a la misma. A veces se puede requerir de algun paso posterior de reducción de salidas parciales.
- Ejemplos:
  - Contar ocurrencias en un vector.
  - Búsqueda de un elementos en un vector.
  - Descomposición estática para realizar una ordenación.

#### Exploratoria

- Útil en problemas cuya solución involucra una búsqueda en un espacio de soluciones.
- Para realizar la descomposición se particiona el espacio de búsqueda en porciones más pequeñas y se realiza una búsqueda concurrente en cada una de ellas hasta encontrar la solución objetivo.
- Es una técnica de propósito específico.
- Ejemplos:
  - Problemas de optimización (buscar la mejor configuración para un determinado conjunto de parámetros).
  - Juegos (puzzle-N, ajedrez, entre otros).

#### Especulativa

- Se usa cuando un programa podría tomar uno de varios caminos que implican un costo computacional alto, pero la decisión depende de la salida de algún cómputo anterior.
- Pensar en un case con múltiples opciones que son evaluadas al mismo tiempo pero antes de tener el valor de la entrada. Cuando la entrada del case está disponible, se descartan las opciones incorrectas y se continúa la ejecución.
- Ejemplo: simulación de eventos discretos.
- Diferencia con la técnica exploratoria:
  - En especulativa, la entrada de una bifurcación que lleva a múltiples nuevas tareas es desconocida.
  - En cambio con exploratoria, la salida de las múltiples tareas que salen de una bifuración son desconocidas.

#### Híbrida

- Las técnicas anteriores se pueden combinar.
- En ocasiones, un programa se estructura en múltiples etapas y cada etapa puede ser descompuesta de forma diferente.
- Ejemplo: búsqueda del mínimo en un vector.
  - Una descomposición recursiva pura podría generar una cantidad excesiva de tareas si n >> P.
  - Una mejor opción consiste en realizar primero una **descomposición de datos** y luego una **recursiva**.

## Etapa de mapeo de tareas a procesos

### Idea

- Este paso consiste en asignar tareas a procesos del programa.
- Para hacer un buen mapeo, se deben tener en cuenta:
  - Forma de generación.
  - Tamaño y conocimiento del mismo.
  - Volúmen de datos asociado.
- El mapeo debe llevarse a cabo buscando que el tiempo requerido para completar las tareas sea el mínimo. Para esto se consideran dos estrategias:
  - Asignar tareas independientes en diferentes procesadores para lograr un mayor grado de concurrencia.
  - Asignar tareas que se comunican frecuentemente en el mismo procesador reducir overhead y mejorar localidad.
- Estas 2 estrategias claramente entran en conflicto entre sí y la clave está en encontrar el balance adecuado.
- El problema de encontrar un mapeo óptimo es NP-completo: esto significa que no existe un algoritmo de complejidad polinomial que evalúe los diferentes compromisos entre las estrategias en el caso general y determine cuál es el mejor.
- Sin embargo, existen heurísticas para determinadas clases de problema que suelen dar buen resultado.

### Forma de generación

- Las tareas que constituyen el programa se pueden generar de dos formas:
- **Estática**:
  - Las tareas que se generan se conocen pre-ejecución.
  - Es fundamental conocer las características de las tareas (generación, tamaño, volumen de datos asociado).
  - Para casos complejos se emplean heurísticas (mapeo óptimo es NP-completo).
  - En general los algoritmos son más fáciles de diseñar y programar.
  - Ejemplos: multiplicación de matrices, búsqueda del mínimo en una lista de números.
- **Dinámica**:
  - Las tareas se generan durante la ejecución, no se conoce de antemano cuál será la cantidad final.
  - Si no se conoce de antemano el tamaño de las tareas, el mapeo dinámico suele dar mejor resultado.
  - Si el volumen de datos asociado a cada tarea es grande pero el cómputo no es significativo, un mapeo dinámico podría incurrir en un alto overhead por la migración de datos.
  - Ejemplo: quicksort recursivo.

### Tamaño y conocimiento del mapeo

- Las tareas del porograma pueden ser **uniformes** o **no uniformes**.
  - Son uniformes si cada tarea requiere aproximadamente el mismo tiempo de cómputo. Ejemplo: multiplicación de matrices.
  - Son no uniformes si cada tarea puede tardar tiempos muy distintos. Ejemplo: quicksort recursivo.
- Conocer el tamaño de las tareas previo a la ejecución es otro factor que puede influir en el mapeo. Por ejemplo: en la multiplicación de matrices conocemos el tamaño de cada tarea previo a la ejecución.

### Volúmen de datos asociado

- Muchas veces tiene que ver con el nivel de granularidad elegido.
- A su vez, la granularidad impacta directamente en la relación cómputo-comunicación.
  - Usualmente, con bajos niveles de comunicación se tiende a afinar la granularidad y a asignar un menor volumen de datos por proceso.
  - Sin embargo, cuando hay mucho intercambio de datos, se suele optar por aumentar la granularidad o emplear memoria compartida (si la arquitectura lo permite).

## Métodos para reducir overhead de las interacciones

### Motivación

Reducir el overhead asociado a las interacciones entre procesos es un factor clave para mejorar la eficiencia de los programas paralelos.

### Métodos

1. **Minimizar volumen de datos intercambiados**: A mayor volumen de datos intercambiados, mayor tiempo de comunicación.
2. **Minimizar frecuencia de las interacciones**: Cada interacción tiene un costo inicial de preparación. Siempre que sea posible, conviene combinar varias comunicaciones en una sola.
3. **Minimizar competencia entre recursos y zonas críticas (hotspots)**: Evitar posibles cuellos de botella mediante el uso de técnica descentralizada. Replicar datos si es necesario.
4. **Solapar cómputo con comunicaciones**: Mediante el uso de operaciones no bloqueantes en pasaje de mensajes y técnicas de multi-hilado y prebúsqueda en memoria compartida.
5. **Replicar datos o cómputo**: si permite reducir las interacciones (mensajes o sincronización).
6. **Usar operaciones de comunicación colectiva**.
7. **Solapar comunicaciones con otras comunicaciones**: siempre y cuando el hardware de soporte lo permita, solapar diferentes comunicaciones puede reducir el overhead.

## Modelos de algoritmos paralelos

### Concepto

- Un modelo de algoritmo representa una estructura usual de código que combina técnicas de descomposición de tareas y mapeo junto a la aplicación de métodos para minimizar overhead.
- Existen varios modelos:
  - Master-Slave.
  - Pipeline.
  - Single Program Multiple Data.
  - Divide and Conquer.

### Master-Worker

- El proceso maestro es el que genera trabajo y se lo asigna a los workers.
- Si el maestro puede estimar de antemano el tamaño de las tareas, un mapeo estático es mejor. Ejemplo: multiplicación de matrices.
- Caso contrario, mapeo dinámico es mejor: tareas pequeñas son asignadas a los workers (posiblemente) en múltiples instancias. Ejemplo: ordenación de un vector.
- Si las tareas son muy pequeñas o los workers muy rápidos, el master se puede volver un cuello de botella: la granularidad de las tareas debe ser elegida de forma tal que el tiempo de procesar la tarea sea mucho mayor que su comunicación o sincronización asociada.
- Se puede generalizar a varios niveles.
- Se puede usar tanto en memoria compartida como pasaje de mensajes.

### Pipeline

- El cómputo consiste en una secuencia de procesos.
- Los datos se suelen particionar y pasar entre los procesos, donde cada uno realiza una tarea determinada sobre ellos.
- Se suele organizar en forma de arreglo lineal o multi-dimensional. A veces, usando árboles o grafos.
- Se puede ver como una cadena de productores y consumidores.
  - Cada proceso consume los datos que genera el anterior en el pipe.
  - Al mismo tiempo, produce los datos que serán consumidos por el siguiente proceso
- El balance de carga depende de la **granularidad** de las tareas:
  - A mayor granularidad, más tiempo tardará el pipeline en llenarse (paralelismo ideal).
  - A menor granularidad, mayor interacción entre los procesos del pipeline.

### Single Program Multiple Data

- Cada proceso realiza el mismo cómputo sobre una **porción de datos diferentes**.
  - Esto se logra vía condicionales.
- En general, la carga de trabajo es proporcional a la cantidad de datos asignados a un proceso. Esto trae dificultades en problemas irregulares o donde la arquitectura de soporte es hetereogénea.
- El cómputo puede tener varias fases, las cuales son intercaladas con comunicación/sincronización.
- Se puede usar tanto en memoria compartida como pasaje de mensajes:
  - En MC suele ser más simple.
  - En pasaje de mensajes:
    - Cuando el espacio de direcciones está particionado se suele tener un mayor control sobre la ubicación de los datos y por ende se logra una mayor localidad de datos.
    - El overhead de las comunicaciones puede ser aliviado mediante el uso de operaciones no bloqueantes, siempre y cuando las dependencias lo permitan.

### Divide and Conquer

- Consiste en 2 fases, dividir y consquistar:
  - **Dividir**: se particiona sucesivamente el problema en sub-problemas más chicos hasta obtener la granularidad deseada.
  - **Conquistar**: se resuelven los subproblemas en forma independiente.
- A veces requiere una tercer fase de combinación de resultados parciales para obtener el resultado final.
- Ejemplo: Merge-sort.

---

<h1 align="center">Clase 5 - 8 de abril, 2025</h1>

## Análisis de rendimiento en sistemas paralelos

### Definición de rendimiento

- Dependiendo del contexto, qué cosa rinde mejor que otra depende de qué métricas son las que se valoren.
- Las dos métricas más comunes son:
  - **Tiempo para completar la tarea**: tiempo de ejecución, latencia, tiempo de respuesta, u otras derivadas.
  - **Tareas completadas por unidad de tiempo**: throughput, ancho de banda.
- Los sistemas paralelos suelen ser sistemas dedicados, que priorizan las **métricas basadas en tiempo**.

### Tiempos de ejecución

- Un algoritmo secuencial se suele evaluar por su tiempo de ejecución. En general, es posible encontrar alguna ley asintótica del tiempo de ejecución **en función del tamaño de datos de entrada**.
- El tiempo de ejecución de un programa paralelo no sólo depende del tamaño de los datos de entrada sino también de:
  - El número de procesadores.
  - Los parámetros de comunicación de la arquitectura de soporte.
- Por ende, el análisis de TE de un programa paralelo se debe realizar a nivel de sistema paralelo: combinación de algoritmo paralelo y contexto de hardware y software.
- El tiempo de ejecución secuencial (Ts) es el tiempo que transcurre desde el **inicio hasta el fin de la ejecución sobre una máquina usando una única unidad de procesamiento**.
- El tiempo de ejecución paralela (Tp) resume la diferencia de tiempo entre que la primera tarea que comienza hasta que la última tarea haya completado su trabajo.

#### Fuentes de overhead

- Usando el doble de recursos, uno creería que el programa paralelo se ejecutará en la mitad del tiempo. Sin embargo, en la práctica, esto no suele pasar.
- Esto se debe a que existen factores que generan **overhead** en los programas paralelos e impiden una mejora proporcional al aumento de la arquitectura. Estos factores son:
  - Ocio.
  - Interacción entre procesos.
  - Cómputo adicional.

### Speedup

- Es una métrica que refleja el beneficio de usar procesamiento paralelo para resolver un problema particular comparado a realizarlo en forma secuencial.
- $S_p(N) = \frac {T_s(N)} {T_p(N)}$
- Indica cuántas veces más rápido se pudo resolver el problema usando el algoritmo paralelo vs el secuencial, usando p unidades de procesamiento y tamaño de problema N.
- El algoritmo secuencial a usar en la comparación debe ser **el mejor**, el más rápido, para el problema dado.

#### Límites

- Si $S_p(N) < 1$, entonces el algoritmo paralelo es **peor** que el secuencial. Caso contrario, el paralelo es **mejor**.
- Si $S_p(N) = p$, entonces el speedup es **óptimo, lineal, o perfecto**.
- Si $S_p(N) > p$, entonces el speedup es **superlineal**.
  - Esto puede ocurrir por ejemplo si el algoritmo paralelo realiza menos trabajo que el algoritmo secuencial. Ejemplo: Búsqueda DFS.
  - También puede ocurrir si combinamos características de hardware y distribución de los datos del algoritmo paralelo que ponen en desventaja al algoritmo secuencial.

#### Potencia Cómputo Total

- Las arquitecturas de hardware no siempre son homogeneas, es decir que todas las unidades de procesamiento son igual de rápidas.
- También tenemos arquitecturas de hardware heterogeneas, y en este caso el Speedup se debe calcular considerando la Potencia Cómputo Total (pct) en vez de usar el número de procesadores (p).
- $pct = \sum_{i=0}^{p-1} pcr_i$
- $pcr_i = \frac{p_i}{p_m}$
- Donde:
  - **$pct$**: Potencia de Cómputo Total
  - **$pcr$**: Potencia de Cómputo Relativa
  - **$p_i$**: Potencia del procesador i
  - **$p_m$**: Potencia del mejor procesador

### Eficiencia

- Médida de la fracción de tiempo en la cual las unidades de procesamiento son usadas de forma útil.
- $E_p(N) = \frac{S_p(N)}{S_{opt}}$
- En arquitecturas homogeneas, el speedup óptimo es igual a p, mientras que en heterogeneas es igual a la potencia de cómputo total.
- Si $S_p(N) = p$, entonces $E_p(N) = 1$.
- Si $S_p(N) \leq p$, entonces $E_p(N) \leq 1$.
- $E_p(N)$ siempre es mayor a 0. Por ende:
- **$0 < E_p(N) \leq 1$**.

### Overhead total

- Diferencia entre la suma del tiempo requerido por **todas** las unidades de procesamiento y el del mejor algoritmo secuencial para resolver el mismo problema empleando una única de unidad de procesamiento.
- $𝑂𝑇_𝑝(N) = 𝑝 \times 𝑇_p(N) − 𝑇_s(N)$

### Overhead de las comunicaciones

- Relación entre el tiempo requerido por las comunicaciones de nuestra solución y el tiempo total que esta requiera.
- $OC_p(N) = \frac{Tcomm_p(N)}{T_p(N)} \times 100$

### Ley de Amdahl

- Esta ley permite estimar el speedup alcanzable en programas paralelos que poseen bloques de ejecución secuenciales, es decir que no se pueden paralelizar.
- Dada una fracción $f$ donde $0 \leq f \leq 1$ de un programa paralelo que debe ser ejecutada secuencialmente, entonces el tiempo de ejecución paralela se calcula como:
- $T_p(N) = f \times T_s(N) + \frac{(1 - f) \times T_s(N)}{p}$
- Por ende el speedup se reescribe:
- $S^A_p(N) = \frac{T_s(N)}{f \times T_s(N) + \frac{(1 - f) \times T_s(N)}{p}} = \frac{1}{f + \frac{1 - f}{p}}$
- Incluso con un número ilimitado de procesadores, el speedup está limitado a $\frac{1}{f}.$
- Por ejemplo, con 5% de ejecución secuencial (f = 0.05) en un programa paralelo, el **máximo Speedup alcanzable será 20**, sin importar el número de unidades de procesamiento que podamos emplear.

### Escalabilidad

- Este concepto hace referencia a la capacidad del sistema de mantener un nivel de eficiencia fijo a medida que aumenta tanto el número de unidades de procesamiento como el tamaño de problema (N).
  - Si estas dos condiciones se cumplen, el sistema se dice escalable.
- También se puede ver a la escalabilidad como una medida de la capacidad de incrementar el Speedup en forma proporcional al número de unidades de procesamiento empleadas.
- La escalabilidad se puede clasificar en fuerte o débil, según qué ocurre al incrementar el número de unidades de procesamiento:
  - **Fuerte**: Al hacer eso, **no resulta necesario** aumentar el tamaño de problema para mantener la eficiencia en un valor fijo.
  - **Débil**: Al hacer eso, **resulta necesario** también aumentar el tamaño de problema para mantener la eficiencia en un valor fijo.

### Ley de Gustafson

- El incremento en el Speedup por un tamaño mayor de problema no es percibido por la Ley de Amdahl.
- En los 80, Gustafson observó que:
  - Un multiprocesador más grande usualmente permite resolver un problema de mayor tamaño en un tiempo de ejecución determinado (escalabilidad) → el tamaño de problema seleccionado depende frecuentemente del número de unidades de procesamiento disponibles.
  - Al incrementar el tamaño del problema y el número de unidades de procesamiento para mantener el tiempo de ejecución constante, la fracción secuencial de los programas se mantiene fija o no crece en forma proporcional al tamaño de la entrada.
- Por lo tanto, asumir que el tamaño de problema es fijo resulta tan válido
  como que el tiempo de ejecución paralela lo es.
- Basándose en sus observaciones, Gustafson re-escribió la ecuación para estimar el máximo speedup alcanzable (conocido como Speedup escalado).
- La ley dice lo siguiente:
  - Dada una fracción $f', 0 \leq f' \leq 1$, de un programa paralelo que debe ser ejecutado secuencialmente pero que no crece en forma proporcional a N, el speedup escalado se calcula como:
    - $S^S_p(N) = \frac{T_s(N)}{T_p(N)} = \frac{f' \times T_p(N) + (1 - f') \times T_p(N) \times p}{T_p(N)} = p + (1 - p) \times f'$
  - Esta versión hace 2 suposiciones:
    - $T_p(N)$ se mantiene constante.
    - $f' \times T_p(N)$ no escala en forma proporcional al aumento de p y N.
  - Por ejemplo, con 5% de ejecución secuencial (f' = 0.05) y 20 procesadores, el speedup alcanzable sería $20 + (1 − 20) × 0.05 = 19.05$ en lugar de $\frac{1}{0.05 + \frac{1 - 0.05}{20}} = 10.26$, el cual se obtendría con Amdahl.

### Comparación Amdahl y Gustafson

| Característica     | Escalabilidad fuerte (Amdahl)                                                               | Escalabilidad débil (Gustafson)                                                                                                                                                                |
| ------------------ | ------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Tamaño de problema | Se mantiene fijo a medida que crece la cantidad de procesadores.                            | El tamaño del problema por procesador se mantiene fijo a medida que se incrementa la cantidad de procesadores → El tamaño total del problema es proporcional al número de procesadores usados. |
| Objetivo           | Resolver el mismo problema de forma más rápida                                              | Resolver un problema más grande en la misma cantidad de tiempo                                                                                                                                 |
| Escalado perfecto  | Se logra cuando el problema se resuelve en 1/P unidades de tiempo, comparado al secuencial. | Se logra cuando se resuelve un problema P veces más grande en la misma cantidad de tiempo que el secuencial.                                                                                   |

### Desbalance de carga

- En arquitecturas heterogéneas se puede medir el desbalance de carga:
- $D = \frac{máximo - mínimo}{promedio}$, donde:
  - máximo hace referencia al tiempo que tardó la tarea que **más** tiempo le llevó terminar.
  - mínimo hace referencia al tiempo que tardó la tarea que **menos** tiempo le llevó terminar.
  - promedio hace referencia al tiempo promedio que tardaron entre todas las tareas.
- Si $D = 0$, entonces todas las unidades de procesamiento tomaron el mismo tiempo, lo cual es poco común.
- Se busca que D esté lo más cerca posible de 0.

### Cómo optimizar algoritmos

#### Principio de Pareto

- Este principio dice: `90% del tiempo de ejecución se dedica a ejecutar 10% del código.`
- En otras palabras, nos dice que deberíamos enfocarnos en optimizar las partes del código que más tiempo están tardando, ya que mejorar estas partes incluso un poco es mucho más productivo que mejorar muchísimo partes que no toman mucho tiempo.
- En programas pequeños, uno puede identificar fácilmente donde está la sección de código que más demanda computacional tiene (a esto se
  lo conoce como **hotspot**):
  - Observando y analizando el código (habitualmente los bucles).
  - Instrumentando el código “a mano” (tomar tiempos de diferentes secciones).
- En programas de complejidad mediana o alta, se puede realizar un proceso llamado **profiling** (o perfilado).
  - Mediante herramientas de software, se analiza y mide el rendimiento de una aplicación o programa → el objetivo principal es identificar los puntos críticos de la aplicación que consumen la mayor cantidad de tiempo de ejecución o recursos del sistema.
  - Hay muchos profilers diferentes, según el lenguaje, el aspecto a mejorar, la modalidad de uso, entre otros. Ejemplos:
    - C: gprof, Valgrind, VTune.
    - Python: cProfile, profile.
    - Julia: profile.

### Cómo medir tiempos de ejecución adecuadamente

- Usualmente interesa analizar la mejora lograda en determinada parte del programa.
- En la práctica, el tiempo de ejecución no siempre se considera desde el que programa empieza hasta que termina.
- Podemos eliminar del timing:
  - Reserva de memoria.
  - Lectura de datos de entrada.
  - Impresión de datos de salida.
  - Liberación de memoria.
- Aunque la solución paralela involucre p tareas, $T_p(n)$ debe ser un único valor que contemple el tiempo que transcurre desde que la primera tarea comenzó a ejecutar hasta que la última haya completado su trabajo: Tener en cuenta que este tiempo puede hacer referencia a una determinada parte del programa.
- En ocasiones, **no todas las tareas comienzan y terminan al mismo tiempo**: Para asegurarnos una medición correcta, podemos usar barreras.
- Otro factor a tener en cuenta es la variabilidad en las mediciones:
  - Se recomienda repetir las pruebas un determinado número de veces y calcular el promedio o la mediana.
  - En ocasiones, también puede ser de interés reportar el tiempo mínimo y el máximo.

---

<h1 align="center">Clase 6 - 11 de abril, 2025</h1>

## Programación en memoria compartida - Open Multiprocessing (OpenMP)

### OpenMP - Definición

- Estándar para programación paralela **basado en directivas** que está disponible en C, C++, Fortran.
- Posee 3 componentes principales:
  - Directivas:
    - Son constructores de alto nivel que liberan al programador del manejo explícito de hilos.
    - Proveen soporte para concurrencia, sincronización y manejo de datos, obviando el uso explícito de locks, variables condición alcance de los datos e inicialización de threads.
    - Son traducidas a código Pthreads.
  - Funciones de librerías.
  - Variables de entorno.
- Fue diseñado con el objetivo de diseñar un estándar para programas de memoria compartida que pudieran ser desarrollados con mayor nivel de abstracción.
- Sigue una filosofía de desarrollo incremental: nos permite convertir nuestra aplicación 100% secuencial en una paralela de forma relativamente simple.

### Sintáxis básica

```c
#pragma omp nombre_directiva [lista de cláusulas]
```

### Modelo Fork-Join

- El programa empieza con un único hilo, el hilo **master**.
- Al encontrar un constructor paralelo (o directiva **parallel**), el hilo master crea un grupo de hilos → **Fork**.
- El bloque encerrado por el constructor de la región paralela es ejecutado en paralelo por todos los hilos.
- Cuando el conjunto de hilos termina el bloque paralelo, se sincronizan y terminan, continuando solamente el hilo master → **Join**.

### Constructor parallel

#### Definición

- Sintaxis:

```c
#pragma omp parallel [lista de cláusulas] {
    ...
}
```

- Constructor principal.
- Permite especificar un bloque de código que será ejecutado en paralelo, dentro de la **región paralela**.
- Asegura la creación de un equipo de hilos, pero la distribución del trabajo dentro de la región paralela es responsabilidad del programador.
- Dentro de la región paralela, cada hilo mantiene su propio ID único (el ID 0 siempre corresponde al hilo master).
- Al final de la región paralela, hay una **barrera implícita**: sólo el hilo master continúa con la ejecución.

#### Private y firstprivate

- Sintaxis:

```c
#pragma omp parallel private(lista_de_variables) {
    ...
}
```

- Crea una copia local **no inicializada** a cada hilo de cada variable especificada en la lista, respetando su tipo y tamaño.
- Esta copia local sólo puede ser accedida y modificada por el hilo que la posee.
- Cambios en esa copia no afectan al valor fuera del bloque paralelo.
- Posee una variante **firstprivate** que es igual, solo que sí hereda el valor original de la variable antes del bloque paralelo, es decir que sí está inicializada.

#### Shared y default

- Sintaxis:

```c
#pragma omp parallel shared(lista_de_variables) {
    ...
}

#pragma omp parallel default(shared | private | none) {
    ...
}
```

- Las variables que son compartidas entre todos los hilos se especifican con la cláusula **shared**.
- Por defecto, todas las variables son compartida. Para alterar este comportamiento, se puede usar la cláusula **default**.

#### Num_threads

- Sintaxis:

```c
#pragma omp parallel num_threads(T) {
    ...
}
```

- Crea T hilos.
- En caso de ausencia, el número de hilos a crear lo determina la variable de entorno **OMP_NUM_THREADS**.

#### Cláusula if

- Sintaxis:

```c
#pragma omp parallel if (cond) {
    ...
}
```

- Solo ejecuta la región en paralelo si se cumple la condición.
- Si no, la ejecuta de forma secuencial, es decir que solo el hilo main ejecutará esa región y nadie más.

### Constructor for

- Sintaxis:

```c
#pragma omp for [lista de cláusulas]
for ( init_exp; check_exp; mod_exp)
```

- Divide las iteraciones de un bucle entre los hilos (**paralelismo de datos**).
- Posee varias restricciones:
  - Las iteraciones deben ser **independientes** entre sí.
  - El n° de iteraciones debe ser conocido de antemano.
  - La variable índice se vuelve privada por defecto y los hilos dentro del bucle no la pueden modificar.
  - No se puede usar `break` dentro de las iteraciones.
- El bucle paralelo finaliza con una sincronización implícita entre todos los hilos que lo integran.

#### Acceso

- **Private**: cada hilo tiene su propia copia de la variable.
- **Firstprivate**: como private, pero inicializada con el valor original.
- **Lastprivate**: funciona como private, sólo que la variable original queda con el valor de la última iteración del bucle

#### Reduction

- Sintaxis:

```c
#pragma omp for reduction(operador:variable)
```

- Aplica una reducción (suma, producto, máximo, etc.) entre todos los hilos.
- Cada hilo tiene una copia privada de variable, y al final se combinan todas con el operador op.
- Evita las condiciones de carrera comunes cuando se quiere acumular resultados.

#### Nowait

- Sintaxis:

```c
#pragma omp for nowait
```

- Evita la barrera implícita al final del bucle for.
- Por defecto, todos los hilos esperan a que todos terminen el bucle, pero si se usa nowait, siguen de largo.

#### Schedule

- Sintaxis:

```c
#pragma omp for schedule(type, chunk_size)
```

- Controla cómo se **reparten** las iteraciones del bucle entre los hilos.
- `type` puede ser de varios tipos:
  - **static**: Divide las iteraciones en bloques fijos (por defecto), asignados de entrada.
  - **dynamic**: Cada hilo pide más trabajo cuando termina el anterior.
  - **guided**: Similar a dynamic pero con bloques que se van achicando.
  - **auto**: El compilador decide.
  - **runtime**: Se define por la variable de entorno OMP_SCHEDULE.
- `chunk_size` es opcional y ajusta cuántas iteraciones recibe cada hilo por bloque.

![Representación visual de los tipos de schedule](https://i.imgur.com/OfZD8hc.png)

### Constructor sections

- Sintaxis:

```c
#pragma omp sections [lista de cláusulas] {
    #pragma omp section { // bloque estructurado 1
        ...
    }
    #pragma omp section { // bloque estructurado 2
        ...
    }
}
```

- Útil para la distribución de trabajo no-iterativo. Por ejemplo, paralelismo funcional.
- Cada bloque de código indicado por la directiva section es independiente de los demás y es ejecutado **una sóla vez por un único hilo**, pudiendo hacerlo en paralelo con el resto de los hilos.
- Existe una barrera implícita al final de sections.
- Cláusulas disponibles:
  - shared, private, firstprivate, lastprivate.
  - reduction.
  - nowait.
- Si hay más hilos que secciones, los hilos extra no hacen nada (buscar en internet para + detalle).

### Combinación de directivas

- Las directivas for y sections se pueden combinar con la directiva parallel.

```c
#pragma omp parallel default (private) shared (n) {
    #pragma omp for
    for (i = 0; i < n; i++) {
        ...
    }
}

// Es equivalente a:

#pragma omp parallel for default (private) shared (n)
for (i = 0; i < n; i++) {
    ...
}
```

```c
#pragma omp parallel {
    #pragma omp sections {
        #pragma omp section {
            tareaA();
        }
        #pragma omp section {
            tareaB();
        }
        #pragma omp section {
            tareaC();
        }
    }
}

// Es equivalente a:

#pragma omp parallel sections {
    #pragma omp section {
        tareaA();
    }
    #pragma omp section {
        tareaB();
    }
    #pragma omp section {
        tareaC();
    }
}
```

### Constructor single

- Sintaxis:

```c
#pragma omp single [lista de cláusulas] {
    ...
}
```

- Permite que un bloque de código sea ejecutado por **un único hilo** dentro de una región paralela.
- El bloque es ejecutado solo por el primer hilo que llega a ese punto de ejecución, el resto de los hilos espera al final del bloque (hay una barrera implícita).
- Permite las cláusulas:
  - private.
  - firstprivate.
  - nowait.

### Constructor master

- Sintaxis:

```c
#pragma omp master {
    ...
}
```

- Igual que single pero asegura que el único hilo que entra el bloque de código es el main.
- A diferencia de single, no hay barrera implícita al final del bloque.

### Constructor barrier

- Sintaxis:

```c
#pragma omp barrier
```

- Implementa un punto de sincronización global entre todos los hilos de un equipo: una barrera.
- Puede causar deadlock.
- Incide en el rendimiento.

### Constructor critical

- Sintaxis:

```c
#pragma omp critical [nombre] {
    ...
}
```

- Permite implementar secciones críticas en forma sencilla.
- Garantiza que, en cualquier punto de ejecución del programa, a lo sumo un hilo estará dentro de la sección crítica nombre.
- Si un hilo alcanza un bloque critical y ya hay otro en la misma, el hilo **espera** a que la sección crítica se libere.
- El nombre es opcional pero se recomienda fuertemente que cada sección crítica tenga su propio nombre único para evitar problemas.

### Constructor ordered

- Sintaxis:

```c
#pragma omp ordered {
    ...
}
```

- Útil para casos en que resulta necesario ejecutar cierto segmento de código en el mismo orden en que lo haría la versión secuencial.
- Se usa en el ámbito de una directiva for o parallel for.

### Directiva flush

- Sintaxis:

```c
#pragma omp flush [(lista de variables)]
```

- Representa un punto de sincronización de la memoria.
- Todas las escrituras pendientes en memoria principal son asentadas.
- Todas las lecturas pendientes son realizadas desde memoria principal.
- No suele ser muy usada ya que las siguientes directivas incluyen un flush implícito:
  - Barrier.
  - Entrada y salida de:
    - critical.
    - ordered.
    - parallel.
    - parallel for.
    - parallel sections.
  - Salida de:
    - For.
    - Sections.
    - Single.
- Excepciones:
  - La cláusula nowait excluye a flush.
  - Tampoco está presente a la entrada de:
    - For.
    - Sections
    - Single.
  - Ni a la entrada o salida de master.

### Funciones generales

#### Básicas

- `void omp_set_num_threads(int num_threads)`: Setea el valor de la variable de entorno OMP_NUM_THREADS, determinando el número de hilos que serán generando en las siguientes regiones paralelas que no especifiquen la clausula num_threads.
- `int omp_get_num_threads()`: Retorna el número de hilos de la región paralela actual.
- `int omp_get_max_threads()`: Retorna la cantidad máxima de hilos que podrían generarse en una región paralela.
- `int omp_get_thread_num()`: Retorna el ID del hilo que la invocó dentro de la región paralela actual.
- `int omp_get_num_procs()`: Retorna el número de procesadores disponibles en el sistema.
- `int omp_in_parallel()`: Retorna True si el hilo que la invocó está dentro una región paralela, si no False.
- ``:

#### Creación de hilos

- `void omp_set_dynamic(int dynamic_threads)`: Habilita o inhabilita el ajuste dinámico del número de hilos a generar en las próximas regiones paralelas.
- `int omp_get_dynamic()`: Retorna True si el ajuste dinámico del número de hilos está habilitado, si no False.
- `void omp_set_nested(int nested)`: Habilita o inhabilita el paralelismo anidado.
- `int omp_get_nested()`: Retorna True si el paralelismo anidado está habilitado, si no False.

#### Planificación de hilos

- `void omp_set_schedule(omp_sched_t kind, int chunk)`: Permite configurar dinámicamente la planificación a usar cuando se especifica runtime en la clausula schedule.
- `void omp_get_schedule(omp_sched_t * kind, int * chunk)`: Retorna la planificación que se usará cuando se especifica runtime en la clausula schedule.

#### Uso explícito de locks

- El tipo de dato para los locks es omp_lock_t y las funciones disponibles son:
- `void omp_init_lock(omp_lock_t *lock)`
- `void omp_destroy_lock(omp_lock_t *lock)`
- `void omp_set_lock(omp_lock_t *lock)`
- `void omp_unset_lock(omp_lock_t *lock)`
- `int omp_test_lock(omp_lock_t *lock)`
- Funcionan de igual manera que los locks de Pthreads.

#### Exclusión mutua recursiva

- El tipo de dato para los locks de esta clase es omp_nest_lock_t y las funciones disponibles son:
- `void omp_init_nest_lock(omp_nest_lock_t *lock)`
- `void omp_destroy_nest_lock(omp_nest_lock_t *lock)`
- `void omp_set_nest_lock(omp_nest_lock_t *lock)`
- `void omp_unset_nest_lock(omp_nest_lock_t *lock)`
- `int omp_test_nest_lock(omp_nest_lock_t *lock)`
- Funcionan de igual manera que los locks recursivos de Pthreads.

### Variables de entorno

- OpenMP cuenta con un conjunto de variables de entorno para ayudar a controlar la ejecución del programa paralelo.
- `OMP_NUM_THREADS`: Especifica la cantidad de hilos por defecto que se crearán.
- `OMP_DYNAMIC`: Determina si el número de hilos puede ser modificado en forma dinámica.
- `OMP_NESTED`: Especifica si se permite el paralelismo anidado.
- `OMP_SCHEDULE`: Planificación para cuando la cláusula schedule es runtime.

### Tasking

#### Concepto

- Una tarea es una unidad de trabajo (porción de código) cuya ejecución puede ser diferida en el tiempo. Se compone de:
  - Código a ejecutar.
  - Entorno de datos.
  - Variables de entorno internas.
- Está pensado para paralelizar problemas **irregulares** como:
  - Bucles while.
  - Bucles for que no tienen una cantidad conocida de iteraciones.
  - Algoritmos recursivos.

#### Constructor task

- Sintaxis:

```c
#pragma omp task [lista de cláusulas] {
    ...
}
```

- El programador identifica las tareas encerrando los bloques de código correspondientes bajo la directiva task: se asume que todas las tareas son independientes entre sí.
- Cuando un hilo encuentra un constructor task, el sistema de ejecución genera una nueva tarea.
- El momento en que esta tarea se ejecute dependerá del sistema de ejecución, el cual puede ser inmediato o diferido.
- Se permite el anidamiento de tareas: una tarea puede generar otras tareas.
- Cláusulas disponibles:
  - shared, private, firstprivate, default.
  - untied: por defecto la tarea es ejecutada de inicio a fin por un mismo hilo (no necesariamente el que la generó). untied permite que la tarea pueda ser completada por más de un hilo.
  - if (expresión): evalúa la expresión.
    - Si el resultado es verdadero, se genera una tarea.
    - Si el resultado es falso, se ejecuta el código inmediatamente.

#### Barreras

- Las barreras para tareas son específicas para un hilo de un equipo.
- Sintaxis:

```c
#pragma omp taskwait
```

- Al llegar a una directiva taskwait, el hilo se suspende hasta que todas sus tareas hijas se hayan completado (**sólo considera las hijas, no sus descendientes**).

#### Reglas de alcance

- Si la cláusula default no fue especificada, entonces:
  - Las variables no especificadas son firstprivate por defecto (esto es así para garantizar su posible ejecución diferida).
  - Las variables que fueron especificadas como shared en la directiva inmediatamente anterior, mantienen su condición.

---

<h1 align="center">Clase 7 - 25 de abril, 2025</h1>

## Programación en pasaje de mensajes

### Fundamentos

- Tenemos un conjunto de procesos, donde cada uno tiene su propio espacio de direcciones privado (partición).
- Cada dato pertenece a una partición.
- Toda interacción requiere la cooperación de dos procesos.
- Enviar mensajes sirve para intercambiar datos y/o para sincronizar procesos.
- Se suele usar en plataformas de memoria distribuida como clusters.
- Suele utilizar el modelo Single Program Multiple Data.

### Ventajas y desventajas

- El programador tiene control total para lograr sistemas más eficientes y escalables. ✅
- Puede implementarse eficientemente en muchas arquitecturas paralelas. ✅
- El rendimiento es más fácil de predecir. ✅
- Mayor complejidad al implementar estos algoritmos para lograr alto rendimiento. ❌

### Operaciones Send y Receive

- Sintaxis:

```c
send(void *sendbuf, int nelems, int dest)
receive(void *recvbuf, int nelems, int source)
```

- Ejemplo:

```c
// Proceso 0
a = 100;
send(&a, 1, 1);
a = 0;

// Proceso 1
receive(&b, 1, 0);
```

### Operaciones bloqueantes vs no bloqueantes

#### Bloqueante

- Ocurren dos cosas:
  - Se devuelve el control al proceso llamador una vez que todos los recursos involucrados (por ejemplo, el buffer de envío/recepción) pueden ser reutilizados. Aplica tanto al emisor como al receptor.
  - Se garantiza que todas las transiciones de estado iniciadas por la operación fueron completadas.
- Una operación bloqueante puede ser con o sin buffering.

##### Sin buffering

- El `send` se bloquea hasta que el receptor no termine el receive del mensaje.
- Produce tiempo ocioso en los procesadores.
- Puede haber deadlocks si las sentencias de comunicación no coinciden.

##### Con buffering

- El `send` se bloquea hasta que el mensaje llega a un buffer (intermedio y diferente al del receptor) prealocado del sistema.
- Transmisión del mensaje:
  - Si se tiene hardware específico para comunicación asincrónica no hace falta que intervenga la CPU, el proceso emisor copia el mensaje a un buffer local y luego este hardware específico transmite ese mensaje al buffer del receptor.
  - Si no se tiene hardware especial el emisor transmite el mensaje al buffer del receptor y recién ahí puede continuar.
- Reducen el tiempo ocioso de los procesadores pero aumentan el costo por manejo de buffers.
- Los buffers intermedios tienen capacidad limitada, por ende si se llega al límite, el emisor quedará bloqueado.
- Reduce la ocurrencia de deadlocks, aunque no los evita del todo.

#### No bloqueante

- Prioriza el rendimiento.
- Devuelven el control de la operación de forma inmediata.
- No garantiza que los recursos involucrados puedan ser reutilizados de forma inmediata.
- No garantiza que todas las transiciones de estado iniciadas por la operación hayan sido completadas.
- Requiere chequear que una comunicación ha terminado: deja en manos del programador asegurar la semántica del send.
- Una operación no bloqueante puede ser con o sin buffering.

#### Sin buffering

- La comunicación comienza al llegar al receive.

#### Con buffering

- El emisor usa DMA para copiar los datos a un buffer prealocado mientras el proceso continúa ejecutándose.
- Reduce el tiempo en el que el dato no está seguro.

#### Resumen

| Buffering | Operación bloqueante                                                           | Operación no bloqueante                                                                                                      |
| --------- | ------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------- |
| Si        | El emisor retoma el control una vez que los datos han sido copiados al buffer. | El emisor retoma el control una vez que ha iniciado la transferencia DMA al buffer, aun cuando podría no haberse completado. |
| No        | El emisor se bloquea hasta que el receptor alcance el receive correspondiente. | El emisor envía los datos cuando el receptor alcanza el receive.                                                             |

- Semántica de las operaciones:
  - Asegurada en bloqueante.
  - Responsabilidad del programador de asegurarla en no bloqueante.

## MPI

### Estándar

- A principios de los 90 existían numerosas librerías para pasaje de mensaje (no compatibles). Un grupo de representantes de las universidades y de la industria se reunieron para crear un estándar para programación basado en pasaje de mensajes.
- MPI define una **librería estándar** que puede ser empleada desde C o Fortran (y potencialmente desde otros lenguajes).
- Existen diferentes implementaciones de MPI en la actualidad (OpenMPI, MPICH, Intel MPI, IBM MPI, entre otras), aunque no todas soportan la especificación en forma completa → Aspecto a considerar a la hora de elegir la implementación MPI.
- Usa el modelo Single Program Multiple Data (SPMD).
- Define la sintaxis y la semántica de más de 400 rutinas.
- Se importa usando `# include <mpi.h>`.
- Se compila usando `mpicc codigo.c -o ejecutable`.
- Se ejecuta usando `mpirun -np <cantidad-de-procesos> ejecutable`.

### MPI_Init y MPI_Finalize

- `MPI_Init`:

  - Inicializa el entorno MPI.
  - Debe ser invocada por todos los procesos antes que cualquier otro llamado a rutinas MPI.
  - Sintaxis:

  ```c
  MPI_Init (int *argc, char **argv);
  ```

- `MPI_Finalize`:

  - Cierra el entorno MPI.
  - Debe ser invocado por todos los procesos como último llamado a rutinas MPI.
  - Sintaxis:

  ```c
  MPI_Finalize();
  ```

### Comunicadores

- Indican qué procesos pueden comunicarse entre sí.
- Son variables de tipo MPI_Comm y almacenan información sobre los procesos que pertenecen a él.
- Un proceso puede pertenecer a varios comunicadores.
- Existe un comunicador default al cual todos los procesos de la aplicación pertenecen: MPI_COMM_WORLD.
- Cada operación de comunicación debe indicar el comunicador que se quiere usar.

#### Adquisición de información

- `MPI_Comm_size`:

  - Indica la cantidad de procesos en el comunicador.
  - Sintaxis:

  ```c
  MPI_Comm_size(MPI_Comm comunicador, int *cantidad);
  ```

- `MPI_Comm_rank`:

  - Indica el rank (id) del proceso dentro de ese comunicador.
  - Es un valor entre 0 y cantidad.
  - Cada proceso puede tener un rank distinto en cada comunicador.
  - Sintaxis:

  ```c
  MPI_Comm_rank(MPI_Comm comunicador, int *rank);
  ```

### Tipos de datos

| C                  | MPI                |
| ------------------ | ------------------ |
| signed char        | MPI_CHAR           |
| signed short int   | MPI_SHORT          |
| signed int         | MPI_INT            |
| signed long int    | MPI_LONG           |
| unsigned char      | MPI_UNSIGNED_CHAR  |
| unsigned short int | MPI_UNSIGNED_SHORT |
| unsigned int       | MPI_UNSIGNED       |
| unsigned long int  | MPI_UNSIGNED_LONG  |
| float              | MPI_FLOAT          |
| double             | MPI_DOUBLE         |
| long double        | MPI_LONG_DOUBLE    |
|                    | MPI_BYTE           |
|                    | MPI_PACKED         |

### Tipos de comunicaciones

#### Punto a punto

- Involucran a dos procesos.
- Pueden ser bloqueantes o no bloqueantes.

##### Bloqueante

###### Send

- MPI provee 4 opciones para realizar un send bloqueante.
- `MPI_Send`:

  - Rutina básica para enviar datos a otro proceso.
  - Retorna el control solo cuando el buffer del emisor está listo para ser reusado, lo cual no significa necesariamente que el receptor ya lo haya recibido.
  - Puede usar buffering o no, depende de la implementación.
  - Sintaxis:

  ```c
  MPI_Send(void *buf, int cantidad, MPI_Datatype tipoDato, int destino, int tag, MPI_Comm comunicador);
  ```

- `MPI_Bsend`:

  - Buffered send.
  - Permite implementar buffering a nivel de usuario, ya sea para mayor customización o porque el sistema no implementa buffers.
  - Sintaxis:

  ```c
  MPI_Bsend(void *buf, int cantidad, MPI_Datatype tipoDato, int destino, int tag, MPI_Comm comunicador);
  ```

- `MPI_Ssend`:

  - Synchronous send.
  - Retorna el control solo cuando el buffer del emisor está listo para ser reusado y el proceso receptor ha comenzado a recibir el mensaje.
  - Sintaxis:

  ```c
  MPI_Ssend(void *buf, int cantidad, MPI_Datatype tipoDato, int destino, int tag, MPI_Comm comunicador);
  ```

- `MPI_Rsend`:

  - Ready send.
  - Solo puede ser invocado si el proceso receptor ya se encuentra listo para recibir, es decir, se encuentra posicionada en el receive correspondiente.
  - Si se invoca a esta operación pero el receptor no está esperando en el receive, la operación será errónea y el resultado no garantizado.
  - No se suele usar.
  - Sintaxis:

  ```c
  MPI_Rsend(void *buf, int cantidad, MPI_Datatype tipoDato, int destino, int tag, MPI_Comm comunicador);
  ```

###### Receive

- `MPI_Recv`:

  - Rutina básica para recibir datos de otro proceso.
  - Puede usar dos comodines:
    - MPI_ANY_SOURCE como reemplazo del argumento origen. Permite recibir de cualquier proceso.
    - MPI_ANY_TAG como reemplazo del argumento tag. Permite que la correspondencia solo sea obligatoria para el argumento origen pero no necesariamente para el tag también.
  - Sintaxis:

  ```c
  MPI_Recv(void *buf, int cantidad, MPI_Datatype tipoDato, int origen, int tag, MPI_Comm comunicador, MPI_Status *estado);
  ```

- `MPI_Get_count`:

  - Obtiene la cantidad de elementos recibidos.
  - Sintaxis:

  ```c
  MPI_Get_count(MPI_Status *estado, MPI_Datatype tipoDato, int *cantidad);
  ```

##### No bloqueante

- Comienzan la operación de comunicación e inmediatamente devuelven el control, sin garantizar que la operación haya terminado.
- Permiten solapar cómputo con comunicación.
- Suelen mejorar la performance del programa.
- Es responsabilidad del programador asegurar la semántica de las operaciones.

###### Send

- `MPI_Isend`:
  - Realiza un envío en forma no bloqueante.
  - Sintaxis:
  ```c
  MPI_Isend(void *buf, int cantidad, MPI_Datatype tipoDato, int destino, int tag, MPI_Comm comunicador, MPI_Request *solicitud);
  ```

###### Receive

- `MPI_Irecv`:
  - Realiza una recepción en forma no bloqueante.
  - Sintaxis:
  ```c
  MPI_Irecv(void *buf, int cantidad, MPI_Datatype tipoDato, int origen, int tag, MPI_Comm comunicador, MPI_Request *solicitud);
  ```

###### Misceláneas

- `MPI_Test`:

  - Evalúa si el send o receive finalizó.
  - Sintaxis:

  ```c
  MPI_Test(MPI_REQUEST *solicitud, int *flag, MPI_STATUS *estado);
  ```

- `MPI_Wait`:
  - Bloquea al proceso hasta que la operación indicada en el request haya terminado.
  - Evita busy-waiting.
  - Sintaxis:
  ```c
  MPI_Wait(MPI_REQUEST *solicitud, MPI_STATUS *estado);
  ```

##### Orden y fairness

- En las comunicaciones punto a punto, se deben tener varias cosas en cuenta:
- Respecto al orden:
  - MPI asegura que los mensajes no se sobrepasan entre ellos.
  - Si un proceso envía dos mensajes M1 y M2 seguidos a un receptor, y el receptor ejecuta un receive, el orden de recepción será M1 M2.
  - Si un proceso ejecuta dos receive seguidos y hay un mensaje pendiente que coincide con ambos, el primer receive recibirá antes que el segundo.
- Respecto al fairness:
  - MPI no lo asegura.
  - Es responsabilidad del programador evitar la inanición de procesos.
  - Ejemplo:
    - P0 le envía un mensaje a P2.
    - P1 envía otro mensaje a P2 que compite con el que envió P0.
    - P2 solo recibe uno de los dos mensajes.

#### Colectivas

- Involucran a dos o más procesos.
- Pueden ser bloqueantes o no bloqueantes.
- Facilitan la programación.
- Mejoran el rendimiento ya que están muy optimizadas.
- Todos los procesos del comunicador deben llamar a la rutina colectiva.
- Hay 3 tipos de operaciones colectivas:
  - **Sincronización** (barreras).
  - **Transferencia de datos** (broadcast, gather, etc).
  - **Computaciones colectivas** (reducción).

##### Sincronización por barrera

- El proceso que llama a esta función queda bloqueado hasta que todos los procesos del comunicador pasado como argumento hayan llegado también a la barrera.
- Sintaxis:

```c
MPI_Barrier(MPI_Comm comunicador);
```

##### Broadcast

- Un proceso envía el mismo mensaje a todos los procesos del comunicador pasado como argumento.
- Sintaxis:

```c
MPI_Bcast(void *buf, int cantidad, MPI_Datatype tipoDato, int origen, MPI_Comm comunicador);
```

##### Gather

- Permite que todos los procesos excepto uno de un comunicador envíen un dato a un proceso en particular de ese comunicador.
- Requiere que todos los procesos aporten la misma cantidad de datos.
- Este proceso elegido denominado destino recibirá cada dato y los concatenará en **orden del id que tiene cada proceso en el comunicador**.
- Sintaxis:

```c
MPI_Gather(void *sendbuf, int cantEnvio, MPI_Datatype tipoDatoEnvio, void *recvbuf, int cantRec, MPI_Datatype tipoDatoRec, int destino, MPI_Comm comunicador);
```

##### Gatherv

- Igual que gather solo que cada proceso puede enviar una cantidad variable de datos.
- Sintaxis:

```c
MPI_Gatherv(void *sendbuf, int cantEnvio, MPI_Datatype tipoDatoEnvio, void *recvbuf, int *cantsRec, int *desplazamientos, MPI_Datatype tipoDatoRec, int destino, MPI_Comm comunicador);
```

##### Allgather

- Igual que gather solo que el resultado concatenado es enviado a todos los procesos y no solo a uno.
- Sintaxis:

```c
MPI_AllGather(void *sendbuf, int cantEnvio, MPI_Datatype tipoDatoEnvio, void *recvbuf, int cantRec, MPI_Datatype tipoDatoRec, MPI_Comm comunicador);
```

##### Allgatherv

- Igual que gatherv solo que el resultado concatenado es enviado a todos los procesos y no solo a uno.
- Sintaxis:

```c
MPI_AllGatherv(void *sendbuf, int cantEnvio, MPI_Datatype tipoDatoEnvio, void *recvbuf, int *cantsRec, int *desplazamientos, MPI_Datatype tipoDatoRec, MPI_Comm comunicador);
```

##### Scatter

- Reparte un vector de datos entre todos los procesos (incluyendo a sí mismo) de forma **equitativa**.
- Sintaxis:

```c
MPI_Scatter(void *sendbuf, int cantEnvio, MPI_Datatype tipoDatoEnvio, void *recvbuf, int cantRec, MPI_Datatype tipoDatoRec, int origen, MPI_Comm comunicador);
```

##### Scatterv

- Igual que scatter solo que la repartición es variable y no equitativa.
- Sintaxis:

```c
MPI_Scatterv(void *sendbuf, int *cantsEnvio, int *desplazamientos, MPI_Datatype tipoDatoEnvio, void *recvbuf, int cantRec, MPI_Datatype tipoDatoRec, int origen, MPI_Comm comunicador);
```

##### All to all

- Cada proceso del comunicador reparte un conjunto de datos entre todos los demás procesos (incluyendo a sí mismo) y luego recibe un bloque de datos de cada uno de ellos.
- Todas las porciones son de mismo tamaño.
- Es equivalente a scatter + gather.
- Sintaxis:

```c
MPI_Alltoall(void *sendbuf, int cantEnvio, MPI_Datatype tipoDatoEnvio, void *recvbuf, int cantRec, MPI_Datatype tipoDatoRec, MPI_Comm comunicador);
```

##### All to all V

- Igual a All to all solo que las porciones son de tamaño variable.
- Sintaxis:

```c
MPI_Alltoallv(void *sendbuf, int *cantsEnvio, int *despEnvio, MPI_Datatype tipoDatoEnvio, void *recvbuf, int *cantsRec, int *despRec, MPI_Datatype tipoDatoRec, MPI_Comm comunicador);
```

##### Reducción todos a uno

- Permite que todo proceso que pertenece a un comunicador envíe datos a uno de los procesos, el cual recibe los datos y los combina aplicando una operación.
- Sintaxis:

```c
MPI_Reduce(void *sendbuf, void *recvbuf, int cantidad, MPI_Datatype tipoDato, MPI_Op operación, int destino, MPI_Comm comunicador);
```

##### Reducción todos a todos

- Igual al anterior solo que el resultado es envíado a todos y no solo uno.
- Sintaxis:

```c
MPI_Allreduce(void *sendbuf, void *recvbuf, int cantidad, MPI_Datatype tipoDato, MPI_Op operación, MPI_Comm comunicador);
```

### Consulta de comunicaciones pendientes

- Es posible consultar si hay comunicaciones pendiente y algunos de sus datos.
- `MPI_Probe`:

  - Bloqueante.
  - Bloquea al proceso hasta que llegue un mensaje que cumpla con el origen y tag.
  - Sintaxis:

  ```c
  MPI_Probe(int origen, int tag, MPI_Comm comunicador, MPI_Status *estado);
  ```

- `MPI_Iprobe`:

  - No bloqueante.
  - Chequea por el arribo de un mensaje que cumpla con el origen y tag.
  - Puede usar comodines en origen y tag.
  - Sintaxis:

  ```c
  MPI_Iprobe(int origen, int tag, MPI_Comm comunicador, int *flag, MPI_Status *estado);
  ```

### Grupos y comunicadores

- Cuando las comunicaciones entre los procesos de un programa se realizan entre un subconjunto de ellos y no todos, MPI provee mecanismos para dividir el grupo de procesos de un comunicador en subgrupos, cada uno con su correspondiente comunicador.
- Sintaxis:

```c
MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm);
```

### Evolución de MPI

- **MPI-1**: 1994.
- **MPI-2**: 1998.
  - Generación dinámica de procesos (durante la ejecución).
  - Comunicaciones one-sided.
  - Soporte para comunicaciones colectivas de a grupos.
  - Soporte para C++.
  - Entrada/Salida paralela.
- **MPI-3**: 2012.
  - Comunicaciones colectivas no bloqueantes.
  - Más soporte para comunicaciones one-sided.
  - Más soporte para comunicaciones colectivas de a grupos.
  - Soporte para Fortran 2008.
- **Hoy**:
  - La mayoría de las implementaciones en distintos lenguajes de programación siguen la versión 1.2 del estándar.
  - Ejemplos de implementaciones:
    - Python: mpi4py, ScientificPython.
    - Java: Java-MPI.
    - R: Rmpi.
    - Rust: rsmpi.
    - Julia: MPI.jl.

---

<h1 align="center">Clase 8 - 9 de mayo, 2025</h1>

## Programación en modelo híbrido

### Fundamentos del modelo

- Procesadores multicore + clusters tradicionales = clusters de multicores.
- Estos "nuevos" clusters son arquitecturas híbridas y jerárquicas de dos niveles.
- Estas arquitecturas requieren modelos de comunicación híbridos, es decir comunicarse tanto vía pasaje de mensajes como vía memoria compartida, ya que usar solo uno de ellos no se adaptaría bien.
- Idea básica:
  - Tareas en un mismo nodo se comunican vía memoria compartida.
  - Tareas en nodos distintos se comunican vía pasaje de mensajes.
  - Combinar Pthreads/OpenMP con MPI.

### Razones para usar el modelo

1. **Al usar MC dentro de cada nodo**:
   1. Se reduce el overhead de comunicaciones MPI.
   2. Se reducen los requerimientos de memoria de la aplicación.
2. **Algunas aplicaciones presentan dos niveles de paralelismo**:
   1. De grano grueso → Se puede explotar con MPI.
   2. De grano fino → Se puede explotar con OpenMP.
   3. El modelo híbrido explota ambos tipos de granularidad.
3. **Carga de trabajo desbalanceada a nivel de MPI**:
   1. Balancear la carga en forma dinámica con OpenMP es más sencillo.
4. Pthreads también es una opción aunque con costo de programación mayor.

### Razones para no usar el modelo

1. Algunas aplicaciones presentan un solo nivel de paralelismo, por ende el paralelismo jerárquico no provee beneficios y hace el código más complejo.
2. Al introducir OpenMP o Pthreads a un código MPI existente también se están introduciendo sus desventajas:
   1. Overhead adicional por la creación, sincronización y destrucción de hilos.
   2. Dependencia en la calidad del compilador y del soporte en ejecución para OpenMP/Pthreads.
   3. Cuestiones relacionadas al uso de MC, como ubicación de los datos en memoria y conflictos en el acceso a los mismos.

### Esquemas

- Existen diferentes esquemas para paralelizar una aplicación usando el modelo híbrido.
- La clasificación se hace teniendo en cuenta:
  - Qué hilo/s envía/n mensajes entre los procesos MPI.
  - En qué momento lo hace/n.

#### Sin solapamiento de cómputo y comunicaciones

- También conocido como master-only o modo vector.
- Usa un proceso MPI por nodo y OpenMP o Pthreads en cada núcleo de cada nodo.
- Las llamadas a las rutinas MPI son realizadas **fuera de las regiones paralelas de OpenMP o del código de los hilos creados con Pthreads**, por lo que el código está muy bien delimitado y separado.
- **Ventajas**:
  - Menos complejo.
  - No hay intercambio de mensajes dentro de cada nodo.
  - La topología de los procesos MPI ya no es una cuestión relevante a la hora de optimizar el rendimiento de la aplicación.
- **Desventajas**:
  - Mientras el hilo master se comunica, el resto de los hilos está ocioso → overhead.
  - Un único hilo probablemente no sea capaz de aprovechar todo el ancho de banda disponible de la red de comunicación.

#### Con solapamiento de cómputo y comunicaciones

- Consiste en permitir que **más de un hilo pueda comunicarse (MPI) en paralelo** a otros que realicen cómputo útil.
- **Ventajas**:
  - Se reduce el tiempo ocioso que los hilos podrían incurrir.
  - Se aprovecha el ancho de banda de la red.
- **Desventajas**:
  - Más complejo.
  - Se debe equilibrar la carga de trabajo entre los hilos que comunican y los que no.

### Soporte MPI para programación híbrida

- Las librerías MPI varían en su soporte para las comunicaciones entre hilos.
- MPI especifica 4 niveles diferentes:
  - `MPI_THREAD_SINGLE` (Nivel 0): No soporta hilos.
  - `MPI_THREAD_FUNNELED` (Nivel 1): Los procesos pueden ser multi- hilados **pero todas las comunicaciones las realizará el hilo master**.
  - `MPI_THREAD_SERIALIZED` (Nivel 2): Los procesos pueden ser multi-hilados y los diferentes hilos pueden ejecutar rutinas MPI pero sólo una a la vez; **los llamados a MPI no pueden ser realizados en simultáneo por 2 hilos**.
  - `MPI_THREAD_MULTIPLE` (Nivel 3): Múltiples hilos pueden realizar múltiples comunicaciones a la vez, sin restricciones.
- Si nuestro programa es híbrido, se debe reemplazar `MPI_Init` por `MPI_Init_thread` para procesos multi-hilados:
  - `MPI_Init_thread(int *argc, char ***argv, int required, int *provided)`

### Ejemplo: Reducción a suma en cluster de multicores

#### Explicación del problema

- Debemos desarrollar un algoritmo paralelo para computar la reducción a suma de un vector.
- La arquitectura de soporte es un cluster de 2 nodos donde cada nodo tiene 2 procesadores quad-core.
  - Por ende tenemos 8 núcleos por nodo.
- Tenemos dos opciones para resolverlo de forma paralela:
  - Un algoritmo que use solo MPI.
  - Un algoritmo híbrido, que use MPI + Pthreads o MPI + OpenMP.

#### Usando solo MPI

```c
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_SIZE 2000
#define COORDINATOR 0

int main(int argc, char* argv[])
{
    int i, num_procs, rank, size, strip_size, local_sum = 0, sum = 0;
    int array[MAX_SIZE];
    MPI_Status status;

    size = atoi(argv[1]);
    size = (size < MAX_SIZE ? size : MAX_SIZE);

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == COORDINATOR) {
        for (i = 0; i < size; i++) {
            array[i] = i + 1;
        }
    }

    strip_size = size / num_procs;

    MPI_Scatter(array, strip_size, MPI_INT, array, strip_size, MPI_INT, COORDINATOR, MPI_COMM_WORLD);

    for (i = 0; i < strip_size; i++) {
        local_sum += array[i];
    }

    MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, COORDINATOR, MPI_COMM_WORLD);

    MPI_Finalize();

    if (rank == COORDINATOR) {
        printf("Sum=%d\n", sum);
    }
    return 0;
}
```

#### Usando híbrido (master-only)

```c
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define MAX_SIZE 2000
#define MAX_THREADS 100
#define COORDINATOR 0

int main(int argc, char* argv[])
{
    int i, num_procs, rank, size, strip_size, local_sum = 0, sum = 0, threads, provided;
    int array[MAX_SIZE];

    size = atoi(argv[1]);
    size = (size < MAX_SIZE ? size : MAX_SIZE);

    threads = atoi(argv[2]);
    threads = (threads < MAX_THREADS ? threads : MAX_THREADS);

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == COORDINATOR) {
        for (i = 0; i < size; i++) {
            array[i] = i + 1;
        }
    }

    strip_size = size / num_procs;

    MPI_Scatter(array, strip_size, MPI_INT, array, strip_size, MPI_INT, COORDINATOR, MPI_COMM_WORLD);

    #pragma omp parallel for num_threads(threads) reduction(+:local_sum) schedule(static)
    for (i = 0; i < strip_size; i++) {
        local_sum += array[i];
    }

    MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, COORDINATOR, MPI_COMM_WORLD);

    MPI_Finalize();

    if (rank == COORDINATOR) {
        printf("Sum=%d\n", sum);
    }
    return 0;
}
```

#### Comparación

- En la solución con MPI:
  - Se generan 16 procesos, uno por cada núcleo de la arquitectura (tenemos 4 núcleos × 2 procesadores × 2 nodos).
  - Como los datos a comunicar tienen el mismo tamaño, esto implica más mensajes comunicados pero más cortos.
- En la solución híbrida:
  - Se generan 2 procesos, uno por cada nodo, y 8 hilos por nodo, con un total de 16 hilos.
  - Tenemos menos mensajes pero más grandes.
  - Tiende a tener menor overhead por menor latencia (menos mensajes, menos uso de la red de interconexión que suele ser un cuello de botella).

---

<h1 align="center">Clase 9 - 23 de mayo, 2025</h1>

## High Performance Computing

### TOP500

- Ranking de las 500 supercomputadoras más potentes.
- Comenzó en 1993 y sigue vigente hasta hoy.
- Se actualiza dos veces por año, en junio y en noviembre.
- Para el cálculo de la potencia se usa un benchmark llamado LINPACK.

### Impacto del consumo energético

- Objetivo HPC = Incrementar el rendimiento y ocasionalmente con consideración precio/rendimiento.
- El incremento enorme de performance de las supercomputadoras trajo como consecuencia un notable incremento en el consumo de energía eléctrica por parte de éstas.
- El Capitán, la supercomputadora TOP 1, usa 29.5 MW.
- Como comparación, un hogar demanda 0.25 kW.
- Es decir que esta supercomputadora usa más energía que toda una ciudad entera.

### Impacto social y medioambiental

- El enorme consumo de energía de las supercomputadoras tiene un impacto social y medioambiental debido a que esa energía debe salir de algun lado.
- Esta generación masiva de energía contamina el medioambiente y genera muertes.
- Representa uno de los mayores obstaculos para continuar optimizando a las supercomputadoras.

### GREEN500

- Ranking de las 500 supercomputadoras más eficientes del mundo desde el punto de vista energético.
- Comenzó en 2006 y sigue vigente hasta hoy.
- Se actualiza dos veces por año, en junio y en noviembre.
- Para el cálculo de la potencia se usa un benchmark llamado LINPACK.
- Además, se mide el consumo energético durante su ejecución.
- Se calcula el cociente potencia en FLOPS dividido uso de energía, y esto da como resultado la eficiencia energética de la supercomputadora.

### Aceleradores

- Un acelerador es un dispositivo de hardware diseñado para mejorar el rendimiento del sistema.
- Su uso en las supercomputadoras fue creciendo exponencialmente.
- Esto se debe a que las placas de video, que usan aceleradores, son mucho más eficientes energéticamente que las CPUs.

### Supercomputadoras en Argentina



## GPUs

### Concepto

- Originalmente diseñadas para procesamiento de gráficos.
- Debido a su gran potencia de cálculo, las empresas comenzaron a aumentar su grado de programación.
- Motivó el surg... 

### Diferencias vs CPUs

- Las GPUs son mucho más rápidas que las CPUs debido a las diferencias de diseño.
- ...
- GPUs muchos más cores, pero más sencillos.
- SIMD en las GPUs, MIMD en las CPUs.

...

### Jerarquía de memoria

...

### Arquitectura

Memoria compartida...

...

### Tecnologías de programación

CUDA funciona solo para GPUs NVIDIA


OpenCL funciona en cualquier dispositivo, GPU CPU NVIDIA AMD INTEL FPGA etc ... No se usa mucho porque es demasiado complejo

SYCL como OpenCL pero menos complejo

...

### Evolución de arquitecturas de NVIDIA

...

### Evolución de arquitecturas de AMD

...

## Intel Xeon Phi, GPUs y procesadores híbridos de Intel

### Intel Xeon Phi (primera generación)

### Intel Xeon Phi - Knights Corner (segunda generación)

### Intel Xeon Phi - Knights Mill (tercera generación)

### Intel Xe

### Procesadores híbridos de Intel 



## FPGAs y ASICs

Estos dos temas NO ENTRAN en el teórico.

###

###
