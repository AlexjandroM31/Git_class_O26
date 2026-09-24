#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 100
#define SENTINEL 9999

// Definición de Operaciones SML
// Operaciones de Entrada/Salida
#define READ 10
#define WRITE 11

// Operaciones de Carga y Almacenamiento
#define LOAD 20
#define STORE 21

// Operaciones Aritméticas
#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33

// Operaciones de Transferencia de Control
#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43

// Prototipos de funciones
void displayWelcomeMessage(void);
void loadProgram(int memory[]);
void executeProgram(int memory[]);
void memoryDump(int accumulator, int instructionCounter, int instructionRegister, 
                int operationCode, int operand, const int memory[]);
int isValidWord(int word);

int main(void) {
    int memory[MEMORY_SIZE] = {0}; // Inicializar memoria en 0

    displayWelcomeMessage();
    loadProgram(memory);
    executeProgram(memory);

    return 0;
}

void displayWelcomeMessage(void) {
    printf("*** Bienvenid@ a Simpletron! ***\n");
    printf("*** Por favor, introduzca en su programa una instruccion ***\n");
    printf("*** (o palabra de datos) a la vez. Yo tipeare la ***\n");
    printf("*** posicion de memoria y un signo de interrogacion (?). ***\n");
    printf("*** Usted tipeela palabra para esa posicion. Tipee el ***\n");
    printf("*** centinela 9999 para terminar la introduccion de ***\n");
    printf("*** su programa. ***\n\n");
}

void loadProgram(int memory[]) {
    int instructionPointer = 0;
    int instruction = 0;

    while (instructionPointer < MEMORY_SIZE) {
        printf("%02d ? ", instructionPointer);
        if (scanf("%d", &instruction) != 1) {
            printf("*** Entrada invalida. Por favor ingrese un numero entero. ***\n");
            while (getchar() != '\n'); // Limpiar búfer de entrada
            continue;
        }

        if (instruction == SENTINEL) {
            printf("*** Carga del programa completada ***\n");
            printf("*** Empieza la ejecucion del programa ***\n\n");
            break;
        }

        if (!isValidWord(instruction)) {
            printf("*** Instruccion fuera de rango (-9999 a +9999). Intente de nuevo. ***\n");
            continue;
        }

        memory[instructionPointer] = instruction;
        instructionPointer++;
    }

    if (instructionPointer == MEMORY_SIZE) {
        printf("*** Memoria llena. Iniciando ejecucion... ***\n\n");
    }
}

void executeProgram(int memory[]) {
    // Registros de la Simpletron
    int accumulator = 0;
    int instructionCounter = 0;
    int instructionRegister = 0;
    int operationCode = 0;
    int operand = 0;

    int fatalError = 0;
    int isRunning = 1;

    while (isRunning && !fatalError) {
        // --- 1. FETCH (Búsqueda) ---
        instructionRegister = memory[instructionCounter];

        // --- 2. DECODE (Decodificación) ---
        operationCode = instructionRegister / 100;
        operand = instructionRegister % 100;

        // Validar operando de memoria
        if (operand < 0 || operand >= MEMORY_SIZE) {
            printf("\n*** ERROR FATAL: Acceso a direccion de memoria invalida (%02d) ***\n", operand);
            fatalError = 1;
            break;
        }

        // Incremento preventivo del contador de instrucciones
        instructionCounter++;

        // --- 3. EXECUTE (Ejecución) ---
        switch (operationCode) {
            case READ: {
                int value = 0;
                int valid = 0;
                while (!valid) {
                    printf("Entrada ? ");
                    if (scanf("%d", &value) == 1 && isValidWord(value)) {
                        valid = 1;
                    } else {
                        printf("*** Entrada invalida. Ingrese un valor entre -9999 y +9999 ***\n");
                        while (getchar() != '\n');
                    }
                }
                memory[operand] = value;
                break;
            }

            case WRITE:
                printf("Salida: %+05d\n", memory[operand]);
                break;

            case LOAD:
                accumulator = memory[operand];
                break;

            case STORE:
                memory[operand] = accumulator;
                break;

            case ADD:
                accumulator += memory[operand];
                if (!isValidWord(accumulator)) {
                    printf("\n*** ERROR FATAL: Desbordamiento del acumulador (Overflow) ***\n");
                    fatalError = 1;
                }
                break;

            case SUBTRACT:
                accumulator -= memory[operand];
                if (!isValidWord(accumulator)) {
                    printf("\n*** ERROR FATAL: Desbordamiento del acumulador (Underflow) ***\n");
                    fatalError = 1;
                }
                break;

            case DIVIDE:
                if (memory[operand] == 0) {
                    printf("\n*** ERROR FATAL: Intento de division entre cero ***\n");
                    fatalError = 1;
                } else {
                    accumulator /= memory[operand];
                }
                break;

            case MULTIPLY:
                accumulator *= memory[operand];
                if (!isValidWord(accumulator)) {
                    printf("\n*** ERROR FATAL: Desbordamiento del acumulador ***\n");
                    fatalError = 1;
                }
                break;

            case BRANCH:
                instructionCounter = operand;
                break;

            case BRANCHNEG:
                if (accumulator < 0) {
                    instructionCounter = operand;
                }
                break;

            case BRANCHZERO:
                if (accumulator == 0) {
                    instructionCounter = operand;
                }
                break;

            case HALT:
                printf("*** Terminó la ejecucion de Simpletron ***\n");
                isRunning = 0;
                break;

            default:
                printf("\n*** ERROR FATAL: Codigo de operacion invalido (%02d) ***\n", operationCode);
                fatalError = 1;
                break;
        }

        // Control adicional por si el Instruction Counter sale de los límites
        if (instructionCounter < 0 || instructionCounter >= MEMORY_SIZE) {
            printf("\n*** ERROR FATAL: Contador de instrucciones fuera de rango ***\n");
            fatalError = 1;
        }
    }

    if (fatalError) {
        printf("*** Simpletron ejecuto un apagado anormal por error fatal ***\n");
    }

    // Imprimir el Vaciado de Memoria (Memory Dump) al finalizar
    memoryDump(accumulator, instructionCounter, instructionRegister, operationCode, operand, memory);
}

void memoryDump(int accumulator, int instructionCounter, int instructionRegister, 
                int operationCode, int operand, const int memory[]) {
    printf("\nREGISTROS:\n");
    printf("acumulador           %+05d\n", accumulator);
    printf("contadorInstruccion     %02d\n", instructionCounter);
    printf("registroInstruccion  %+05d\n", instructionRegister);
    printf("codigoOperacion         %02d\n", operationCode);
    printf("operando                %02d\n\n", operand);

    printf("MEMORIA:\n");
    printf("%8d", 0);
    for (int i = 1; i < 10; i++) {
        printf("%7d", i);
    }
    printf("\n");

    for (int row = 0; row < 100; row += 10) {
        printf("%02d ", row);
        for (int col = 0; col < 10; col++) {
            printf(" %+05d", memory[row + col]);
        }
        printf("\n");
    }
    printf("\n");
}

int isValidWord(int word) {
    return (word >= -9999 && word <= 9999);
}