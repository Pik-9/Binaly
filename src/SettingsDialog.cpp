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
 * \file SettingsDialog.cpp
 * \author Daniel Steinhauer
 */

#include "SettingsDialog.hpp"

#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>

ColorButton::ColorButton (QColor color, QWidget* parent)
  : QPushButton (parent), chosenColor (color)
{
  dialog = new QColorDialog (chosenColor, parent);
  connect (this, SIGNAL (clicked ()), dialog, SLOT (show ()));
  connect (dialog, SIGNAL (colorSelected (QColor)), this, SLOT (setColor (QColor)));
}

ColorButton::ColorButton (const ColorButton& other)
  : QPushButton (other.parentWidget ()), chosenColor (other.chosenColor)
{
  dialog = new QColorDialog (other.chosenColor, other.parentWidget ());
  connect (this, SIGNAL (clicked ()), dialog, SLOT (show ()));
  connect (dialog, SIGNAL (colorSelected (QColor)), this, SLOT (setColor (QColor)));
}

ColorButton::~ColorButton ()
{
  delete dialog;
}

QColor ColorButton::getColor ()
{
  return chosenColor;
}

void ColorButton::paintEvent (QPaintEvent* event)
{
  QPushButton::paintEvent (event);
  QRect colorBox;
  colorBox.setWidth (width () * 0.5);
  colorBox.setHeight (height () * 0.65);
  colorBox.moveCenter (rect ().center ());
  QPainter pnt (this);
  pnt.fillRect (colorBox, chosenColor);
  pnt.end ();
}

void ColorButton::setColor (QColor color)
{
  chosenColor = color;
  repaint ();
}

/*****************************************************************************/

SettingsDialog::SettingsDialog (QSettings* appSettings, QWidget* parent)
  : QWidget (parent), settings (appSettings)
{
  resize (300, 500);
  setWindowTitle (tr ("Hexalizer - Settings"));
  setWindowIcon (QIcon ("/usr/local/share/Binaly/Binaly.png"));
  QRect winrect = geometry ();
  winrect.moveCenter (QApplication::desktop ()->availableGeometry ().center ());
  setGeometry (winrect);
  
  /* Load previously saved settings. */
  settings->beginGroup ("Colors");
  QColor textColor (settings->value ("TextSec", "#0000FF").toString ());
  QColor binColor (settings->value ("BinarySection", "#AAAA00").toString ());
  QColor homColor (settings->value ("HomoSection", "#808080").toString ());
  QColor miscColor (settings->value ("MiscColor", "#804000").toString ());
  settings->endGroup ();
  QString ccode = settings->value ("Language/Countrycode", "en").toString ();
  
  lyt = new QGridLayout (this);
  colors_box = new QGroupBox (tr ("Color settings"));
  cb_lyt = new QGridLayout (colors_box);
  
  cb_textCol = new QLabel (tr ("Text section color:"));
  cb_binCol = new QLabel (tr ("Binary section color:"));
  cb_homCol = new QLabel (tr ("Homogeneous section color:"));
  cb_miscCol = new QLabel (tr ("Other data section color:"));
  cb_textCol_btn = new ColorButton (textColor, this);
  cb_binCol_btn = new ColorButton (binColor, this);
  cb_homCol_btn = new ColorButton (homColor, this);
  cb_miscCol_btn = new ColorButton (miscColor, this);
  
  cb_lyt->addWidget (cb_textCol, 0, 0, 1, 1);
  cb_lyt->addWidget (cb_textCol_btn, 0, 1, 1, 1);
  cb_lyt->addWidget (cb_binCol, 1, 0, 1, 1);
  cb_lyt->addWidget (cb_binCol_btn, 1, 1, 1, 1);
  cb_lyt->addWidget (cb_homCol, 2, 0, 1, 1);
  cb_lyt->addWidget (cb_homCol_btn, 2, 1, 1, 1);
  cb_lyt->addWidget (cb_miscCol, 3, 0, 1, 1);
  cb_lyt->addWidget (cb_miscCol_btn, 3, 1, 1, 1);
  
  lang_box = new QGroupBox (tr ("Language settings"));
  lang_lyt = new QHBoxLayout (lang_box);
  lang_l = new QLabel (tr ("Language:"));
  lang_c = new QComboBox (this);
  lang_c->addItem (tr ("English"));
  lang_c->addItem (tr ("German"));
  lang_lyt->addWidget (lang_l);
  lang_lyt->addWidget (lang_c);
  if (ccode == "de")  {
    lang_c->setCurrentIndex (1);
  } else  {
    lang_c->setCurrentIndex (0);
  }
  
  ok_btn = new QPushButton (tr ("Apply"));
  cancel_btn = new QPushButton (tr ("Cancel"));
  lyt->addWidget (colors_box, 0, 0, 1, 4);
  lyt->addWidget (lang_box, 1, 0, 1, 4);
  lyt->addWidget (cancel_btn, 2, 1, 1, 1);
  lyt->addWidget (ok_btn, 2, 2, 1, 1);
  lyt->setColumnStretch (0, 1);
  lyt->setColumnStretch (1, 1);
  lyt->setColumnStretch (2, 1);
  lyt->setColumnStretch (3, 1);
  
  connect (cancel_btn, SIGNAL (clicked ()), this, SLOT (hide ()));
  connect (ok_btn, SIGNAL (clicked ()), this, SLOT (applySettings ()));
}

SettingsDialog::~SettingsDialog ()
{}

void SettingsDialog::applySettings ()
{
  settings->beginGroup ("Colors");
  settings->setValue ("TextSec", cb_textCol_btn->getColor ().name ());
  settings->setValue ("BinarySection", cb_binCol_btn->getColor ().name ());
  settings->setValue ("HomoSection", cb_homCol_btn->getColor ().name ());
  settings->setValue ("MiscColor", cb_miscCol_btn->getColor ().name ());
  settings->endGroup ();
  
  const QString countrycodes[2] = {"en", "de"};
  settings->setValue ("Language/Countrycode", countrycodes[lang_c->currentIndex ()]);
  
  QMessageBox::information (
    this,
    tr ("Settings saved"),
    tr ("The new settings have been applied. Some changes will take effect after you restart the application.")
  );
  hide ();
}