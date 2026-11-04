# Laboratorio: Programación con OpenMP — Lista Enlazada y Odd-Even Transposition Sort

Instrucciones de compilación y ejecución

## 1. Compilación de programas C++
Compilar los programas secuenciales y multithreading utilizando g++ con optimización -O2 y la bandera -fopenmp:

```bash
# lista enlazada con bloqueo global
g++ -O2 -fopenmp listaenlazada.cpp -o listaenlazada

# lista enlazada con bloqueo de grano fino
g++ -O2 -fopenmp listaenlazada2.cpp -o listaenlazada2

# odd-even transposition sort secuencial y paralelo
g++ -O2 -fopenmp oddeven.cpp -o oddeven
```

## 2. Ejecución de los benchmarks
Ejecutar los binarios para recolectar las métricas de rendimiento en archivos CSV:

```bash
# ejecucion de pruebas para lista enlazada
./listaenlazada && ./listaenlazada2

# ejecucion de pruebas para odd-even sort
./oddeven
```

## 3. Generación de gráficas
Ejecutar el script de Python para procesar los datos de los CSV y generar las figuras de rendimiento:

```bash
# generacion de graficas para lista enlazada y odd-even sort
python graficar.py
```
