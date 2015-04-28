#include <QApplication>
#include "Filalyzer.hpp"

int main(int argc, char** argv)
{
  QApplication app (argc, argv);
  Filalyzer filalyzer;
  filalyzer.show ();
  return app.exec ();
}