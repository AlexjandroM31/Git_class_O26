# Simulador de Computadora Simpletron (SML)

Este proyecto es una simulación en C de la arquitectura de la computadora **Simpletron**, capaz de cargar y ejecutar programas escritos en el lenguaje de máquina SML (Simpletron Machine Language).

## Características Implementadas
- Administra una memoria principal de 100 posiciones (`00` a `99`).
- Registros de CPU simulados: `accumulator`, `instructionCounter`, `instructionRegister`, `operationCode` y `operand`.
- Implementación de las 12 operaciones base de SML (Entrada/Salida, Carga/Almacenamiento, Aritmética y Transferencia de control).
- Detección de errores fatales (división entre cero, desbordamiento del acumulador, códigos de operación inválidos).
- Vaciado de memoria (*memory dump*) y registros al finalizar la ejecución o por error fatal.

## Requisitos
- Compilador GCC (MinGW para Windows o nativo en Linux/Mac).

## Compilación y Ejecución

Desde la terminal integrada, ejecuta los siguientes comandos:

```bash
# Compilar el código fuente
gcc CodigoSimpletron.c -o simpletron.exe

# Ejecutar el simulador
.\simpletron.exe