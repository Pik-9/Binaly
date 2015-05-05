#include "Filalyzer.hpp"
#include "HexFile.hpp"

#include <QDesktopWidget>
#include <QStatusBar>
#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>

Filalyzer::Filalyzer (QSettings* appSettings)
  : QMainWindow (), filePosition (0), file (NULL), settings (appSettings)
{
  resize (1000, 800);
  setWindowTitle (tr ("Filalyzer"));
  QRect winrect = geometry ();
  winrect.moveCenter (QApplication::desktop ()->availableGeometry ().center ());
  setGeometry (winrect);
  
  streamLoader = new BackGroundWorker ();
  setDia = new SettingsDialog (settings);
  
  fdia = new QFileDialog (
    this,
    tr ("Select file..."),
    QDir::homePath ()
  );
  
  filemenu = new QMenu (tr ("&File"), this);
  filemenu->addAction (tr ("&Open"), fdia, SLOT (show ()), tr ("CTRL+O"));
  filemenu->addAction (tr ("&Save"), this, SLOT (saveFile ()), tr ("CTRL+S"));
  filemenu->addAction (tr ("&Quit"), qApp, SLOT (quit ()), tr ("CTRL+Q"));
  menuBar ()->addMenu (filemenu);
  
  settingsmenu = new QMenu (tr ("S&ettings"), this);
  settingsmenu->addAction (tr ("Settings"), setDia, SLOT (show ()));
  menuBar ()->addMenu (settingsmenu);
  
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
  
  l_plain = new QLabel (tr ("<font color='%1'>Homogeneous data</font>").arg (textColor));
  l_bin = new QLabel (tr ("<font color='%1'>Random binary data</font>").arg (binColor));
  l_text = new QLabel (tr ("<font color='%1'>Text data</font>").arg (homColor));
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
}

Filalyzer::~Filalyzer ()
{}

void Filalyzer::keyPressEvent (QKeyEvent* event)
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

void Filalyzer::changeFilePosition (uint64_t newPos)
{
  filePosition = newPos;
  l_pos->setText (tr ("Current position: %1").arg (BinaryBar::sizeString (filePosition)));
  prev_btn->setEnabled (filePosition >= 1024);
  next_btn->setEnabled ((file->filesize () - filePosition) >= 1024);
  dev_hist->loadData (filePosition);
  fourier_hist->loadData (filePosition);
  hexw->loadData (filePosition);
}

void Filalyzer::openFile (QString filePath)
{
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

void Filalyzer::fileModified ()
{
  statusBar ()->showMessage (tr ("File changed."));
  setWindowTitle (tr ("Filalyzer - %1 *").arg (path));
}

void Filalyzer::saveFile ()
{
  try  {
    hexw->saveChanges ();
  } catch (EFileException& exc)  {
    error ();
  }
  
  if (!file->failStatus ())  {
    statusBar ()->showMessage (tr ("File saved to %1.").arg (path));
    setWindowTitle (tr ("Filalyzer - %1").arg (path));
  }
}

void Filalyzer::fileLoaded ()
{
  if (!file->failStatus ())  {
    statusBar ()->showMessage (tr ("File %1 loaded.").arg (path));
    bar->setFileStream (file);
    dev_hist->setFileStream (file);
    fourier_hist->setFileStream (file);
    hexw->setFileStream (file);
    changeFilePosition (0);
    setWindowTitle (tr ("Filalyzer - %1").arg (path));
  } else  {
    delete file;
    path = "";
    file = NULL;
  }
}

void Filalyzer::error ()
{
  file->fail ();
  QMessageBox::critical (
    this,
    tr ("File error!"),
    tr ("An error occured while reading/writing the file!")
  );
  statusBar ()->showMessage (tr ("IO error"));
}

void Filalyzer::prevKiB ()
{
  if (filePosition >= 1023)  {
    bar->setPositionOnBar (filePosition -= 1024);
  }
}

void Filalyzer::nextKiB ()
{
  if (file)  {
    if ((file->filesize () - filePosition) >= 1024)  {
      bar->setPositionOnBar (filePosition += 1024);
    }
  }
}