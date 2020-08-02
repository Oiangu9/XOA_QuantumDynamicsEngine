#include "xoa_schrodingersimulator.h"
#include "ui_xoa_schrodingersimulator.h"


XOA_SchrodingerSimulator::XOA_SchrodingerSimulator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XOA_SchrodingerSimulator)
{
    ui->setupUi(this);

    // The cleanup of the TDSE folder is performed

    QString wd = QDir::currentPath()+"/TDSE_Simulator";
    QString command = "cd "+wd +"; bash "+ wd +"/CLEAN_Clear_trash_Recompile_Necessities.sh";
    QProcess process;
    process.startDetached("/bin/bash", QStringList()<<"-c" << command );
    process.waitForFinished();

    //We fill the items of the comboBox to allow the selection of the simulation
    for(int i=1; i<9; ++i){
        ui->simulationSelection->addItem(QString::number(i));
    }

    connect(ui->goSimulation, SIGNAL (clicked()), this, SLOT (goSimulationPressed()));
}

XOA_SchrodingerSimulator::~XOA_SchrodingerSimulator()
{
    delete ui;
}


void XOA_SchrodingerSimulator::goSimulationPressed()
{
    if(ui->simulationSelection->currentText().toInt()==1){
        delete option1Window;
        option1Window = new Option1();
        option1Window->show();
    }
}
