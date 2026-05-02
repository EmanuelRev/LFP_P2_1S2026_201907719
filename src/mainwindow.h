#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "errormanager.h"
#include "lexicalanalyzer.h"
#include "syntaxanalyzer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnCargar_clicked();
    void on_btnAnalizar_clicked();

private:
    Ui::MainWindow *ui;

    // Funciones auxiliares para limpiar las tablas
    void limpiarTablas();
};

#endif // MAINWINDOW_H