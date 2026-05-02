#include "errormanager.h"

ErrorManager::ErrorManager() {
    this->contadorErrores = 0;
}

void ErrorManager::agregarError(std::string lexema, std::string tipoError, std::string descripcion, int linea, int columna, std::string gravedad) {
    contadorErrores++;
    ErrorItem nuevoError;
    nuevoError.numero = contadorErrores;
    nuevoError.lexema = lexema;
    nuevoError.tipoError = tipoError;
    nuevoError.descripcion = descripcion;
    nuevoError.linea = linea;
    nuevoError.columna = columna;
    nuevoError.gravedad = gravedad;

    listaErrores.push_back(nuevoError);
}

std::vector<ErrorItem> ErrorManager::getErrores() const {
    return listaErrores;
}

bool ErrorManager::hayErrores() const {
    return !listaErrores.empty();
}

void ErrorManager::limpiarErrores() {
    listaErrores.clear();
    contadorErrores = 0;
}