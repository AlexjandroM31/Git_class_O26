# 🖥️ Simpletron Hardware Simulator (SML)

![C Architecture](https://img.shields.io/badge/Language-C99-blue.svg)
![Status](https://img.shields.io/badge/Status-Completed-success.svg)

Simulador por software de la computadora **Simpletron**, desarrollado en C. Este proyecto implementa la ejecución paso a paso de programas escritos en **Simpletron Machine Language (SML)**, modelando la memoria principal, los registros de control y el ciclo clásico de ejecución (*Fetch-Decode-Execute*).

Basado en las especificaciones del libro *Cómo programar en C* (Deitel & Deitel).

---

## 📑 Tabla de Contenidos

- [Características Principales](#-características-principales)
- [Arquitectura del Sistema](#-arquitectura-del-sistema)
- [Conjunto de Instrucciones SML](#-conjunto-de-instrucciones-sml)
- [Requisitos e Instalación](#-requisitos-e-instalación)
- [Casos de Prueba](#-casos-de-prueba)
  - [Prueba 1: Ejecución Correcta (Suma de 2 números)](#prueba-1-ejecución-correcta-suma-de-2-números)
  - [Prueba 2: Error Fatal (División entre cero)](#prueba-2-error-fatal-división-entre-cero)
- [Estructura del Proyecto](#-estructura-del-proyecto)

---

## 🚀 Características Principales

* **Memoria Principal:** Arreglo simulado de 100 posiciones (`00` a `99`) para datos e instrucciones.
* **Ciclo de Instrucción Completo:** Implementación de las fases de *Fetch*, *Decode* y *Execute*.
* **Control de Errores e Integridad:**
  * Validación de rango de palabras introducidas ($-9999$ a $+9999$).
  * Detección de desbordamiento (*Overflow* y *Underflow*) en el acumulador.
  * Protección contra división entre cero.
  * Prevención de accesos fuera de rango en memoria e instrucciones inválidas.
* **Vaciado de Memoria (Memory Dump):** Despliegue visual completo de los registros internos y el estado de la memoria tras finalizar la ejecución o al ocurrir un error fatal.

---

## 📐 Arquitectura del Sistema

El simulador cuenta con los siguientes componentes:

| Componente | Variable en C | Descripción |
| :--- | :--- | :--- |
| **Acumulador** | `accumulator` | Registro donde se realizan las operaciones aritméticas y de carga. |
| **Contador de Instrucciones** | `instructionCounter` | Apunta a la dirección de la siguiente instrucción a ejecutar. |
| **Registro de Instrucción** | `instructionRegister` | Contiene la instrucción actual extraída de memoria. |
| **Código de Operación** | `operationCode` | Extrae los dos primeros dígitos de la instrucción (operación a realizar). |
| **Operando** | `operand` | Extrae los dos últimos dígitos de la instrucción (dirección de memoria). |

---

## 📜 Conjunto de Instrucciones SML

El formato de cada palabra SML es de cuatro dígitos con signo (`+OPADDR` / `-OPADDR`).

| Categoría | Código (OP) | Mnemónico | Descripción |
| :--- | :---: | :--- | :--- |
| **Entrada / Salida** | `10` | `READ` | Lee una palabra desde el teclado y la almacena en `memory[operand]`. |
| | `11` | `WRITE` | Muestra en pantalla el contenido de `memory[operand]`. |
| **Carga / Almacenamiento** | `20` | `LOAD` | Carga una palabra de `memory[operand]` en el acumulador. |
| | `21` | `STORE` | Almacena el valor del acumulador en `memory[operand]`. |
| **Aritmética** | `30` | `ADD` | Suma `memory[operand]` al acumulador. |
| | `31` | `SUBTRACT` | Resta `memory[operand]` al acumulador. |
| | `32` | `DIVIDE` | Divide el acumulador entre `memory[operand]`. |
| | `33` | `MULTIPLY` | Multiplica el acumulador por `memory[operand]`. |
| **Transferencia de Control**| `40` | `BRANCH` | Salto incondicional a la dirección `operand`. |
| | `41` | `BRANCHNEG` | Salta a `operand` si el acumulador es negativo. |
| | `42` | `BRANCHZERO`| Salta a `operand` si el acumulador es cero. |
| | `43` | `HALT` | Detiene la ejecución del programa. |

---

## 🛠️ Requisitos e Instalación

### Requisitos Previos
* Compilador **GCC** o **Clang** compatible con C99 o superior.

### Compilación
```bash
gcc -Wall -Wextra -std=c99 simpletron.c -o simpletron