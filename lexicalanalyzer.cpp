#include "lexicalanalyzer.h"
#include <iostream>

LexicalAnalyzer::LexicalAnalyzer(std::string codigo, ErrorManager* gestor) {
    this->codigoFuente = codigo;
    this->gestorErrores = gestor;
    this->posicion = 0;
    this->linea = 1;
    this->columna = 1;
}

bool LexicalAnalyzer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool LexicalAnalyzer::isDigit(char c) {
    return (c >= '0' && c <= '9');
}

std::vector<Token> LexicalAnalyzer::getTokens() const {
    return listaTokens;
}

void LexicalAnalyzer::analizar() {
    listaTokens.clear();
    Token t = nextToken();

    while (t.getTipo() != TokenType::TK_EOF) {
        if (t.getTipo() != TokenType::TK_ERROR) {
            listaTokens.push_back(t);
        }
        t = nextToken();
    }

    listaTokens.push_back(t);
}

Token LexicalAnalyzer::nextToken() {
    int length = codigoFuente.length();

    while (posicion < length) {
        char c = codigoFuente[posicion];

        if (c == ' ' || c == '\t' || c == '\r') {
            posicion++;
            columna++;
            continue;
        } else if (c == '\n') {
            posicion++;
            linea++;
            columna = 1;
            continue;
        }

        int colInicial = columna;

        if (c == '{') { posicion++; columna++; return Token(TokenType::TK_LLAVE_IZQ, "{", linea, colInicial); }
        if (c == '}') { posicion++; columna++; return Token(TokenType::TK_LLAVE_DER, "}", linea, colInicial); }
        if (c == '[') { posicion++; columna++; return Token(TokenType::TK_CORCHETE_IZQ, "[", linea, colInicial); }
        if (c == ']') { posicion++; columna++; return Token(TokenType::TK_CORCHETE_DER, "]", linea, colInicial); }
        if (c == ':') { posicion++; columna++; return Token(TokenType::TK_DOS_PUNTOS, ":", linea, colInicial); }
        if (c == ',') { posicion++; columna++; return Token(TokenType::TK_COMA, ",", linea, colInicial); }
        if (c == ';') { posicion++; columna++; return Token(TokenType::TK_PUNTO_Y_COMA, ";", linea, colInicial); }

        if (c == '"') {
            std::string lexema = "\"";
            posicion++; columna++;

            while (posicion < length && codigoFuente[posicion] != '"' && codigoFuente[posicion] != '\n') {
                lexema += codigoFuente[posicion];
                posicion++; columna++;
            }

            if (posicion < length && codigoFuente[posicion] == '"') {
                lexema += '"';
                posicion++; columna++;
                return Token(TokenType::TK_CADENA, lexema, linea, colInicial);
            } else {
                gestorErrores->agregarError(lexema, "Léxico", "Cadena no cerrada antes del fin de línea", linea, colInicial, "CRÍTICO");
                return Token(TokenType::TK_ERROR, lexema, linea, colInicial);
            }
        }

        if (isAlpha(c)) {
            std::string lexema = "";
            while (posicion < length && isAlpha(codigoFuente[posicion])) {
                lexema += codigoFuente[posicion];
                posicion++; columna++;
            }

            if (lexema == "TABLERO") return Token(TokenType::TK_TABLERO, lexema, linea, colInicial);
            if (lexema == "COLUMNA") return Token(TokenType::TK_COLUMNA, lexema, linea, colInicial);
            if (lexema == "tarea") return Token(TokenType::TK_TAREA, lexema, linea, colInicial);
            if (lexema == "prioridad") return Token(TokenType::TK_PRIORIDAD, lexema, linea, colInicial);
            if (lexema == "responsable") return Token(TokenType::TK_RESPONSABLE, lexema, linea, colInicial);
            if (lexema == "fecha_limite") return Token(TokenType::TK_FECHA_LIMITE, lexema, linea, colInicial);
            if (lexema == "ALTA") return Token(TokenType::TK_ALTA, lexema, linea, colInicial);
            if (lexema == "MEDIA") return Token(TokenType::TK_MEDIA, lexema, linea, colInicial);
            if (lexema == "BAJA") return Token(TokenType::TK_BAJA, lexema, linea, colInicial);

            gestorErrores->agregarError(lexema, "Léxico", "Palabra no reconocida en el lenguaje", linea, colInicial, "ERROR");
            return Token(TokenType::TK_ERROR, lexema, linea, colInicial);
        }

        if (isDigit(c)) {
            std::string lexema = "";
            while (posicion < length && isDigit(codigoFuente[posicion])) {
                lexema += codigoFuente[posicion];
                posicion++; columna++;
            }

            if (lexema.length() == 4 && posicion < length && codigoFuente[posicion] == '-') {
                std::string posibleFecha = lexema;
                int posTemp = posicion;
                int colTemp = columna;

                posibleFecha += "-";
                posTemp++; colTemp++;

                int digitosMes = 0;
                while (posTemp < length && isDigit(codigoFuente[posTemp]) && digitosMes < 2) {
                    posibleFecha += codigoFuente[posTemp];
                    posTemp++; colTemp++; digitosMes++;
                }

                if (digitosMes == 2 && posTemp < length && codigoFuente[posTemp] == '-') {
                    posibleFecha += "-";
                    posTemp++; colTemp++;

                    int digitosDia = 0;
                    while (posTemp < length && isDigit(codigoFuente[posTemp]) && digitosDia < 2) {
                        posibleFecha += codigoFuente[posTemp];
                        posTemp++; colTemp++; digitosDia++;
                    }

                    if (digitosDia == 2) {
                        posicion = posTemp;
                        columna = colTemp;
                        return Token(TokenType::TK_FECHA, posibleFecha, linea, colInicial);
                    }
                }
            }
            return Token(TokenType::TK_ENTERO, lexema, linea, colInicial);
        }

        std::string lexemaError = std::string(1, c);
        gestorErrores->agregarError(lexemaError, "Léxico", "Carácter no reconocido por el lenguaje", linea, colInicial, "ERROR");
        posicion++; columna++;
        return Token(TokenType::TK_ERROR, lexemaError, linea, colInicial);
    }

    return Token(TokenType::TK_EOF, "EOF", linea, columna);
}