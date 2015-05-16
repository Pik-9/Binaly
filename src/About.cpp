/******************************************************************************
 *  Binaly                                                                    *
 *                                                                            *
 *   (C) 2015 by Daniel Steinhauer <pik-9@users.sourceforge.net>              *
 *                                                                            *
 *                                                                            *
 *  This file is part of Binaly.                                              *
 *                                                                            *
 *  Binaly is free software: you can redistribute it and/or modify            *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  Binaly is distributed in the hope that it will be useful,                 *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with Binaly.  If not, see <http://www.gnu.org/licenses/>.           *
 ******************************************************************************/

/**
 * \file About.cpp
 * \author Daniel Steinhauer
 */

#include "About.hpp"

#include <QApplication>
#include <QDesktopWidget>

AboutDialog::AboutDialog (QWidget* parent)
  : QWidget (parent)
{
  resize (700, 200);
  setWindowTitle (tr ("About Binaly"));
  setWindowIcon (QIcon ("/usr/local/share/Binaly/Binaly.png"));
  
  lyt = new QGridLayout (this);
  g_desc = new QGroupBox ();
  g_desc->setFlat (true);
  lyt_desc = new QHBoxLayout (g_desc);
  l_icon = new QLabel ();
  l_icon->setPixmap (QPixmap ("/usr/local/share/Binaly/Binaly.png"));
  l_desc = new QLabel (tr (
    "Binaly is a binary/hex editor which shows the whole file on a colorful strip.\n"
    "It subdivides the file into blocks of 1 Kib (1024 Bytes) and analyses them.\n"
    "It knows the following kinds of data sections:\n"
    "- Random binary data:      The byte's average value is in [108, 148] and the"
    " standard deviation is in [60, 68].\n"
    "- Text data:               The average value is > 64 and the std. dev. < 40.\n"
    "- Homogeneous data:        The std. dev. is < 2.\n"
    "- Other data section:      The block doesn't fit into any of the categories above.\n"
    "\nThis is very useful to get a quick overview over the file and its content.\n\n"
    "(C) 2015 by Daniel Steinhauer"
  ));
  btn_ok = new QPushButton (tr ("OK"));
  
  lyt_desc->addWidget (l_icon);
  lyt_desc->addWidget (l_desc);
  
  lyt->addWidget (g_desc, 0, 0, 1, 3);
  lyt->addWidget (btn_ok, 1, 1, 1, 1);
  
  QRect winrect = geometry ();
  winrect.moveCenter (QApplication::desktop ()->availableGeometry ().center ());
  setGeometry (winrect);
  
  connect (btn_ok, SIGNAL (clicked ()), this, SLOT (hide ()));
}

AboutDialog::~AboutDialog ()
{}