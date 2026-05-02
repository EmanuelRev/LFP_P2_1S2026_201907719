#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "token.h"
#include "errormanager.h"


struct TareaData {
    std::string nombre;
    std::string prioridad;
    std::string responsable;
    std::string fecha;
};

struct ColumnaData {
    std::string nombre;
    std::vector<TareaData> tareas;
};

class ReportGenerator {
private:
    std::string nombreTablero;
    std::vector<ColumnaData> columnas;


    void extraerDatos(std::vector<Token> tokens);

public:

    ReportGenerator(std::vector<Token> tokens);


    void generarReporteKanban();
    void generarReporteCarga();
    void generarReporteTokensErrores(std::vector<Token> tokens, std::vector<ErrorItem> errores);
};

#endif // REPORTGENERATOR_H