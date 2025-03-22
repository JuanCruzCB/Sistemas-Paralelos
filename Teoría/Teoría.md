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

### Arreglos

- Los arreglos multidimensionales son la estructura de datos más usada en High Performance Computing. Las demás estructuras de datos (árboles, listas, grafos, etc) no se usan seguido ya que son menos eficientes en el contexto del paralelismo comparados a los arreglos.
- Los datos de un arreglo multidimensional pueden ser almacenados en memoria por filas o por columnas.

## Coherencia de caché

## Costos de comunicación

---

<h1 align="center">Clase 3 - 28 de marzo, 2025</h1>
