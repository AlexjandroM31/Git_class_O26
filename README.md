# Simulador Simpletron SML — Proyecto v1.0 / v2.0

Este repositorio contiene la implementación en **Lenguaje C** del simulador de la computadora **Simpletron**, capaz de cargar, validar y ejecutar programas escritos en el lenguaje de máquina Simpletron (**SML**).

---

## 📋 Cumplimiento de la Rúbrica de Evaluación

| Criterio | Requerimiento de la Rúbrica | Estado en la Solución |
| :--- | :--- | :---: |
| **1. Compilación y Organización** | Código modular, legible, estructurado en C y libre de errores. | **Cumplido** |
| **2. Carga y Validación** | Lectura desde archivo/teclado hasta centinela `99999` (o `9999` en v1.0) y validación de rango. | **Cumplido** |
| **3. Memoria y Ciclo de Ejecución** | Administración de registros (`accumulator`, `instructionCounter`, `instructionRegister`, `operationCode`, `operand`) y ciclo Fetch-Decode-Execute. | **Cumplido** |
| **4. Implementación SML** | Soporte de operaciones de E/S, Carga/Almacenamiento, Aritmética avanzada (incluyendo módulo y exponenciación) y Cadenas. | **Cumplido** |
| **5. Transferencia de Control** | Bifurcaciones incondicionales (`40`), condicionales (`41`, `42`), parada (`43`), e instrucciones de subrutina (`44 CALL`, `45 RET`). | **Cumplido** |
| **6. Vaciado de Memoria (Dump)** | Impresión formateada de todos los registros, nivel de anidamiento y matriz de memoria con signos y encabezados. | **Cumplido** |
| **7. Manejo de Errores Fatales** | Detección de división entre cero, operando inválido, desbordamiento de pila y desbordamiento del acumulador con *dump* automático. | **Cumplido** |
| **8. Repositorio y Pruebas** | Archivos fuente, manual de uso, casos de prueba documentados y estructura limpia en GitHub. | **Cumplido** |

---

## 🌿 Estructura de Ramas en Git

* **`main`**: Versión estable del proyecto Simpletron v2.0 con soporte extendido de memoria, flotantes, manejo de cadenas y archivos.
* **`anidamiento`**: Rama de desarrollo de nueva funcionalidad que incorpora la **Pila de Llamadas (Call Stack)** para manejar subrutinas anidadas (`44 SUBRUTINA` / `45 RETORNO`) y el rastreo en tiempo real de la profundidad de anidamiento actual y máxima.

---

## 💻 Juego de Instrucciones SML (Opcodes)

| Código | Operación | Descripción |
| :---: | :--- | :--- |
| **10** | `LEE` | Lee una palabra desde el teclado hacia una ubicación de memoria. |
| **11** | `ESCRIBE` | Imprime en pantalla la palabra de una ubicación de memoria. |
| **12** | `NUEVA_LINEA` | Imprime un salto de línea en consola. |
| **13** | `LEE_CADENA` | Lee una cadena de caracteres y la almacena en memoria. |
| **14** | `ESCRIBE_CADENA` | Imprime una cadena almacenada en memoria. |
| **20** | `CARGA` | Carga una palabra desde la memoria al acumulador. |
| **21** | `ALMACENA` | Guarda la palabra del acumulador en la memoria. |
| **30** | `SUMA` | Suma una palabra de memoria al acumulador. |
| **31** | `RESTA` | Resta una palabra de memoria al acumulador. |
| **32** | `DIVIDE` | Divide el acumulador entre una palabra de memoria. |
| **33** | `MULTIPLICA` | Multiplica el acumulador por una palabra de memoria. |
| **34** | `MODULO` | Obtiene el residuo de la división del acumulador. |
| **35** | `EXPONENCIACION` | Eleva el acumulador a la potencia almacenada en memoria. |
| **40** | `BIFURCA` | Salto incondicional a la ubicación indicada. |
| **41** | `BIFURCANEG` | Salto condicional si el acumulador es negativo. |
| **42** | `BIFURCACERO` | Salto condicional si el acumulador es cero. |
| **43** | `ALTA` | Finaliza la ejecución del programa (HALT). |
| **44** | `SUBRUTINA` | Apila la dirección de retorno e incrementa la profundidad de anidamiento (`CALL`). |
| **45** | `RETORNO` | Desapila la dirección de retorno y decrementa la profundidad de anidamiento (`RET`). |

---

## 🚀 Compilación y Ejecución

### Opción A: Compilar localmente con GCC
```bash
gcc CodigoSimpletron.c -o simpletron -lm
./simpletron