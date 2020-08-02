#ifndef OPTION1_H
#define OPTION1_H
#include <QDir>
#include <QProcess>
#include <iostream>
#include <fstream>
#include <QMainWindow>

namespace Ui {
class Option1;
}

class Option1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Option1(QWidget *parent = nullptr);
    ~Option1();

private slots:
    void on_runSimulationAndPlot_clicked();

    void on_abortSimulation_clicked();

private:
    Ui::Option1 *ui;
    QProcess process;
    QString wd, command;
    void runSimulation();
    void plot();
};

#endif // OPTION1_H
