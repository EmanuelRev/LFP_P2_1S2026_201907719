#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    TK_TABLERO,
    TK_COLUMNA,
    TK_TAREA,
    TK_PRIORIDAD,
    TK_RESPONSABLE,
    TK_FECHA_LIMITE,
    TK_ALTA,
    TK_MEDIA,
    TK_BAJA,
    TK_CADENA,
    TK_ENTERO,
    TK_FECHA,
    TK_LLAVE_IZQ,
    TK_LLAVE_DER,
    TK_CORCHETE_IZQ,
    TK_CORCHETE_DER,
    TK_DOS_PUNTOS,
    TK_COMA,
    TK_PUNTO_Y_COMA,
    TK_EOF,
    TK_ERROR
};

class Token {
private:
    TokenType tipo;
    std::string lexema;
    int linea;
    int columna;

public:
    Token(TokenType tipo, std::string lexema, int linea, int columna);

    TokenType getTipo() const;
    std::string getLexema() const;
    int getLinea() const;
    int getColumna() const;

    std::string getTipoString() const;
};

#endif
