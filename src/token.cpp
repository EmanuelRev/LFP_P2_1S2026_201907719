#include "token.h"

Token::Token(TokenType tipo, std::string lexema, int linea, int columna) {
    this->tipo = tipo;
    this->lexema = lexema;
    this->linea = linea;
    this->columna = columna;
}

TokenType Token::getTipo() const { return tipo; }
std::string Token::getLexema() const { return lexema; }
int Token::getLinea() const { return linea; }
int Token::getColumna() const { return columna; }

std::string Token::getTipoString() const {
    switch (tipo) {
    case TokenType::TK_TABLERO: return "TABLERO";
    case TokenType::TK_COLUMNA: return "COLUMNA";
    case TokenType::TK_TAREA: return "TAREA";
    case TokenType::TK_PRIORIDAD: return "PRIORIDAD";
    case TokenType::TK_RESPONSABLE: return "RESPONSABLE";
    case TokenType::TK_FECHA_LIMITE: return "FECHA_LIMITE";
    case TokenType::TK_ALTA: return "ALTA";
    case TokenType::TK_MEDIA: return "MEDIA";
    case TokenType::TK_BAJA: return "BAJA";
    case TokenType::TK_CADENA: return "CADENA";
    case TokenType::TK_ENTERO: return "ENTERO";
    case TokenType::TK_FECHA: return "FECHA";
    case TokenType::TK_LLAVE_IZQ: return "{";
    case TokenType::TK_LLAVE_DER: return "}";
    case TokenType::TK_CORCHETE_IZQ: return "[";
    case TokenType::TK_CORCHETE_DER: return "]";
    case TokenType::TK_DOS_PUNTOS: return ":";
    case TokenType::TK_COMA: return ",";
    case TokenType::TK_PUNTO_Y_COMA: return ";";
    case TokenType::TK_EOF: return "EOF";
    case TokenType::TK_ERROR: return "ERROR";
    default: return "DESCONOCIDO";
    }
}