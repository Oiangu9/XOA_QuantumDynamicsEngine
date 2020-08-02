#include "xoa_schrodingersimulator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // We instanciate the main window of the app
    XOA_SchrodingerSimulator mainWindow;
    mainWindow.show();
    // We execute the Qt app for waiting to user input
    return app.exec();
}
