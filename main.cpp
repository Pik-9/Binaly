#include <QtGui/QApplication>
#include "Filalyzer.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Filalyzer filalyzer;
    filalyzer.show();
    return app.exec();
}
