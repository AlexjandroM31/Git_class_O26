#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define TAM_PILA 100

// --- ESTRUCTURA DE LA PILA DE OPERANDOS ---
typedef struct {
    float items[TAM_PILA];
    int tope;
} PilaFloat;

void initPilaFloat(PilaFloat *p) {
    p->tope = -1;
}

int isEmptyFloat(PilaFloat *p) {
    return p->tope == -1;
}

void pushFloat(PilaFloat *p, float val) {
    if (p->tope < TAM_PILA - 1) {
        p->items[++(p->tope)] = val;
    }
}

float popFloat(PilaFloat *p) {
    if (!isEmptyFloat(p)) {
        return p->items[(p->tope)--];
    }
    return 0.0f;
}

// --- ALGORITMO: EVALUACIÓN DE EXPRESIÓN EN POSTFIJO ---
float evaluarPostfijo(const char *expresion) {
    PilaFloat pila;
    initPilaFloat(&pila);

    char symb;
    int i = 0;
    float opnd1, opnd2, value;

    // 1. Nos posicionamos al inicio de la cadena
    // 2. Mientras no hayamos leído toda la cadena
    while (expresion[i] != '\0') {
        // 2.1. Copiar el siguiente carácter en symb
        symb = expresion[i];

        // Ignorar espacios en blanco si los hay
        if (symb == ' ') {
            i++;
            continue;
        }

        // 2.2. Si symb es un operando (dígito del '0' al '9')
        if (isdigit(symb)) {
            // 2.2.1. push(symb) -> Convertimos el char a float
            pushFloat(&pila, (float)(symb - '0'));
        } 
        // 2.3. Si no (es un operador)
        else {
            // 2.3.1. opnd2 = pop()
            opnd2 = popFloat(&pila);
            
            // 2.3.2. opnd1 = pop()
            opnd1 = popFloat(&pila);

            // 2.3.3 y 2.3.4. value = resultado de (opnd1 symb opnd2)
            switch (symb) {
                case '+': value = opnd1 + opnd2; break;
                case '-': value = opnd1 - opnd2; break;
                case '*': value = opnd1 * opnd2; break;
                case '/': value = (opnd2 != 0) ? (opnd1 / opnd2) : 0; break;
                case '^': value = powf(opnd1, opnd2); break;
                default:  value = 0; break;
            }

            // push(value)
            pushFloat(&pila, value);
        }

        i++;
    }

    // 3. return pop()
    return popFloat(&pila);
}

// --- PROGRAMA PRINCIPAL PARA PROBAR EL ALGORITMO ---
int main(void) {
    char expresion[100];

    printf("=== EVALUACION DE EXPRESIONES POSTFIJAS ===\n");
    printf("Ingrese la expresion en postfijo (ejemplo: 35+4*): ");
    
    if (scanf("%99s", expresion) == 1) {
        float resultado = evaluarPostfijo(expresion);
        printf("\nResultado de la evaluacion: %.2f\n", resultado);
    }

    return 0;
}