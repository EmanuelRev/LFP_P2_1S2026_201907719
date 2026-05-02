# MedLexer - Analizador Léxico y Sintactico

**MedLexer** es una herramienta de software desarrollada en **C++** y el framework **Qt** diseñada para el procesamiento y análisis de registros médicos. El sistema utiliza un Autómata Finito Determinista (AFD) para transformar archivos de entrada con extensión `.med` en información estructurada, generando reportes estadísticos en HTML y diagramas de relaciones en Graphviz.

##  Requisitos y Dependencias

Para compilar y ejecutar este proyecto, asegúrese de contar con:

* **Qt Framework:** Versión 6.x o superior.
* **Compilador:** MinGW 64-bit (recomendado) o MSVC 2019+.
* **IDE:** Qt Creator.
* **Visualizador de Grafos:** [Graphviz](https://graphviz.org/) (para abrir los archivos .dot).

##  Instrucciones de Compilación

1.  Clone o descargue el repositorio con el código fuente.
2.  Abra el archivo de proyecto `MedLexer.pro` en **Qt Creator**.
3.  Seleccione el Kit de compilación (ej. *Desktop Qt 6.5.0 MinGW 64-bit*).
4.  Ejecute el proceso de limpieza y construcción (*Clean* y *Build*).
5.  Presione el botón **Run** (ícono verde) o la combinación de teclas `Ctrl + R`.

##  Guía de Uso

1.  **Cargar Archivo:** Haga clic en el botón "Cargar Archivo" y seleccione un documento `.task`.
2.  **Analizar:** Presione el botón "Analizar" para iniciar el motor de procesamiento léxico.
3.  **Visualizar:** Observe la tabla de tokens para ver los datos válidos y la tabla de errores para identificar posibles fallos en el archivo de entrada.
4.  **Reportes:** Haga clic en "Generar Reportes" para exportar los 5 archivos HTML y el archivo DOT de Graphviz de forma automática.

##  Formato del Archivo .med

El sistema procesa la estructura jerárquica definida por el hospital. Ejemplo:

```text
HOSPITAL {
    PACIENTES {
        paciente: "Nombre Apellido" [edad: 25, tipo_sangre: "O+", habitacion: 101],
    };
    MEDICOS {
        medico: "Dr. Ejemplo" [especialidad: CARDIOLOGIA, codigo: "MED-001"],
    };
    CITAS {
        cita: "Nombre Apellido" con "Dr. Ejemplo" [fecha: 2026-04-10, hora: 08:30],
    };
    DIAGNOSTICOS {
        diagnostico: "Nombre Apellido" [condicion: "Estable", medicamento: "Ninguno", dosis: DIARIA],
    };
};