#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MEMORY_SIZE 100
#define SENTINEL 9999

// Definicion de Operaciones SML
#define READ 10
#define WRITE 11
#define LOAD 20
#define STORE 21
#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33
#define MODULUS 34        // Extension: Modulo
#define EXPONENTIATION 35 // Extension: Exponenciacion
#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43

// Prototipos
void displayWelcomeMessage(void);
void loadProgram(int memory[]);
void executeProgram(int memory[]);
void memoryDump(int accumulator, int instructionCounter, int instructionRegister, 
                int operationCode, int operand, const int memory[]);
int isValidWord(int word);

int main(void) {
    int memory[MEMORY_SIZE] = {0};

    displayWelcomeMessage();
    loadProgram(memory);
    executeProgram(memory);

    return 0;
}

void displayWelcomeMessage(void) {
    printf("*** Simulador Simpletron Avanzado ***\n");
    printf("*** Con soporte para Modulo (34) y Exponenciacion (35) ***\n\n");
}

void loadProgram(int memory[]) {
    char filename[100];
    printf("Ingrese el nombre del archivo SML (o presione ENTER para entrada manual): ");
    
    // Leer la linea de entrada
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\r\n")] = 0; // Quitar el salto de linea

    if (strlen(filename) > 0) {
        // Cargar desde archivo
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            printf("*** Error al abrir el archivo %s. Abortando. ***\n", filename);
            exit(1);
        }

        int i = 0;
        int word;
        // Leer el archivo ignorando los signos '+' si existen (+1099 -> 1099)
        while (fscanf(file, "%d", &word) != EOF && i < MEMORY_SIZE) {
            memory[i] = word;
            i++;
        }
        fclose(file);
        printf("*** Carga del archivo '%s' completada (%d instrucciones/datos) ***\n", filename, i);
        printf("*** Empieza la ejecucion del programa ***\n\n");
    } else {
        // Carga Manual (Interactivo)
        int instructionPointer = 0;
        int instruction = 0;

        printf("*** Por favor, introduzca su programa una instruccion a la vez. ***\n");
        printf("*** Tipee el centinela 9999 para terminar. ***\n");

        while (instructionPointer < MEMORY_SIZE) {
            printf("%02d ? ", instructionPointer);
            if (scanf("%d", &instruction) != 1) {
                printf("*** Entrada invalida. Por favor ingrese un numero entero. ***\n");
                while (getchar() != '\n'); 
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
    }
}

void executeProgram(int memory[]) {
    int accumulator = 0;
    int instructionCounter = 0;
    int instructionRegister = 0;
    int operationCode = 0;
    int operand = 0;
    int fatalError = 0;
    int isRunning = 1;

    while (isRunning && !fatalError) {
        if (instructionCounter < 0 || instructionCounter >= MEMORY_SIZE) {
            printf("\n*** ERROR FATAL: Contador de instrucciones fuera de rango (%d) ***\n", instructionCounter);
            fatalError = 1;
            break;
        }

        instructionRegister = memory[instructionCounter];
        operationCode = instructionRegister / 100;
        operand = instructionRegister % 100;

        if (operationCode < 0) operationCode = -operationCode;

        if (operand < 0 || operand >= MEMORY_SIZE) {
            printf("\n*** ERROR FATAL: Acceso a direccion de memoria invalida (%02d) ***\n", operand);
            fatalError = 1;
            break;
        }

        instructionCounter++;

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
                break;
            case SUBTRACT:
                accumulator -= memory[operand];
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
                break;
            case MODULUS: 
                if (memory[operand] == 0) {
                    printf("\n*** ERROR FATAL: Intento de modulo entre cero ***\n");
                    fatalError = 1;
                } else {
                    accumulator %= memory[operand];
                }
                break;
            case EXPONENTIATION: 
                accumulator = (int)pow((double)accumulator, (double)memory[operand]);
                break;
            case BRANCH:
                instructionCounter = operand;
                break;
            case BRANCHNEG:
                if (accumulator < 0) instructionCounter = operand;
                break;
            case BRANCHZERO:
                if (accumulator == 0) instructionCounter = operand;
                break;
            case HALT:
                printf("*** Termino la ejecucion de Simpletron ***\n");
                isRunning = 0;
                break;
            default:
                // Si llegamos aqui y el opcode es 0, suele ser espacio vacio de memoria
                // Si instructionRegister es 0, no hacer nada y seguir.
                if (instructionRegister != 0) {
                    printf("\n*** ERROR FATAL: Codigo de operacion invalido (%02d) ***\n", operationCode);
                    fatalError = 1;
                }
                break;
        }

        if (accumulator < -9999 || accumulator > 9999) {
            printf("\n*** ERROR FATAL: Desbordamiento del acumulador (Overflow) [%+05d] ***\n", accumulator);
            fatalError = 1;
        }
    }

    if (fatalError) {
        printf("*** La ejecucion de Simpletron se detuvo anormalmente ***\n");
    }

    memoryDump(accumulator, instructionCounter, instructionRegister, operationCode, operand, memory);
}

void memoryDump(int accumulator, int instructionCounter, int instructionRegister, 
                int operationCode, int operand, const int memory[]) {
    printf("\nREGISTROS:\n");
    printf("acumulador            %+05d\n", accumulator);
    printf("contadorInstruccion      %02d\n", instructionCounter);
    printf("registroInstruccion   %+05d\n", instructionRegister);
    printf("codigoOperacion          %02d\n", operationCode);
    printf("operando                 %02d\n\n", operand);

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