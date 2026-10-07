#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>

// ==========================================
// FASE 1: Estructuras de Datos y Globales
// ==========================================
struct TableEntry {
    int symbol;        // Unicode / ASCII o valor entero
    char type;         // 'C' = Constante, 'L' = Línea, 'V' = Variable
    int location;      // Dirección SML (00 a 99)
    TableEntry* next;  // Puntero al siguiente elemento
};

TableEntry* symbolTable = nullptr;
int memory[100] = {0};
int flags[100];
int instructionCounter = 0;
int dataCounter = 99;

void initFlags() {
    for (int i = 0; i < 100; ++i) {
        flags[i] = -1;
    }
}

TableEntry* searchSymbol(int symbol, char type) {
    TableEntry* current = symbolTable;
    while (current != nullptr) {
        if (current->symbol == symbol && current->type == type) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void insertSymbol(int symbol, char type, int location) {
    TableEntry* newEntry = new TableEntry;
    newEntry->symbol = symbol;
    newEntry->type = type;
    newEntry->location = location;
    newEntry->next = symbolTable;
    symbolTable = newEntry;
}

int getVariableLocation(char varName) {
    TableEntry* entry = searchSymbol(varName, 'V');
    if (entry != nullptr) {
        return entry->location;
    }
    int loc = dataCounter--;
    insertSymbol(varName, 'V', loc);
    return loc;
}

int getConstantLocation(int value) {
    TableEntry* entry = searchSymbol(value, 'C');
    if (entry != nullptr) {
        return entry->location;
    }
    int loc = dataCounter--;
    insertSymbol(value, 'C', loc);
    memory[loc] = value; // Carga el valor en la sección de datos
    return loc;
}

// ==========================================
// FASE 2: Conversión Infija a Posfija
// ==========================================
int prec(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

std::vector<std::string> infixToPostfix(const std::vector<std::string>& tokens) {
    std::vector<std::string> postfix;
    std::stack<std::string> opStack;

    for (const auto& token : tokens) {
        if (isalnum(token[0])) { // Variable o literal entero
            postfix.push_back(token);
        } else if (token == "(") {
            opStack.push(token);
        } else if (token == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        } else {
            while (!opStack.empty() && prec(opStack.top()[0]) >= prec(token[0])) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
    }

    while (!opStack.empty()) {
        postfix.push_back(opStack.top());
        opStack.pop();
    }

    return postfix;
}

int generatePostfixSML(const std::vector<std::string>& postfix) {
    std::stack<int> tempStack;

    for (const auto& token : postfix) {
        if (isalpha(token[0])) {
            tempStack.push(getVariableLocation(token[0]));
        } else if (isdigit(token[0]) || (token.length() > 1 && token[0] == '-')) {
            tempStack.push(getConstantLocation(std::stoi(token)));
        } else {
            int op2Loc = tempStack.top(); tempStack.pop();
            int op1Loc = tempStack.top(); tempStack.pop();

            memory[instructionCounter++] = 2000 + op1Loc; // LOAD

            if (token == "+") memory[instructionCounter++] = 3000 + op2Loc;      // ADD
            else if (token == "-") memory[instructionCounter++] = 3100 + op2Loc; // SUB
            else if (token == "/") memory[instructionCounter++] = 3200 + op2Loc; // DIV
            else if (token == "*") memory[instructionCounter++] = 3300 + op2Loc; // MULT

            int tempLoc = dataCounter--;
            memory[instructionCounter++] = 2100 + tempLoc; // STORE
            tempStack.push(tempLoc);
        }
    }

    int resultLoc = tempStack.top();
    tempStack.pop();
    return resultLoc;
}

// ==========================================
// FASE 3: Primera Pasada del Compilador
// ==========================================
void firstPass(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        int simpleLine;
        std::string command;
        ss >> simpleLine >> command;

        insertSymbol(simpleLine, 'L', instructionCounter);

        if (command == "rem") {
            continue;
        } else if (command == "input") {
            char var;
            ss >> var;
            int loc = getVariableLocation(var);
            memory[instructionCounter++] = 1000 + loc; // READ
        } else if (command == "print") {
            char var;
            ss >> var;
            int loc = getVariableLocation(var);
            memory[instructionCounter++] = 1100 + loc; // WRITE
        } else if (command == "end") {
            memory[instructionCounter++] = 4300; // HALT
        } else if (command == "goto") {
            int targetLine;
            ss >> targetLine;
            TableEntry* entry = searchSymbol(targetLine, 'L');

            if (entry != nullptr) {
                memory[instructionCounter++] = 4000 + entry->location;
            } else {
                flags[instructionCounter] = targetLine;
                memory[instructionCounter++] = 4000;
            }
        } else if (command == "if") {
            std::string token1, relOp, token2, gotoKw;
            int targetLine;
            ss >> token1 >> relOp >> token2 >> gotoKw >> targetLine;

            int loc1 = isalpha(token1[0]) ? getVariableLocation(token1[0]) : getConstantLocation(std::stoi(token1));
            int loc2 = isalpha(token2[0]) ? getVariableLocation(token2[0]) : getConstantLocation(std::stoi(token2));

            // Cargar token1 y restar token2
            memory[instructionCounter++] = 2000 + loc1; // LOAD
            memory[instructionCounter++] = 3100 + loc2; // SUB

            TableEntry* entry = searchSymbol(targetLine, 'L');
            int targetLoc = (entry != nullptr) ? entry->location : 0;

            if (relOp == "==") {
                if (entry == nullptr) flags[instructionCounter] = targetLine;
                memory[instructionCounter++] = 4200 + targetLoc; // BRANCHZERO
            } else if (relOp == "<") {
                if (entry == nullptr) flags[instructionCounter] = targetLine;
                memory[instructionCounter++] = 4100 + targetLoc; // BRANCHNEG
            } else if (relOp == ">=") {
                // A >= B es lo mismo que NOT (A < B)
                // Si la resta no es negativa, salta
                if (entry == nullptr) flags[instructionCounter] = targetLine;
                memory[instructionCounter++] = 4200 + targetLoc; // BRANCHZERO
            }
        } else if (command == "let") {
            char targetVar, eq;
            ss >> targetVar >> eq;

            std::vector<std::string> exprTokens;
            std::string token;
            while (ss >> token) {
                exprTokens.push_back(token);
            }

            std::vector<std::string> postfix = infixToPostfix(exprTokens);
            int resultLoc = generatePostfixSML(postfix);

            int targetLoc = getVariableLocation(targetVar);
            memory[instructionCounter++] = 2000 + resultLoc; // LOAD
            memory[instructionCounter++] = 2100 + targetLoc; // STORE
        }
    }
    file.close();
}

// ==========================================
// FASE 4: Segunda Pasada y Salida
// ==========================================
void secondPass() {
    for (int i = 0; i < instructionCounter; ++i) {
        if (flags[i] != -1) {
            int targetLine = flags[i];
            TableEntry* entry = searchSymbol(targetLine, 'L');
            if (entry != nullptr) {
                memory[i] += entry->location;
            } else {
                std::cerr << "Error de compilación: Línea " << targetLine << " no encontrada." << std::endl;
            }
        }
    }
}

void writeSMLFile(const std::string& outputFile) {
    std::ofstream file(outputFile);
    for (int i = 0; i < 100; ++i) {
        if (memory[i] >= 0) {
            file << "+";
            if (memory[i] < 10) file << "000";
            else if (memory[i] < 100) file << "00";
            else if (memory[i] < 1000) file << "0";
        }
        file << memory[i] << std::endl;
    }
    file.close();
}

int main() {
    initFlags();

    std::string inputFile = "programa.simple";
    std::string outputFile = "codigo.sml";

    std::cout << "Iniciando compilacion de " << inputFile << "..." << std::endl;
    firstPass(inputFile);
    secondPass();
    writeSMLFile(outputFile);
    std::cout << "Compilacion completada con exito. Archivo guardado como " << outputFile << std::endl;

    return 0;
}