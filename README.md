# Laboratorio 2 - Transformación de lazos, optimización del compilador y jerarquía de memoria
### EL4314 - Arquitectura de Computadoras I
### Escuela de Ingeniería Electrónica
### Tecnológico de Costa Rica

<br/><br/>

## Preámbulo
En este segundo laboratorio, usted experimentará el efecto que tienen la transformación de lazos, las optimizaciones del compilador y la jerarquía de memoria en el rendimiento de un procesador. Para la experimentación se usará un conjunto de aplicaciones de prueba, que incluye un arreglo de dos dimensiones, y otras cinco aplicaciones: _matrix multiplication_, _Fast Fourier Transform_ (FFT), _Adaptive Differential Pulse Code Modulation_ (ADPCM), _Advanced Encryption Standard_ (AES) y JPEG, las cuales provienen del _benchmark CHStone_ [1].

Para esta experimentación se utilizará el simulador (algo antiguo) llamado *SimpleScalar*, el cual es "una plataforma modular para la investigación de la arquitectura de sistemas computacionales". *SimpleScalar* permite realizar exploraciones del espacio de diseño de procesadores superscalares y entre los parámetros que se pueden explorar están la configuración de la memoria caché y del predictor de saltos. *Wattch* es una plataforma basada en *SimpleScalar* que permite la exploración y análisis de consumo de energía para un software en particular. Dicho simulador se encuentra disponible en una máquina virtual, la cual podrá descargar de [aquí](https://tecnube1-my.sharepoint.com/:f:/g/personal/jocastro_itcr_ac_cr/Et2j-bTKL4JFjir2rtHyKZwBde69Je0A8aKH2xubgCNBcw?e=95nDK0), y que ha sido preparada para este laboratorio 2 (usuario: _arquitectura_, contraseña: _ARQUI_)

[1] Yuko Hara, Hiroyuki Tomiyama, Shinya Honda, Hiroaki Takada and Katsuya Ishii, "CHStone: A benchmark program suite for practical C-based high-level synthesis," 2008 IEEE International Symposium on Circuits and Systems (ISCAS), Seattle, WA, USA, 2008, pp. 1192-1195, doi: 10.1109/ISCAS.2008.4541637.


## Uso del simulador

Para el uso del simulador es necesario:

-   Compilar el código fuente mediante un _toolchain_ de gcc específico para la arquitectura que utiliza el simulador.
-   Definir la configuración deseada con las caracteristicas de la caché
    de la simulación.

### Compilar el código fuente

Para compilar el código fuente utilice.

`sslittle-na-sstrix-gcc -O0 hello.c -o hello`

El `-O0` indica que se utiliza el nivel de optimización 0, hay 4 niveles
lo cuales tendrá que variar durante las pruebas `O0`, `O1`, `O2` y `O3`.

Este toolchain se encuentra, en la máquina virtual, en la siguiente
dirección: `$HOME/simplescalar/bin`. 

**Recomendación**: exporte al PATH los directorios mencionados anteriormente de forma permanente en el archivo `.bashrc`
([guía](https://phoenixnap.com/kb/linux-add-to-path)). 

```bash
export PATH=$HOME/wattch/sim-wattch-1.02d:$PATH
export PATH=$HOME/simplescalar/bin:$PATH
```

Usted puede utilizar el archivo `tools.setup` para cargar los directorios de las herramientas a utilizar en este laboratorio en la variable `$PATH`, haciendo

```bash
source tools.setup
```

### Configuración de la caché

La configuración base que usted utilizará en este laboratorio es:

|                     | L1 Inst | L1 Data | L2 Unified |
|---------------------|---------|---------|------------|
| Tamaño (KB)         | 8       | 8       | 128        |
| Número de conjuntos | 256     | 256     | 4096       |
| Asociatividad       | 1       | 1       | 1          |
| Línea de cache (B)  | 32      | 32      | 32         |

La configuración anterior se configura en el simulador de la siguiente
forma.

```bash
sim-outorder -cache:dl1 dl1:256:32:1:l -cache:il1 il1:256:32:1:l  -cache:il2 dl2 -cache:dl2 dl2:4096:32:1:l <binary>
```

Esta versión de sim-outorder se encuentra en la máquina vitual en la
siguiente dirección: `$HOME/wattch/sim-wattch-1.02d`

## Parte 1: Arreglo 2D

Usted empleará un pequeño programa en C que llenará una matriz de dos
dimensiones, el mismo se realiza con dos lazos. Para asignar los datos a
la matriz, se utiliza la suma del índice del lazo interno y externo
actual, i.e., `A[i][j]=i+j`.

```c
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            A[i][j] = i + j;
```

En esta sección del laboratorio, usted analizará el rendimiento y el consumo de potencia, de este sencillo programa, esto desde dos puntos de vista de:

-   El efecto de las transformaciones de lazo.
-   El efecto de la optmizacion del compilador.

### Transformación de lazos

Usted realizará la transformación de lazos al código de arreglo 2D, utilizando
distintas dimensiones para la variable `SIZE`: 16x16, 32x23, 64x64, 128x128.
Para este experimento utilice la configuración de la caché presentada
anteriomente.

La siguiente lista presenta posibles [transformaciones de lazo](https://tecnube1-my.sharepoint.com/:b:/g/personal/jocastro_itcr_ac_cr/EcnKvkGM_XdFmy0fuHxgxecBVO4s_YqyxplXeqCuH0X5JA?e=88hV0w) a realizar. 

-   Loop permutation
-   Loop unrrolling
-   Loop tiling


Note que se pueden implementar y analizar las transformaciones
de manera independente o bien combinadas para cada tamaño del arreglo,
además algunas transformaciones, como loop tiling, sólo tienen sentido para tamaños especificos.

Reporte y analice brevemente para cada uno de los tipos de trasformación utilizados. Agregue los archivos generados, a partir de `array.c`, en su repositorio.

### Niveles de optimización del compilador

En esta sección se desea medir el efecto de los niveles de
optimizacion del compilador. Para en el que el arreglo tiene dimensión
32x32, reporte y analice brevemente para los siguientes niveles de optimización: `O0`, `O1`, `O2`, `O3`.

## Parte 2: _Benchmark_

### Optimización del compilador 
Usted ejecutará las 5 aplicaciones con las diferentes niveles de optimización del compilador y la configuración de la caché base descrita. Reporte y discuta brevemente el efecto de las diferentes optimizaciones.


### Configuración de la caché
Considere además el efecto de modificar la jerarquía de memoria, particularmente el nivel L1 de la caché de datos. Proponga al menos 5 diferentes escenarios para esta memoria caché en donde varíe el nivel de asociatividad (y por lo tanto la cantidad de conjuntos), el tamaño de la línea de caché y la estrategia de reemplazo de bloque (LRU, FIFO o random). En todo caso, el tamaño de esta memoria caché no debe cambiar y el nivel de optimización en la compilación debe ser `O0`.

Reporte y discuta brevemente los resultados obtenidos para los diferentes escenarios y para las 5 aplicaciones. Para el caso de las modifiaciones de la caché L1 de datos, utilice la media geométrica para determinar cuál es la mejor configuración tomando en cuenta el rendimiento de la caché y el tiempo de ejecución.

## Mediciones y análisis

Para todas las experimentaciones que realizará en este laboratorio, extraiga los siguientes datos de los reportes generados por el simulador:

-   Potencia consumida por el software.
-   Número de instrucciones ejecutadas y tiempo de ejecución.
-   Rendimiento de caché.

## Parte 3: _Análisis de resultados_

## 1

### Tabla Array diferente optimizacion

|				|00 			| 01 			| 02 			| 03 			|
|:---:			| :---:			| :---: 		| :---: 		| :---: 		|
|N. insn		| 75004| 28507| 26138| 26116|
|T. sim			| 40462| 17554| 17040| 16913|
|cycles/insn	| 0.5522| 0.6449| 0.691| 0.6862|
|N. misses		| 571| 571| 571| 571|
|Miss rate		| 0.0245| 0.0834| 0.0834| 0.0834|
|Total Power	|2655060.06|1151869.02|1118141.056|1109807.493|
|TotalPwr/insn	|35.3989|40.4065|42.7784|42.4953|
|dcache access	|23319|6845|6845|6845|

### Tabla Array diferente tamaño

|				| 16 			| 32 			| 64 			| 128 			|
|:---:			| :---:			| :---: 		| :---: 		| :---: 		|
|N. insn		|24923|			|75003|			|270405|		|1042114|
|T. sim			|19194|         |40518|         |122453|        |445753|
|cycles/insn	|0.8043|        |0.5530|        |0.4581|        |0.4301|
|N. misses		|471|           |571|           |6183|          |23334|
|Miss rate		|0.0530|        |0.0245|        |0.0775|        |0.0769|
|Total Power	|1259483.5349|  |2658734.7019|  |8035195.2330|  |29249690.7443|
|TotalPwr/insn	|50.5350|       |35.4484|       |29.7154|       |28.0676|
|dcache access	|8890|          |23321|         |79732|         |303320|

### adpcm
|				|00 			| 01 			| 02 			| 03 			|
|:---:			| :---:			| :---: 		| :---: 		| :---: 		|
|N. insn		| 243576		| 121489		| 119048		| 118522		|
|T. sim			| 183988		| 72306			| 70707			| 69911			|
|cycles/insn	| 0.7831		| 0.6287		| 0.6253		| 0.6222		|
|N. misses		| 764			| 761			| 762			| 761			|
|Miss rate		| 0.0073		| 0.0237		| 0.0236		| 0.0237		|
|Total Power	| 12073036.19	| 4744618.968	| 4639694.816	| 4587462.406	|
|TotalPwr/insn	| 49.5658		| 39.0539		| 38.9733		| 38.7056		|
|dcache access	| 104840		| 32047			| 32321			| 32070			|

### aes
|				|00 			| 01 			| 02 			| 03 			|
|:---:			| :---:			| :---: 		| :---: 		| :---: 		|
|N. insn		| 126687		| 70887			| 57500			| 57500			|
|T. sim			| 111160		| 60872			| 50103			| 50103			|	
|cycles/insn	| 0.9988		| 1.1146		| 1.2181		| 1.2181		|
|N. misses		| 622  			| 619			| 620			| 620			|
|Miss rate		| 0.0188		| 0.0481		| 0.0513		| 0.0513		|
|Total Power	| 7294164.309	| 3994335.821	| 3287689.046	| 3287689.046	|
|TotalPwr/insn	| 57.5763		| 56.3479		| 57.1772		| 65.6186		|
|dcache access	| 33055			| 12863			| 12079			| 12079			|

### fft
|				|00 			| 01 			| 02 			| 03 			|
|:---:			| :---:			| :---: 		| :---: 		| :---: 		|
|N. insn		| 65537			| 37549         | 36378         | 37549|
|T. sim			| 42779			| 25875         | 25273         | 25875|	
|cycles/insn	| 0.6709		| 0.7158        | 0.7165        | 0.7158|
|N. misses		| 493			| 493           | 493           | 493|
|Miss rate		| 0.0214		| 0.0656        | 0.0656        | 0.0656|
|Total Power	| 2807098.371	| 1697881.446   | 1658379.044   | 1697881.446|
|TotalPwr/insn	| 42.8323		| 45.2178       | 45.5874       | 45.2178|
|dcache access	| 23088			| 7513          | 7510          | 7513|

### jpeg
|				|00 			| 01 			| 02 			| 03 			|
|:---:			| :---:			| :---: 		| :---: 		| :---: 		|
|N. insn		| 6190956| 3313695| 3250896| 2899778|
|T. sim			| 3165187| 1550458| 1529354| 1362811|	
|cycles/insn	| 0.5399| 0.5332| 0.536| 0.5322|
|N. misses		| 26741| 16404| 16719| 19373|
|Miss rate		| 0.0143| 0.0213| 0.0219| 0.0334|
|Total Power	| 207695160.5| 101738893.5| 100354078.5| 89425758.87|
|TotalPwr/insn	| 33.5482| 30.7026| 30.8697| 30.8388|
|dcache access	| 1872396| 770213| 762276| 579534|

### matrixmultiply
|				|00 			| 01 			| 02 			| 03 			|
|:---:			| :---:			| :---: 		| :---: 		| :---: 		|
|N. insn		| 345093| 137876| 113862| 107730|
|T. sim			| 175767| 82409| 63048| 60667|	
|cycles/insn	| 0.5127| 0.6139| 0.5699| 0.5814|
|N. misses		| 615| 626| 626| 625|
|Miss rate		| 0.0067| 0.0303| 0.0303| 0.0303|
|Total Power	| 11533585.62| 5407563.751| 4137121.908| 3980884.006|
|TotalPwr/insn	| 33.4217| 39.2205| 36.3345| 36.9524|
|dcache access	| 91549| 20644| 20645| 20641|



## Evaluación
Este laboratorio se evaluará con la siguiente rúbrica


| Rubro | % | C | EP | D | NP |
|-------|---|---|----|---|----|
|Transformaciones manuales| 50|   |    |   |    |
|Optimización del compilador| 10|   |    |   |    |
|Experimentación con _benchmark_| 20|   |    |   |    |
|Uso de repositorio|20|   |    |   |    |

C: Completo,
EP: En progreso ($\times 0,8$),
D: Deficiente ($\times 0,5$),
NP: No presenta ($\times 0$)

- El uso del repositorio implica que existan contribuciones de todos los miembros del equipo. El último _commit_ debe registrarte antes de las 23:59 del viernes 24 de marzo de 2023.
