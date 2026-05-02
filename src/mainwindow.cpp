#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "reportgenerator.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColor>
#include <QBrush>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tablaTokens->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tablaErrores->setColumnCount(7);
    QStringList cabecerasErrores = {"No.", "Lexema", "Tipo", "Descripción", "Línea", "Col", "Gravedad"};
    ui->tablaErrores->setHorizontalHeaderLabels(cabecerasErrores);
    ui->tablaErrores->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::limpiarTablas() {
    ui->tablaTokens->setRowCount(0);
    ui->tablaErrores->setRowCount(0);
}

void MainWindow::on_btnCargar_clicked()
{
    QString nombreArchivo = QFileDialog::getOpenFileName(this, "Abrir archivo TaskScript", "", "Archivos Task (*.task);;Todos los archivos (*.*)");

    if (nombreArchivo.isEmpty()) {
        return;
    }

    QFile archivo(nombreArchivo);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
        return;
    }

    QTextStream entrada(&archivo);
    QString contenido = entrada.readAll();
    archivo.close();

    ui->txtCodigo->setPlainText(contenido);
    limpiarTablas();
}

void MainWindow::on_btnAnalizar_clicked()
{
    limpiarTablas();
    QString codigoFuente = ui->txtCodigo->toPlainText();

    if(codigoFuente.trimmed().isEmpty()){
        QMessageBox::warning(this, "Advertencia", "No hay código para analizar.");
        return;
    }

    ErrorManager* gestor = new ErrorManager();

    LexicalAnalyzer lexer(codigoFuente.toStdString(), gestor);
    lexer.analizar();
    std::vector<Token> tokens = lexer.getTokens();

    ui->tablaTokens->setRowCount(tokens.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        ui->tablaTokens->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        ui->tablaTokens->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(tokens[i].getLexema())));
        ui->tablaTokens->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(tokens[i].getTipoString())));
        ui->tablaTokens->setItem(i, 3, new QTableWidgetItem(QString::number(tokens[i].getLinea())));
        ui->tablaTokens->setItem(i, 4, new QTableWidgetItem(QString::number(tokens[i].getColumna())));
    }

    SyntaxAnalyzer parser(tokens, gestor);
    parser.analizar();

    if (gestor->hayErrores()) {
        std::vector<ErrorItem> errores = gestor->getErrores();
        ui->tablaErrores->setRowCount(errores.size());

        for (size_t i = 0; i < errores.size(); i++) {
            ui->tablaErrores->setItem(i, 0, new QTableWidgetItem(QString::number(errores[i].numero)));
            ui->tablaErrores->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(errores[i].lexema)));
            ui->tablaErrores->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(errores[i].tipoError)));
            ui->tablaErrores->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(errores[i].descripcion)));
            ui->tablaErrores->setItem(i, 4, new QTableWidgetItem(QString::number(errores[i].linea)));
            ui->tablaErrores->setItem(i, 5, new QTableWidgetItem(QString::number(errores[i].columna)));

            QTableWidgetItem* itemGravedad = new QTableWidgetItem(QString::fromStdString(errores[i].gravedad));
            itemGravedad->setForeground(QBrush(QColor(178, 34, 34)));
            ui->tablaErrores->setItem(i, 6, itemGravedad);
        }

        QMessageBox::warning(this, "Análisis Finalizado", "Se encontraron errores. Revisa la tabla.");
    } else {
        QMessageBox::information(this, "Análisis Finalizado", "El código es correcto. ¡Cero errores! Generando reportes HTML...");

        ReportGenerator generador(tokens);
        generador.generarReporteKanban();
        generador.generarReporteCarga();
    }

    ReportGenerator generadorTablas(tokens);
    generadorTablas.generarReporteTokensErrores(tokens, gestor->getErrores());

    delete gestor;
}