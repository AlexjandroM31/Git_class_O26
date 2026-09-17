# Simpletron Machine Language (SML) Simulator v2.0

Simulador en C de la arquitectura Simpletron, actualizado para soportar arquitectura ampliada, punto flotante, manipulación de cadenas de texto y carga de programas desde archivo.

## 🚀 Características y Mejoras Implementadas

1. **Carga desde archivo (`programa.simp`)**: Carga automática de instrucciones al iniciar. Si el archivo no existe, conmuta a modo interactivo por teclado.
2. **Memoria expandida (1000 posiciones)**: Direccionamiento desde `000` hasta `999`.
3. **Punto Flotante**: Memoria de tipo `double` que permite leer, operar y almacenar números con decimales.
4. **Operaciones Aritméticas Extendidas**:
   - Módulo / Residuo (`MOD`)
   - Exponenciación (`EXP`)
5. **Salida de Nueva Línea**: Instrucción dedicada para formato de consola.
6. **Entrada / Salida de Cadenas**: Formato de almacenamiento secuencial en memoria (longitud en la celda base seguida de valores ASCII).

---

## 📜 Tabla de Instrucciones SML (Opcodes)

Formato de instrucción: **`AABBB`** donde `AA` es el código de operación (2 dígitos) y `BBB` es la dirección de memoria/operando (3 dígitos).

| Categoría | Opcode | Nombre | Descripción |
| :--- | :---: | :--- | :--- |
| **E/S y Cadenas** | `10` | `LEE` | Lee un número desde el teclado y lo guarda en `memoria[BBB]`. |
| | `11` | `ESCRIBE` | Muestra en pantalla el valor de `memoria[BBB]`. |
| | `12` | `NUEVA_LINEA` | Muestra un salto de línea (`\n`) en la salida. |
| | `13` | `LEE_CADENA` | Lee una cadena por teclado, guarda su longitud en `memoria[BBB]` y sus caracteres ASCII en las celdas contiguas. |
| | `14` | `ESCRIBE_CADENA` | Lee la longitud en `memoria[BBB]` e imprime la cadena almacenada en las celdas continuas. |
| **Transferencia** | `20` | `CARGA` | Carga el valor de `memoria[BBB]` en el **Acumulador**. |
| | `21` | `ALMACENA` | Almacena el valor del **Acumulador** en `memoria[BBB]`. |
| **Aritmética** | `30` | `SUMA` | `Acumulador += memoria[BBB]` |
| | `31` | `RESTA` | `Acumulador -= memoria[BBB]` |
| | `32` | `DIVIDE` | `Acumulador /= memoria[BBB]` (valida división por 0). |
| | `33` | `MULTIPLICA` | `Acumulador *= memoria[BBB]` |
| | `34` | `MODULO` | `Acumulador = Acumulador % memoria[BBB]` |
| | `35` | `EXPONENCIACION`| `Acumulador = Acumulador ^ memoria[BBB]` |
| **Control** | `40` | `BIFURCA` | Salta a la dirección `BBB`. |
| | `41` | `BIFURCANEG` | Salta a `BBB` si el **Acumulador** es negativo. |
| | `42` | `BIFURCACERO` | Salta a `BBB` si el **Acumulador** es cero. |
| | `43` | `ALTA` | Finaliza la ejecución y genera el vaciado de memoria (*memory dump*). |

---

## 🛠️ Compilación y Ejecución

Compila con `gcc` habilitando la librería matemática (`-lm`):

```bash
gcc CodigoSimpletron.c -o simpletron.exe -lm
.\simpletron.exe