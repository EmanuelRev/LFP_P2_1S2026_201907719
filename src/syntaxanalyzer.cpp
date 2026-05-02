#include "syntaxanalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(std::vector<Token> tokens, ErrorManager* gestor)
    : tokenActual(TokenType::TK_ERROR, "", 0, 0)
{
    this->listaTokens = tokens;
    this->gestorErrores = gestor;
    this->indiceActual = 0;
    this->huboErrorFatal = false;
    this->contadorNodos = 0;
    this->dotNodos = "";
    this->dotAristas = "";

    if (!listaTokens.empty()) {
        tokenActual = listaTokens[0];
    }
}

void SyntaxAnalyzer::avanzar() {
    if (indiceActual < listaTokens.size() - 1) {
        indiceActual++;
        tokenActual = listaTokens[indiceActual];
    }
}

std::string SyntaxAnalyzer::escapar(std::string str) {
    std::string res = "";
    for (char c : str) {
        if (c == '"') res += "\\\"";
        else res += c;
    }
    return res;
}

int SyntaxAnalyzer::crearNodo(std::string etiqueta) {
    int id = contadorNodos++;
    dotNodos += "n" + std::to_string(id) + " [label=\"" + escapar(etiqueta) + "\", fillcolor=\"#2E75B6\", fontcolor=white];\n";
    return id;
}

int SyntaxAnalyzer::crearNodoHoja(std::string etiqueta) {
    int id = contadorNodos++;
    dotNodos += "n" + std::to_string(id) + " [label=\"" + escapar(etiqueta) + "\", fillcolor=\"#D6EAF8\"];\n";
    return id;
}

void SyntaxAnalyzer::agregarArista(int padre, int hijo) {
    if (padre != -1 && hijo != -1) {
        dotAristas += "n" + std::to_string(padre) + " -> n" + std::to_string(hijo) + ";\n";
    }
}

void SyntaxAnalyzer::generarArchivoDOT() {
    std::ofstream archivo("arbol.dot");
    if (archivo.is_open()) {
        archivo << "digraph ArbolDerivacion {\n";
        archivo << "rankdir=TB;\n";
        archivo << "node [shape=box, style=filled, fontname=\"Arial\"];\n";
        archivo << dotNodos;
        archivo << dotAristas;
        archivo << "}\n";
        archivo.close();
    }
}

int SyntaxAnalyzer::match(TokenType tipoEsperado) {
    if (huboErrorFatal) return -1;

    if (tokenActual.getTipo() == tipoEsperado) {
        int idHoja = crearNodoHoja(tokenActual.getLexema());
        avanzar();
        return idHoja;
    } else {
        huboErrorFatal = true;
        std::string desc = "Error sintáctico fatal. Se esperaba otro token pero se encontró '" + tokenActual.getLexema() + "'";
        gestorErrores->agregarError(tokenActual.getLexema(), "Sintáctico", desc, tokenActual.getLinea(), tokenActual.getColumna(), "ERROR");
        return -1;
    }
}

void SyntaxAnalyzer::analizar() {
    huboErrorFatal = false;
    contadorNodos = 0;
    dotNodos = "";
    dotAristas = "";

    analizarPrograma();

    if (!huboErrorFatal) {
        generarArchivoDOT();
    }
}

int SyntaxAnalyzer::analizarPrograma() {
    if (huboErrorFatal) return -1;
    int nodoActual = crearNodo("<programa>");

    agregarArista(nodoActual, match(TokenType::TK_TABLERO));
    agregarArista(nodoActual, match(TokenType::TK_CADENA));
    agregarArista(nodoActual, match(TokenType::TK_LLAVE_IZQ));
    agregarArista(nodoActual, analizarColumnas());
    agregarArista(nodoActual, match(TokenType::TK_LLAVE_DER));

    return nodoActual;
}

int SyntaxAnalyzer::analizarColumnas() {
    if (huboErrorFatal) return -1;
    int nodoActual = crearNodo("<columnas>");

    agregarArista(nodoActual, analizarColumna());

    if (!huboErrorFatal && tokenActual.getTipo() == TokenType::TK_COLUMNA) {
        agregarArista(nodoActual, analizarColumnas());
    }

    return nodoActual;
}

int SyntaxAnalyzer::analizarColumna() {
    if (huboErrorFatal) return -1;
    int nodoActual = crearNodo("<columna>");

    agregarArista(nodoActual, match(TokenType::TK_COLUMNA));
    agregarArista(nodoActual, match(TokenType::TK_CADENA));
    agregarArista(nodoActual, match(TokenType::TK_LLAVE_IZQ));

    if (!huboErrorFatal && tokenActual.getTipo() == TokenType::TK_TAREA) {
        agregarArista(nodoActual, analizarTareas());
    }

    agregarArista(nodoActual, match(TokenType::TK_LLAVE_DER));
    agregarArista(nodoActual, match(TokenType::TK_PUNTO_Y_COMA));

    return nodoActual;
}

int SyntaxAnalyzer::analizarTareas() {
    if (huboErrorFatal) return -1;
    int nodoActual = crearNodo("<tareas>");

    agregarArista(nodoActual, analizarTarea());

    if (!huboErrorFatal && tokenActual.getTipo() == TokenType::TK_COMA) {
        agregarArista(nodoActual, match(TokenType::TK_COMA));

        if (tokenActual.getTipo() == TokenType::TK_TAREA) {
            agregarArista(nodoActual, analizarTareas());
        } else if (tokenActual.getTipo() == TokenType::TK_LLAVE_DER) {
        } else {
            huboErrorFatal = true;
            gestorErrores->agregarError(tokenActual.getLexema(), "Sintáctico", "Se esperaba otra 'tarea' después de la coma", tokenActual.getLinea(), tokenActual.getColumna(), "ERROR");
        }
    }

    return nodoActual;
}

int SyntaxAnalyzer::analizarTarea() {
    if (huboErrorFatal) return -1;
    int nodoActual = crearNodo("<tarea>");

    agregarArista(nodoActual, match(TokenType::TK_TAREA));
    agregarArista(nodoActual, match(TokenType::TK_DOS_PUNTOS));
    agregarArista(nodoActual, match(TokenType::TK_CADENA));
    agregarArista(nodoActual, match(TokenType::TK_CORCHETE_IZQ));

    agregarArista(nodoActual, analizarAtributos());

    agregarArista(nodoActual, match(TokenType::TK_CORCHETE_DER));

    return nodoActual;
}

int SyntaxAnalyzer::analizarAtributos() {
    if (huboErrorFatal) return -1;
    int nodoActual = crearNodo("<atributos>");

    agregarArista(nodoActual, analizarAtributo());

    if (!huboErrorFatal && tokenActual.getTipo() == TokenType::TK_COMA) {
        agregarArista(nodoActual, match(TokenType::TK_COMA));

        if (tokenActual.getTipo() == TokenType::TK_PRIORIDAD || tokenActual.getTipo() == TokenType::TK_RESPONSABLE || tokenActual.getTipo() == TokenType::TK_FECHA_LIMITE) {
            agregarArista(nodoActual, analizarAtributos());
        } else if (tokenActual.getTipo() == TokenType::TK_CORCHETE_DER) {
        } else {
            huboErrorFatal = true;
            gestorErrores->agregarError(tokenActual.getLexema(), "Sintáctico", "Se esperaba otro atributo después de la coma", tokenActual.getLinea(), tokenActual.getColumna(), "ERROR");
        }
    }

    return nodoActual;
}

int SyntaxAnalyzer::analizarAtributo() {
    if (huboErrorFatal) return -1;
    int nodoActual = crearNodo("<atributo>");

    if (tokenActual.getTipo() == TokenType::TK_PRIORIDAD) {
        agregarArista(nodoActual, match(TokenType::TK_PRIORIDAD));
        agregarArista(nodoActual, match(TokenType::TK_DOS_PUNTOS));
        agregarArista(nodoActual, analizarPrioridad());
    }
    else if (tokenActual.getTipo() == TokenType::TK_RESPONSABLE) {
        agregarArista(nodoActual, match(TokenType::TK_RESPONSABLE));
        agregarArista(nodoActual, match(TokenType::TK_DOS_PUNTOS));
        agregarArista(nodoActual, match(TokenType::TK_CADENA));
    }
    else if (tokenActual.getTipo() == TokenType::TK_FECHA_LIMITE) {
        agregarArista(nodoActual, match(TokenType::TK_FECHA_LIMITE));
        agregarArista(nodoActual, match(TokenType::TK_DOS_PUNTOS));
        agregarArista(nodoActual, match(TokenType::TK_FECHA));
    }
    else {
        huboErrorFatal = true;
        gestorErrores->agregarError(tokenActual.getLexema(), "Sintáctico", "Atributo inválido", tokenActual.getLinea(), tokenActual.getColumna(), "ERROR");
        return -1;
    }
    return nodoActual;
}

int SyntaxAnalyzer::analizarPrioridad() {
    if (huboErrorFatal) return -1;
    int nodoActual = crearNodo("<prioridad>");

    if (tokenActual.getTipo() == TokenType::TK_ALTA) {
        agregarArista(nodoActual, match(TokenType::TK_ALTA));
    } else if (tokenActual.getTipo() == TokenType::TK_MEDIA) {
        agregarArista(nodoActual, match(TokenType::TK_MEDIA));
    } else if (tokenActual.getTipo() == TokenType::TK_BAJA) {
        agregarArista(nodoActual, match(TokenType::TK_BAJA));
    } else {
        huboErrorFatal = true;
        gestorErrores->agregarError(tokenActual.getLexema(), "Sintáctico", "Se esperaba ALTA, MEDIA o BAJA", tokenActual.getLinea(), tokenActual.getColumna(), "ERROR");
        return -1;
    }

    return nodoActual;
}