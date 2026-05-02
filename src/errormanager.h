#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

#include <string>
#include <vector>

struct ErrorItem {
    int numero;
    std::string lexema;
    std::string tipoError;
    std::string descripcion;
    int linea;
    int columna;
    std::string gravedad;
};

class ErrorManager {
private:
    std::vector<ErrorItem> listaErrores;
    int contadorErrores;

public:
    ErrorManager();

    void agregarError(std::string lexema, std::string tipoError, std::string descripcion, int linea, int columna, std::string gravedad);
    std::vector<ErrorItem> getErrores() const;
    bool hayErrores() const;
    void limpiarErrores();
};

#endif