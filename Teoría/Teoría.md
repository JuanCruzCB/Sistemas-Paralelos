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

###

- Multiples unidades de procesamiento.
- Considerar concurrencia y especificar qué pasos se pueden hacer en simultaneo.
- Aisngar tareas a procesos que se ejecutan en dif procesadores.
- Administrar accesos a data compartida.
- Sincronizar.
- Distribuir datos de entrada salida e intermedios.

## Etapa de descomposición en tareas

###

- PRoceso de dividir el computo en tareas las cuales algunas o todas se ejecutaran en paralelo.
- Gran cantidad de tareas peqeuñas inicialmente.
- Paralelismo de datos vs paralelismo funcional.
- Si el problema lo permite, todas las tareas son independientes.
- Para problemas complejos, un grafo de dependencia de tareas es util.
- Grano fino vs Grano grueso. Cual es mejor depende de cuantos cores tiene la maquina.
- Grado de concurrencia -> Maximo grado de concurrencia -> Grado de concurrencia promedio.
- Camino crítico de grafo indica el grado de concurrencia. UN camino critico mas corto favorece a un mayor grado de concurrencia. Puede ahber mas de un camino critico.
- Se busca un grado de concurrencia promedio mayor.
- Aglomeración de tareas: combinar varias tareas en una.
- Tipos de tecnicas de descomposicion: recursiva; basada en los datos; exploratoria; especulativa; híbrida. Nos interesan las dos primeras.

## Etapa de mapeo de tareas a procesos

###

- Las tareas se pueden generar de forma estática o dinámica.
- Estática -> la cantidaad se conoce pre-ejecucion
- Dinámica -> No se sabe de antemano, se resuelve en ejecucion.
- Las tareas pueden ser uniformes o no uniformes.

## Métodos para reducir overhead de las interacciones

###



## Modelos de algoritmos paralelos

###

---

<h1 align="center">Clase 5 - 11 de abril, 2025</h1>

## ?

---
