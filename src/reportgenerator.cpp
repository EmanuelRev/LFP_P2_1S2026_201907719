#include "reportgenerator.h"
#include <iostream>

ReportGenerator::ReportGenerator(std::vector<Token> tokens) {
    extraerDatos(tokens);
}

void ReportGenerator::extraerDatos(std::vector<Token> tokens) {
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i].getTipo() == TokenType::TK_TABLERO) {
            nombreTablero = tokens[i + 1].getLexema();
            nombreTablero = nombreTablero.substr(1, nombreTablero.length() - 2);
        }
        else if (tokens[i].getTipo() == TokenType::TK_COLUMNA) {
            ColumnaData nuevaColumna;
            std::string nombreCol = tokens[i + 1].getLexema();
            nuevaColumna.nombre = nombreCol.substr(1, nombreCol.length() - 2);
            columnas.push_back(nuevaColumna);
        }
        else if (tokens[i].getTipo() == TokenType::TK_TAREA) {
            TareaData nuevaTarea;
            std::string nombreTar = tokens[i + 2].getLexema();
            nuevaTarea.nombre = nombreTar.substr(1, nombreTar.length() - 2);

            while (tokens[i].getTipo() != TokenType::TK_CORCHETE_DER) {
                if (tokens[i].getTipo() == TokenType::TK_PRIORIDAD) {
                    nuevaTarea.prioridad = tokens[i + 2].getLexema();
                } else if (tokens[i].getTipo() == TokenType::TK_RESPONSABLE) {
                    std::string resp = tokens[i + 2].getLexema();
                    nuevaTarea.responsable = resp.substr(1, resp.length() - 2);
                } else if (tokens[i].getTipo() == TokenType::TK_FECHA_LIMITE) {
                    nuevaTarea.fecha = tokens[i + 2].getLexema();
                }
                i++;
            }

            if (!columnas.empty()) {
                columnas.back().tareas.push_back(nuevaTarea);
            }
        }
    }
}

void ReportGenerator::generarReporteKanban() {
    std::ofstream archivo("Reporte_Kanban.html");

    archivo << "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Tablero Kanban</title>";
    archivo << "<style>";
    archivo << "body { font-family: Arial, sans-serif; background-color: #0079bf; color: #333; margin: 0; padding: 20px; }";
    archivo << "h1 { color: white; }";
    archivo << ".tablero { display: flex; gap: 20px; align-items: flex-start; overflow-x: auto; }";
    archivo << ".columna { background-color: #ebecf0; border-radius: 8px; width: 300px; padding: 10px; flex-shrink: 0; }";
    archivo << ".columna-header { font-weight: bold; margin-bottom: 10px; color: #172b4d; }";
    archivo << ".tarea { background-color: white; border-radius: 6px; padding: 10px; margin-bottom: 10px; box-shadow: 0 1px 2px rgba(0,0,0,0.1); }";
    archivo << ".tarea-titulo { font-weight: bold; margin-bottom: 8px; }";
    archivo << ".badge { padding: 4px 8px; border-radius: 4px; font-size: 12px; color: white; font-weight: bold; display: inline-block; margin-bottom: 8px; }";
    archivo << ".badge.ALTA { background-color: #eb5a46; }";
    archivo << ".badge.MEDIA { background-color: #f2d600; color: #333; }";
    archivo << ".badge.BAJA { background-color: #61bd4f; }";
    archivo << ".info { font-size: 12px; color: #5e6c84; margin-top: 4px; }";
    archivo << ".responsable { display: inline-block; background-color: #dfe1e6; border-radius: 12px; padding: 4px 10px; font-size: 12px; font-weight: bold; margin-top: 8px; }";
    archivo << "</style></head><body>";

    archivo << "<h1>" << nombreTablero << "</h1>";
    archivo << "<div class='tablero'>";

    for (const ColumnaData& col : columnas) {
        archivo << "<div class='columna'>";
        archivo << "<div class='columna-header'>" << col.nombre << " (" << col.tareas.size() << ")</div>";

        for (const TareaData& tar : col.tareas) {
            archivo << "<div class='tarea'>";
            archivo << "<div class='tarea-titulo'>" << tar.nombre << "</div>";
            archivo << "<span class='badge " << tar.prioridad << "'>" << tar.prioridad << "</span>";
            archivo << "<div class='info'>Fecha límite: " << tar.fecha << "</div>";
            archivo << "<div class='responsable'>" << tar.responsable << "</div>";
            archivo << "</div>";
        }

        archivo << "</div>";
    }

    archivo << "</div></body></html>";
    archivo.close();
}

void ReportGenerator::generarReporteCarga() {
    std::ofstream archivo("Reporte_Carga.html");

    std::map<std::string, std::map<std::string, int>> cargaResponsables;
    std::map<std::string, int> totalTareasResponsable;
    int totalTareasTablero = 0;

    for (const ColumnaData& col : columnas) {
        for (const TareaData& tar : col.tareas) {
            cargaResponsables[tar.responsable][tar.prioridad]++;
            totalTareasResponsable[tar.responsable]++;
            totalTareasTablero++;
        }
    }

    archivo << "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Carga por Responsable</title>";
    archivo << "<style>";
    archivo << "body { font-family: Arial, sans-serif; background-color: #f4f5f7; padding: 20px; }";
    archivo << "table { width: 100%; border-collapse: collapse; background: white; box-shadow: 0 1px 3px rgba(0,0,0,0.2); }";
    archivo << "th, td { padding: 15px; text-align: left; border-bottom: 1px solid #ddd; }";
    archivo << "th { background-color: #0079bf; color: white; }";
    archivo << ".barra-fondo { background-color: #ebecf0; border-radius: 4px; width: 100%; height: 20px; }";
    archivo << ".barra-relleno { background-color: #0079bf; height: 100%; border-radius: 4px; color: white; text-align: center; font-size: 12px; line-height: 20px; }";
    archivo << "</style></head><body>";

    archivo << "<h2>Carga por Responsable</h2>";
    archivo << "<table><tr><th>Responsable</th><th>Tareas Asignadas</th><th>Alta</th><th>Media</th><th>Baja</th><th>Distribución</th></tr>";

    for (const auto& par : totalTareasResponsable) {
        std::string resp = par.first;
        int total = par.second;
        int alta = cargaResponsables[resp]["ALTA"];
        int media = cargaResponsables[resp]["MEDIA"];
        int baja = cargaResponsables[resp]["BAJA"];

        int porcentaje = totalTareasTablero > 0 ? (total * 100) / totalTareasTablero : 0;

        archivo << "<tr>";
        archivo << "<td>" << resp << "</td>";
        archivo << "<td>" << total << "</td>";
        archivo << "<td>" << alta << "</td>";
        archivo << "<td>" << media << "</td>";
        archivo << "<td>" << baja << "</td>";
        archivo << "<td><div class='barra-fondo'><div class='barra-relleno' style='width:" << porcentaje << "%'>" << porcentaje << "%</div></div></td>";
        archivo << "</tr>";
    }

    archivo << "</table></body></html>";
    archivo.close();
}

void ReportGenerator::generarReporteTokensErrores(std::vector<Token> tokens, std::vector<ErrorItem> errores) {
    std::ofstream archivo("Reporte_Tokens_Errores.html");

    archivo << "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Tablas de Análisis</title>";
    archivo << "<style>body{font-family:Arial;} table{width:100%; border-collapse:collapse; margin-bottom:30px;} th,td{border:1px solid black; padding:8px;} th{background-color:#f2f2f2;}</style>";
    archivo << "</head><body>";

    archivo << "<h2>Tabla de Errores (" << errores.size() << ")</h2>";
    archivo << "<table><tr><th>No.</th><th>Lexema</th><th>Tipo</th><th>Descripción</th><th>Línea</th><th>Columna</th><th>Gravedad</th></tr>";
    for (const ErrorItem& e : errores) {
        archivo << "<tr><td>" << e.numero << "</td><td>" << e.lexema << "</td><td>" << e.tipoError << "</td><td>" << e.descripcion << "</td><td>" << e.linea << "</td><td>" << e.columna << "</td><td style='color:#b22222; font-weight:bold;'>" << e.gravedad << "</td></tr>";
    }
    archivo << "</table>";

    archivo << "<h2>Tabla de Tokens</h2>";
    archivo << "<table><tr><th>No.</th><th>Lexema</th><th>Tipo</th><th>Línea</th><th>Columna</th></tr>";
    for (size_t i = 0; i < tokens.size(); i++) {
        archivo << "<tr><td>" << (i+1) << "</td><td>" << tokens[i].getLexema() << "</td><td>" << tokens[i].getTipoString() << "</td><td>" << tokens[i].getLinea() << "</td><td>" << tokens[i].getColumna() << "</td></tr>";
    }
    archivo << "</table></body></html>";
    archivo.close();
}