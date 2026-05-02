#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include <vector>
#include <string>
#include <fstream>
#include "token.h"
#include "errormanager.h"

class SyntaxAnalyzer {
private:
    std::vector<Token> listaTokens;
    int indiceActual;
    Token tokenActual;
    ErrorManager* gestorErrores;
    bool huboErrorFatal;

    int contadorNodos;
    std::string dotNodos;
    std::string dotAristas;

    void avanzar();
    int match(TokenType tipoEsperado);

    std::string escapar(std::string str);
    int crearNodo(std::string etiqueta);
    int crearNodoHoja(std::string etiqueta);
    void agregarArista(int padre, int hijo);
    void generarArchivoDOT();

    int analizarPrograma();
    int analizarColumnas();
    int analizarColumna();
    int analizarTareas();
    int analizarTarea();
    int analizarAtributos();
    int analizarAtributo();
    int analizarPrioridad();

public:
    SyntaxAnalyzer(std::vector<Token> tokens, ErrorManager* gestor);
    void analizar();
};

#endif