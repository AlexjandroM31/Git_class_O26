# Proyecto Final: Compilador y Simulador Simpletron

## Descripción general del proyecto
Este proyecto integra el desarrollo completo de un simulador de la computadora virtual Simpletron y la construcción de un compilador de dos pasadas para el lenguaje de alto nivel "Simple". El sistema permite escribir programas en lenguaje Simple, convertirlos a lenguaje de máquina SML (Simpletron Machine Language) mediante el compilador, y ejecutarlos directamente en el simulador modificado. 

El proyecto demuestra el ciclo completo de procesamiento de un programa, desde el análisis léxico y la tabla de símbolos hasta el vaciado de memoria post-ejecución, incluyendo la evaluación de expresiones infijas a postfijas.

## Integrantes
*   [Tu Nombre Completo] - Desarrollador Principal

## Descripción de archivos principales
*   **`compilador.cpp`**: Archivo fuente en C++ que contiene el compilador completo. Lee un archivo con extensión `.simple`, realiza dos pasadas para construir la tabla de símbolos y resolver referencias hacia adelante, y genera un archivo ejecutable `.sml`.
*   **`simpletron.c`**: Archivo fuente en C modificado del simulador original. Está programado para cargar instrucciones desde un archivo `.sml`, ejecutar el programa y generar un Memory Dump (vaciado de memoria) al finalizar.
*   **`programa.simple`**: Archivo de texto utilizado como entrada de prueba para el compilador. Contiene código escrito en el lenguaje de alto nivel Simple.
*   **`codigo.sml`**: Archivo generado automáticamente por el compilador, el cual contiene las instrucciones numéricas en lenguaje máquina listas para ejecutarse.

## Instrucciones de compilación
Para poder ejecutar el sistema, es necesario compilar tanto el compilador como el simulador desde una terminal (como PowerShell o Bash).

1.  **Compilar el simulador Simpletron:**
    ```bash
    gcc -o simpletron.exe simpletron.c
    ```

2.  **Compilar el compilador de Simple a SML:**
    ```bash
    g++ -o compilador.exe compilador.cpp
    ```

## Instrucciones de ejecución
El flujo de trabajo consta de dos pasos principales: primero compilar el programa fuente y luego ejecutarlo.

1.  Asegúrese de que su archivo de código fuente se llame `programa.simple` y se encuentre en el mismo directorio.
2.  **Ejecute el compilador:**
    ```bash
    .\compilador.exe
    ```
    *Este paso generará automáticamente el archivo `codigo.sml`.*
3.  **Ejecute el simulador:**
    ```bash
    .\simpletron.exe
    ```
4.  Cuando el simulador pregunte: *"Ingrese el nombre del archivo SML (o presione ENTER para entrada manual):"*, escriba **`codigo.sml`** y presione Enter.

## Ejemplos de entrada y salida
### Programa de prueba: Media de 3 números
**Entrada (Código en `programa.simple`):**
```text
10 input a
20 input b
30 input c
40 let m = ( a + b + c ) / 3
50 print m
60 end