/******************************************************************************
 *  Filalyzer                                                                 *
 *                                                                            *
 *   (C) 2015 by Daniel Steinhauer <pik-9@users.sourceforge.net>              *
 *                                                                            *
 *                                                                            *
 *  This file is part of Filalyzer.                                           *
 *                                                                            *
 *  Filalyzer is free software: you can redistribute it and/or modify         *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  Filalyzer is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with Filalyzer.  If not, see <http://www.gnu.org/licenses/>.           *
 ******************************************************************************/

/**
 * \file main.cpp
 * \author Daniel Steinhauer
 */

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