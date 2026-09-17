#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TAMANO_MEMORIA 1000
#define CENTINELA 99999
#define ARCHIVO_PROGRAMA "programa.simp"

/* Opcodes SML */
#define LEE 10
#define ESCRIBE 11
#define NUEVA_LINEA 12
#define LEE_CADENA 13
#define ESCRIBE_CADENA 14

#define CARGA 20
#define ALMACENA 21

#define SUMA 30
#define RESTA 31
#define DIVIDE 32
#define MULTIPLICA 33
#define MODULO 34
#define EXPONENCIACION 35

#define BIFURCA 40
#define BIFURCANEG 41
#define BIFURCACERO 42
#define ALTA 43

/* Declaración de prototipos */
void mostrarEncabezado(void);
int cargarPrograma(double memoria[]);
void ejecutarPrograma(double memoria[], double *acumulador, int *contadorInstrucciones, 
                      int *registroInstruccion, int *codigoOperacion, int *operando);
void vaciadoMemoria(const double memoria[], double acumulador, int contadorInstrucciones, 
                    int registroInstruccion, int codigoOperacion, int operando);

int main(void) {
    double memoria[TAMANO_MEMORIA] = {0.0};
    double acumulador = 0.0;
    int contadorInstrucciones = 0;
    int registroInstruccion = 0;
    int codigoOperacion = 0;
    int operando = 0;

    mostrarEncabezado();
    
    if (cargarPrograma(memoria)) {
        ejecutarPrograma(memoria, &acumulador, &contadorInstrucciones, 
                         &registroInstruccion, &codigoOperacion, &operando);
    }

    return 0;
}

void mostrarEncabezado(void) {
    printf("*** Bienvenido a Simpletron v2.0 (1000 Memoria / Flotantes / Strings) ***\n\n");
}

/* Modificación 1: Intentar cargar desde archivo programa.simp, si no, modo interactivo */
int cargarPrograma(double memoria[]) {
    FILE *archivo = fopen(ARCHIVO_PROGRAMA, "r");
    int i = 0;
    double valor;

    if (archivo != NULL) {
        printf("*** Cargando programa desde el archivo '%s'... ***\n", ARCHIVO_PROGRAMA);
        while (i < TAMANO_MEMORIA && fscanf(archivo, "%lf", &valor) == 1) {
            if ((int)valor == CENTINELA) break;
            memoria[i++] = valor;
        }
        fclose(archivo);
        printf("*** Carga desde archivo completada (%d instrucciones/datos). ***\n\n", i);
        return 1;
    }

    // Modo interactivo si no existe el archivo
    printf("*** Archivo '%s' no encontrado. Iniciando modo interactivo ***\n", ARCHIVO_PROGRAMA);
    printf("*** Introduzca instrucciones SML (Centinela: %d) ***\n\n", CENTINELA);

    while (i < TAMANO_MEMORIA) {
        printf("%03d ? ", i);
        if (scanf("%lf", &valor) != 1) {
            printf("*** Entrada invalida. Ingrese un numero. ***\n");
            while (getchar() != '\n');
            continue;
        }

        if ((int)valor == CENTINELA) break;

        if (valor < -99999.0 || valor > 99999.0) {
            printf("*** Valor fuera de rango (-99999 a +99999) ***\n");
            continue;
        }

        memoria[i++] = valor;
    }

    printf("*** Carga interactiva completada ***\n\n");
    return 1;
}

void ejecutarPrograma(double memoria[], double *acumulador, int *contadorInstrucciones, 
                      int *registroInstruccion, int *codigoOperacion, int *operando) {
    double datoEntrada;
    char bufferCadena[100];

    while (*contadorInstrucciones < TAMANO_MEMORIA) {
        *registroInstruccion = (int)memoria[*contadorInstrucciones];

        // Formato para 1000 posiciones: OP de 2 dígitos, ADDR de 3 dígitos
        *codigoOperacion = abs(*registroInstruccion) / 1000;
        *operando = abs(*registroInstruccion) % 1000;

        switch (*codigoOperacion) {
            case LEE:
                printf("? ");
                scanf("%lf", &datoEntrada);
                memoria[*operando] = datoEntrada;
                (*contadorInstrucciones)++;
                break;

            case ESCRIBE:
                printf("Salida: %+09.2f\n", memoria[*operando]);
                (*contadorInstrucciones)++;
                break;

            case NUEVA_LINEA: // Modificación 5
                printf("\n");
                (*contadorInstrucciones)++;
                break;

          case LEE_CADENA: // Modificación 6 (Ajustada)
                printf("Ingrese cadena: ");
                fflush(stdout);
                // Limpiar cualquier residuo previo en el búfer de entrada
                int c;
            while ((c = getchar()) != '\n' && c != EOF); 
    
                if (fgets(bufferCadena, sizeof(bufferCadena), stdin) != NULL) {
                bufferCadena[strcspn(bufferCadena, "\n")] = '\0'; // Quitar el salto de línea
                int lon = strlen(bufferCadena);
                memoria[*operando] = lon; // Guardar la longitud
        
            for (int k = 0; k < lon; k++) {
                if (*operando + 1 + k < TAMANO_MEMORIA) {
                memoria[*operando + 1 + k] = (double)bufferCadena[k]; // Guardar ASCII
            }
        }
    }
            (*contadorInstrucciones)++;
             break;

            case ESCRIBE_CADENA: // Modificación 7
                {
                    int lon = (int)memoria[*operando];
                    if (lon < 0 || *operando + lon >= TAMANO_MEMORIA) {
                        printf("\n*** ERROR FATAL: Cadena invalida o fuera de limites ***\n");
                        vaciadoMemoria(memoria, *acumulador, *contadorInstrucciones, *registroInstruccion, *codigoOperacion, *operando);
                        return;
                    }
                    printf("Cadena: ");
                    for (int k = 0; k < lon; k++) {
                        char c = (char)memoria[*operando + 1 + k];
                        printf("%c", c);
                    }
                    printf("\n");
                    (*contadorInstrucciones)++;
                }
                break;

            case CARGA:
                *acumulador = memoria[*operando];
                (*contadorInstrucciones)++;
                break;

            case ALMACENA:
                memoria[*operando] = *acumulador;
                (*contadorInstrucciones)++;
                break;

            case SUMA:
                *acumulador += memoria[*operando];
                (*contadorInstrucciones)++;
                break;

            case RESTA:
                *acumulador -= memoria[*operando];
                (*contadorInstrucciones)++;
                break;

            case DIVIDE:
                if (memoria[*operando] == 0.0) {
                    printf("\n*** ERROR FATAL: Division entre cero ***\n");
                    vaciadoMemoria(memoria, *acumulador, *contadorInstrucciones, *registroInstruccion, *codigoOperacion, *operando);
                    return;
                }
                *acumulador /= memoria[*operando];
                (*contadorInstrucciones)++;
                break;

            case MULTIPLICA:
                *acumulador *= memoria[*operando];
                (*contadorInstrucciones)++;
                break;

            case MODULO: // Modificación 3
                if ((int)memoria[*operando] == 0) {
                    printf("\n*** ERROR FATAL: Modulo entre cero ***\n");
                    vaciadoMemoria(memoria, *acumulador, *contadorInstrucciones, *registroInstruccion, *codigoOperacion, *operando);
                    return;
                }
                *acumulador = (int)*acumulador % (int)memoria[*operando];
                (*contadorInstrucciones)++;
                break;

            case EXPONENCIACION: // Modificación 4
                *acumulador = pow(*acumulador, memoria[*operando]);
                (*contadorInstrucciones)++;
                break;

            case BIFURCA:
                *contadorInstrucciones = *operando;
                break;

            case BIFURCANEG:
                if (*acumulador < 0.0) *contadorInstrucciones = *operando;
                else (*contadorInstrucciones)++;
                break;

            case BIFURCACERO:
                if (*acumulador == 0.0) *contadorInstrucciones = *operando;
                else (*contadorInstrucciones)++;
                break;

            case ALTA:
                printf("\n*** Ejecucion terminada correctamente ***\n");
                vaciadoMemoria(memoria, *acumulador, *contadorInstrucciones, *registroInstruccion, *codigoOperacion, *operando);
                return;

            default:
                printf("\n*** ERROR FATAL: Codigo de operacion invalido (%02d) ***\n", *codigoOperacion);
                vaciadoMemoria(memoria, *acumulador, *contadorInstrucciones, *registroInstruccion, *codigoOperacion, *operando);
                return;
        }
    }
}

void vaciadoMemoria(const double memoria[], double acumulador, int contadorInstrucciones, 
                    int registroInstruccion, int codigoOperacion, int operando) {
    printf("\nREGISTROS:\n");
    printf("%-22s %+09.2f\n", "acumulador", acumulador);
    printf("%-22s      %03d\n", "contadorInstrucciones", contadorInstrucciones);
    printf("%-22s   %+06d\n", "registroInstruccion", registroInstruccion);
    printf("%-22s       %02d\n", "codigoOperacion", codigoOperacion);
    printf("%-22s      %03d\n", "operando", operando);

    printf("\nMEMORIA (Primeras 100 celdas desglosadas):\n       ");
    for (int col = 0; col < 10; col++) printf("%7d ", col);
    printf("\n");

    for (int fila = 0; fila < 100; fila += 10) {
        printf("%03d  ", fila);
        for (int col = 0; col < 10; col++) {
            printf(" %+07.1f", memoria[fila + col]);
        }
        printf("\n");
    }
}