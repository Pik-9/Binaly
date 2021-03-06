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
 * \file Binaly.cpp
 * \author Daniel Steinhauer
 */

#include "Binaly.hpp"
#include "HexFile.hpp"

#include <QDesktopWidget>
#include <QStatusBar>
#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>

Binaly::Binaly (QSettings* appSettings)
  : QMainWindow (), filePosition (0), file (NULL), settings (appSettings), modified_file (false)
{
  resize (1000, 800);
  setWindowTitle (tr ("Binaly"));
  setWindowIcon (QIcon ("/usr/local/share/Binaly/Binaly.png"));
  QRect winrect = geometry ();
  winrect.moveCenter (QApplication::desktop ()->availableGeometry ().center ());
  setGeometry (winrect);
  
  streamLoader = new BackGroundWorker ();
  setDia = new SettingsDialog (settings);
  aboutDia = new AboutDialog ();
  
  fdia = new QFileDialog (
    this,
    tr ("Select file..."),
    QDir::homePath ()
  );
  
  filemenu = new QMenu (tr ("&File"), this);
  filemenu->addAction (tr ("&Open"), fdia, SLOT (show ()), QKeySequence ("CTRL+O"));
  saveAction = filemenu->addAction (tr ("&Save"), this, SLOT (saveFile ()), QKeySequence ("CTRL+S"));
  filemenu->addAction (tr ("&Quit"), this, SLOT (close ()), QKeySequence ("CTRL+Q"));
  menuBar ()->addMenu (filemenu);
  
  saveAction->setEnabled (false);
  
  settingsmenu = new QMenu (tr ("S&ettings"), this);
  settingsmenu->addAction (tr ("Settings"), setDia, SLOT (show ()));
  menuBar ()->addMenu (settingsmenu);
  
  helpmenu = new QMenu (tr ("&Help"), this);
  helpmenu->addAction (tr ("About Binaly"), aboutDia, SLOT (show ()));
  helpmenu->addAction (tr ("About Qt"), this, SLOT (showAboutQt ()));
  menuBar ()->addMenu (helpmenu);
  
  sp_main = new QSplitter (Qt::Vertical);
  sp_sub = new QSplitter (Qt::Horizontal);
  
  /* Prepare the file overview split. */
  overview_widget = new QWidget ();
  overview_lyt = new QGridLayout (overview_widget);
  bar = new BinaryBar (settings, this);
  
  /* Load previously saved settings. */
  settings->beginGroup ("Colors");
  QString textColor (settings->value ("TextSec", "#0000FF").toString ());
  QString binColor (settings->value ("BinarySection", "#AAAA00").toString ());
  QString homColor (settings->value ("HomoSection", "#808080").toString ());
  QString miscColor (settings->value ("MiscColor", "#804000").toString ());
  settings->endGroup ();
  
  l_plain = new QLabel (tr ("<font color='%1'>Homogeneous data</font>").arg (homColor));
  l_bin = new QLabel (tr ("<font color='%1'>Random binary data</font>").arg (binColor));
  l_text = new QLabel (tr ("<font color='%1'>Text data</font>").arg (textColor));
  l_other = new QLabel (tr ("<font color='%1'>Other data</font>").arg (miscColor));
  l_pos = new QLabel (tr ("Current position: 0"));
  prev_btn = new QPushButton (tr ("<= 1024 [Q]"));
  next_btn = new QPushButton (tr ("[W] 1024 =>"));
  prev_btn->setEnabled (false);
  next_btn->setEnabled (false);
  l_plain->setAlignment (Qt::AlignCenter);
  l_bin->setAlignment (Qt::AlignCenter);
  l_text->setAlignment (Qt::AlignCenter);
  l_other->setAlignment (Qt::AlignCenter);
  l_pos->setAlignment (Qt::AlignCenter);
  overview_lyt->addWidget (bar, 0, 0, 2, 20);
  overview_lyt->addWidget (l_plain, 2, 0, 1, 5);
  overview_lyt->addWidget (l_bin, 2, 5, 1, 5);
  overview_lyt->addWidget (l_text, 2, 10, 1, 5);
  overview_lyt->addWidget (l_other, 2, 15, 1, 5);
  overview_lyt->addWidget (prev_btn, 3, 4, 1, 4);
  overview_lyt->addWidget (l_pos, 3, 8, 1, 4);
  overview_lyt->addWidget (next_btn, 3, 12, 1, 4);
  overview_widget->setMinimumHeight (250);
  
  /* Prepare the TabWidget that contains the histograms. */
  hist_widget = new QWidget (this);
  hist_lyt = new QGridLayout (hist_widget);
  hist_tabs = new QTabWidget (hist_widget);
  hist_tabs->setTabPosition (QTabWidget::South);
  dev_hist = new Histogram (hist_tabs);
  fourier_hist = new FourierSheet (hist_tabs);
  hist_tabs->addTab (dev_hist, tr ("Byte histogram"));
  hist_tabs->addTab (fourier_hist, tr ("Fourier transform"));
  hist_lyt->addWidget (hist_tabs, 0, 0, 1, 1);
  hist_widget->setMinimumWidth (300);
  
  hexw = new HexWidget ();
  
  sp_sub->addWidget (hist_widget);
  sp_sub->addWidget (hexw);
  sp_main->addWidget (overview_widget);
  sp_main->addWidget (sp_sub);
  
  sp_main->setStretchFactor (0, 1);
  sp_main->setStretchFactor (1, 2);
  
  sp_sub->setStretchFactor (0, 1);
  sp_sub->setStretchFactor (1, 1);
  
  statusBar ()->showMessage (tr ("Ready"));
  
  setCentralWidget (sp_main);
  
  connect (bar, SIGNAL (filePosChanged (uint64_t)), this, SLOT (changeFilePosition (uint64_t)));
  connect (bar, SIGNAL (error ()), this, SLOT (error ()));
  connect (dev_hist, SIGNAL (error ()), this, SLOT (error ()));
  connect (fourier_hist, SIGNAL (error ()), this, SLOT (error ()));
  connect (fdia, SIGNAL (fileSelected (QString)), this, SLOT (openFile (QString)));
  connect (streamLoader, SIGNAL (finished ()), this, SLOT (fileLoaded ()));
  connect (streamLoader, SIGNAL (errorOccured ()), this, SLOT (error ()));
  connect (hexw, SIGNAL (fileChanged ()), this, SLOT (fileModified ()));
  connect (prev_btn, SIGNAL (clicked ()), this, SLOT (prevKiB ()));
  connect (next_btn, SIGNAL (clicked ()), this, SLOT (nextKiB ()));
  
  /* If application is called with argument. Open the file on startup. */
  QStringList args = qApp->arguments ();
  if (args.count () > 1)  {
    openFile (args.at (1));
  }
}

Binaly::~Binaly ()
{}

bool Binaly::askFileSave ()
{
  QMessageBox::StandardButton reply = QMessageBox::question (
    this,
    tr ("Unsaved changes"),
    tr ("The current file has unsaved changes. Do you want to save it?"),
    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
  );
  
  if (reply == QMessageBox::Yes)  {
    saveFile ();
  }
  
  return ((reply == QMessageBox::Yes) || (reply == QMessageBox::No));
}

void Binaly::keyPressEvent (QKeyEvent* event)
{
  switch (event->key ())  {
    case 81:  {
      prevKiB ();
      break;
    }
    
    case 87:  {
      nextKiB ();
      break;
    }
  }
}

void Binaly::closeEvent (QCloseEvent* event)
{
  if (modified_file)  {
    if (!askFileSave ())  {
      event->ignore ();
    }
  }
}

void Binaly::changeFilePosition (uint64_t newPos)
{
  filePosition = newPos;
  l_pos->setText (tr ("Current position: %1").arg (BinaryBar::sizeString (filePosition)));
  prev_btn->setEnabled (filePosition >= 1024);
  next_btn->setEnabled ((file->filesize () - filePosition) >= 1024);
  dev_hist->loadData (filePosition);
  fourier_hist->loadData (filePosition);
  hexw->loadData (filePosition);
}

void Binaly::showAboutQt ()
{
  QMessageBox::aboutQt (this, tr ("About Qt"));
}

void Binaly::openFile (QString filePath)
{
  bool goon = true;
  if (modified_file)  {
    goon = askFileSave ();
  }
  
  if (goon)  {
    if (file)  {
      delete file;
    }
    path = filePath;
    file = new Hexfile (path.toStdString ().c_str ());
    streamLoader->setFileStream (file);
  
    /* Set all element's file stream pointers to NULL. */
    bar->setFileStream (NULL);
    dev_hist->setFileStream (NULL);
    fourier_hist->setFileStream (NULL);
    hexw->setFileStream (NULL);
  
    streamLoader->start ();
    statusBar ()->showMessage (tr ("Loading file %1, please wait...").arg (path));
  }
}

void Binaly::fileModified ()
{
  modified_file = true;
  statusBar ()->showMessage (tr ("File changed."));
  setWindowTitle (tr ("Binaly - %1 *").arg (path));
  saveAction->setEnabled (true);
}

void Binaly::saveFile ()
{
  try  {
    hexw->saveChanges ();
  } catch (EFileException& exc)  {
    error ();
  }
  
  if (!file->failStatus ())  {
    modified_file = false;
    statusBar ()->showMessage (tr ("File saved to %1.").arg (path));
    setWindowTitle (tr ("Binaly - %1").arg (path));
    saveAction->setEnabled (false);
  }
}

void Binaly::fileLoaded ()
{
  if (!file->failStatus ())  {
    modified_file = false;
    statusBar ()->showMessage (tr ("File %1 loaded.").arg (path));
    bar->setFileStream (file);
    dev_hist->setFileStream (file);
    fourier_hist->setFileStream (file);
    hexw->setFileStream (file);
    changeFilePosition (0);
    setWindowTitle (tr ("Binaly - %1").arg (path));
  } else  {
    delete file;
    path = "";
    file = NULL;
  }
}

void Binaly::error ()
{
  file->fail ();
  QMessageBox::critical (
    this,
    tr ("File error!"),
    tr ("An error occured while reading/writing the file!")
  );
  statusBar ()->showMessage (tr ("IO error"));
}

void Binaly::prevKiB ()
{
  if (filePosition >= 1023)  {
    bar->setPositionOnBar (filePosition -= 1024);
  }
}

void Binaly::nextKiB ()
{
  if (file)  {
    if ((file->filesize () - filePosition) >= 1024)  {
      bar->setPositionOnBar (filePosition += 1024);
    }
  }
}