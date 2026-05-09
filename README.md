# cache_oblivious_static_tree

## Integrantes
- Marcelo Chincha
- Adrian Cespedes

## Objetivo
Comparar la localidad de cache entre un BST en layout plano (array tipo heap) y un BST en layout van Emde Boas (vEB) para consultas de busqueda. El benchmark mide tiempo por consulta y reporta el speedup de vEB sobre el BST plano.


## Parametros del experimento
- B (block size target): **B = 512 Bytes**.
- Numero de elementos (N): [1M, 2M, 4M, 8M, 16M, 32M, 64M, 255M, 512M, 1024M].
- Numero de consultas (Q): 1M queries por N.
- Trials (T): 3.


## Hardware usado
Completar aqui:
- CPU: Ryzen 7 5700X3D (8 cores, 16 threads, 3.4GHz base, 4.6GHz boost)
- Memoria RAM: 32GB DDR4-3200, DUAL CHANNEL (2x16GB) 
- Cache (L1/L2/L3): 512KB/4MB/96MB
- Sistema operativo: Windows 10 Pro 64-bit

## Como correr
Compilar y ejecutar:
```bash
make
make run
```

El programa genera:
- Tabla en consola con `N`, `Data MB`, `Flat ns/query`, `vEB ns/query`, `Speedup`.
- Archivo `benchmark_results.csv` con los mismos datos.

## Diseno del benchmark
- Los arboles se construyen con **el mismo vector ordenado** (sin sorting interno).
- Las consultas son aleatorias con 50% hits y 50% misses.
- En cada trial, ambos arboles usan **el mismo conjunto y orden** de queries.

## Resultados

| N | Data MB | Flat ns/query | vEB ns/query | Speedup |
|---:|---:|---:|---:|---:|
| 1000000 | 3.81 | 55.95 | 39.84 | 1.40 |
| 2000000 | 7.63 | 61.04 | 40.22 | 1.52 |
| 4000000 | 15.26 | 65.91 | 40.07 | 1.64 |
| 8000000 | 30.52 | 78.21 | 40.51 | 1.93 |
| 16000000 | 61.04 | 92.23 | 40.78 | 2.26 |
| 32000000 | 122.07 | 122.76 | 42.03 | 2.92 |
| 64000000 | 244.14 | 129.66 | 39.66 | 3.27 |
| 255000000 | 972.75 | 153.35 | 39.63 | 3.87 |
| 512000000 | 1953.12 | 159.99 | 41.23 | 3.88 |
| 1024000000 | 3906.25 | 206.73 | 41.92 | 4.93 |

Tambien se guarda el CSV para graficar o analizar con scripts.