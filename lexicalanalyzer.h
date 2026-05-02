#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <string>
#include <vector>
#include "token.h"
#include "errormanager.h"

class LexicalAnalyzer {
private:
    std::string codigoFuente;
    int posicion;
    int linea;
    int columna;
    ErrorManager* gestorErrores;
    std::vector<Token> listaTokens;


    bool isAlpha(char c);
    bool isDigit(char c);

public:

    LexicalAnalyzer(std::string codigo, ErrorManager* gestor);


    Token nextToken();


    void analizar();


    std::vector<Token> getTokens() const;
};

#endif // LEXICALANALYZER_H