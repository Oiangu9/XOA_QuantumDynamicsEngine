#ifndef XOA_SCHRODINGERSIMULATOR_H
#define XOA_SCHRODINGERSIMULATOR_H

#include <QMainWindow>
#include <QProcess>
#include "option1.h"

QT_BEGIN_NAMESPACE
namespace Ui { class XOA_SchrodingerSimulator; }
QT_END_NAMESPACE

class XOA_SchrodingerSimulator : public QMainWindow
{
    Q_OBJECT

public:
    XOA_SchrodingerSimulator(QWidget *parent = nullptr);
    ~XOA_SchrodingerSimulator();

private slots:
    void goSimulationPressed();

private:
    Ui::XOA_SchrodingerSimulator *ui;
    Option1* option1Window;
};
#endif // XOA_SCHRODINGERSIMULATOR_H
