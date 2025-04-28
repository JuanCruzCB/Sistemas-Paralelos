#!/bin/bash

EJECUTABLE_PARALELO="./paralelo"
EJECUTABLE_SECUENCIAL="./secuencial"
RESULTADO="resultados_tiempos.txt"

VALORES_N=(100 1000 5000 10000)
HILOS=(2 4 8)

# Función para extraer el tiempo de la salida del programa
extraer_tiempo() {
    local output="$1"
    echo "$output" | grep -oP "Tiempo de ejecución: \K[0-9]+\.[0-9]{6}" | head -1
}

# Encabezado de la tabla
printf "%-8s | %-10s | %-9s | %-9s | %-9s |\n" "N" "Secuencial" "2 hilos" "4 hilos" "8 hilos" >> $RESULTADO
printf "%-8s-|-%-10s-|-%-9s-|-%-9s-|-%-9s-|\n" "--------" "----------" "---------" "---------" "---------" >> $RESULTADO

# Array para almacenar todos los resultados
declare -A resultados

# Ejecución secuencial
for N in "${VALORES_N[@]}"; do
    echo "Ejecutando secuencial con N = $N..."
    output=$($EJECUTABLE_SECUENCIAL $N 2>&1)
    tiempo=$(extraer_tiempo "$output")
    resultados["$N,secuencial"]=$tiempo
done

# Ejecución paralela
for N in "${VALORES_N[@]}"; do
    for hilos in "${HILOS[@]}"; do
        echo "Ejecutando paralelo con N=$N y $hilos hilos..."
        output=$($EJECUTABLE_PARALELO $N $hilos 2>&1)
        tiempo=$(extraer_tiempo "$output")
        resultados["$N,$hilos"]=$tiempo
    done
done

# Imprimir resultados en formato de tabla
for N in "${VALORES_N[@]}"; do
    printf "%-8s | %-10s | %-9s | %-9s | %-9s |\n" \
        "$N" \
        "${resultados["$N,secuencial"]}" \
        "${resultados["$N,2"]}" \
        "${resultados["$N,4"]}" \
        "${resultados["$N,8"]}" >> $RESULTADO
done

echo "" >> $RESULTADO
echo "Tabla de resultados generada en $RESULTADO"
