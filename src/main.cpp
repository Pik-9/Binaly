#include <QApplication>
#include <QSettings>
#include <QTranslator>

#include "Filalyzer.hpp"

int main (int argc, char *argv[])
{
  QApplication app (argc, argv);
  QSettings *appSettings = new QSettings ("Pik-9", "Hexalizer");
  QTranslator *appTrans = new QTranslator ();
  
  appSettings->beginGroup ("Language");
  QString ccode = appSettings->value ("Countrycode", "en").toString ();
  appSettings->endGroup ();
  appTrans->load (QString ("Hexalizer_%1").arg (ccode));
  app.installTranslator (appTrans);
  
  Filalyzer *filalyzer = new Filalyzer (appSettings);
  filalyzer->show ();
  return app.exec ();
}