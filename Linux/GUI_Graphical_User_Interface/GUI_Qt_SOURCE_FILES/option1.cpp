#include "option1.h"
#include "ui_option1.h"

using namespace std;

Option1::Option1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Option1)
{
    ui->setupUi(this);

    connect(ui->runSimulationAndPlot, SIGNAL (clicked()), this, SLOT (on_runSimulationAndPlot_clicked()));
    connect(ui->abortSimulation, SIGNAL (clicked()), this, SLOT (on_abortSimulation_clicked()));

    wd = QDir::currentPath()+"/TDSE_Simulator";
    command = "cd "+wd +"; bash MAIN_Simulator.sh < ARGS_parametersMenu.txt";

}

Option1::~Option1()
{
    delete ui;
}

void Option1::on_runSimulationAndPlot_clicked()
{
    ofstream argumentFile;
    argumentFile.open(wd.toStdString() + "/ARGS_parametersMenu.txt");
    argumentFile << 1 << endl << ui->psiIni->displayText().toStdString()<< endl << ui->potential->displayText().toStdString()<<endl;
    argumentFile << ui->nx->displayText().toStdString() << " " << ui->xmin->displayText().toStdString() << " " << ui->xmax->displayText().toStdString()<< endl;
    argumentFile << ui->tIt->displayText().toStdString() << endl << ui->dt->displayText().toStdString() << endl;

    if(ui->N->isChecked()){
        argumentFile << "N"<<endl;
    }else if(ui->R->isChecked()){
        argumentFile << "R" << endl << ui->numTrajs->displayText().toStdString()<<endl;
    }else{
        argumentFile << "C" << endl << ui->numTrajs->displayText().toStdString()<<endl << ui->customTrajs->displayText().toStdString()<<endl;
    }
    argumentFile.close();

    process.start("/bin/bash", QStringList()<<"-c" << command );
}

void Option1::runSimulation(){

}

void Option1::plot(){

}

void Option1::on_abortSimulation_clicked()
{
    process.kill();
    process.terminate();
}
