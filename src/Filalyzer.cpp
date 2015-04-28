#include "Filalyzer.hpp"
#include "HexFile.hpp"

#include <QDesktopWidget>
#include <QApplication>
#include <QMenuBar>

Filalyzer::Filalyzer ()
  : QMainWindow (), filePosition (0), file (NULL)
{
  resize (900, 600);
  setWindowTitle (tr ("Filalyzer"));
  QRect winrect = geometry ();
  winrect.moveCenter (QApplication::desktop ()->availableGeometry ().center ());
  setGeometry (winrect);
  
  fdia = new QFileDialog (
    this,
    tr ("Select file..."),
    QDir::homePath ()
  );
  
  filemenu = new QMenu (tr ("&File"), this);
  filemenu->addAction (tr ("&Open"), fdia, SLOT (show ()));
  filemenu->addAction (tr ("&Quit"), qApp, SLOT (quit ()));
  menuBar()->addMenu (filemenu);
  
  sp_main = new QSplitter (Qt::Vertical);
  sp_sub = new QSplitter (Qt::Horizontal);
  
  /* Prepare the file overview split. */
  overview_widget = new QWidget ();
  overview_lyt = new QGridLayout (overview_widget);
  bar = new BinaryBar (this);
  l_plain = new QLabel (tr ("<font color='gray'>Homogeneous data</font>"));
  l_bin = new QLabel (tr ("<font color='yellow'>Random binary data</font>"));
  l_text = new QLabel (tr ("<font color='blue'>Text data</font>"));
  l_other = new QLabel (tr ("<font color='red'>Other data</font>"));
  l_pos = new QLabel (tr ("Current position: 0"));
  l_plain->setAlignment (Qt::AlignCenter);
  l_bin->setAlignment (Qt::AlignCenter);
  l_text->setAlignment (Qt::AlignCenter);
  l_other->setAlignment (Qt::AlignCenter);
  l_pos->setAlignment (Qt::AlignCenter);
  overview_lyt->addWidget (bar, 0, 0, 2, 4);
  overview_lyt->addWidget (l_plain, 2, 0, 1, 1);
  overview_lyt->addWidget (l_bin, 2, 1, 1, 1);
  overview_lyt->addWidget (l_text, 2, 2, 1, 1);
  overview_lyt->addWidget (l_other, 2, 3, 1, 1);
  overview_lyt->addWidget (l_pos, 3, 1, 1, 2);
  
  /* Prepare the TabWidget that contains the histograms. */
  hist_tabs = new QTabWidget (this);
  hist_tabs->setTabPosition (QTabWidget::South);
  dev_hist = new Histogram (hist_tabs);
  hist_tabs->addTab (dev_hist, tr ("Byte histogram"));
  
  l_edit = new QLabel (tr ("Hex editor for block"));
  
  sp_sub->addWidget (hist_tabs);
  sp_sub->addWidget (l_edit);
  sp_main->addWidget (overview_widget);
  sp_main->addWidget (sp_sub);
  
  sp_main->setStretchFactor (0, 1);
  sp_main->setStretchFactor (1, 2);
  
  setCentralWidget (sp_main);
  
  connect (bar, SIGNAL (filePosChanged (uint64_t)), this, SLOT (changeFilePosition (uint64_t)));
  connect (fdia, SIGNAL (fileSelected (QString)), this, SLOT (openFile (QString)));
}

Filalyzer::~Filalyzer ()
{}

void Filalyzer::changeFilePosition (uint64_t newPos)
{
  filePosition = newPos;
  l_pos->setText (tr ("Current position: %1").arg (BinaryBar::sizeString (filePosition)));
  dev_hist->loadData (filePosition);
}

void Filalyzer::openFile (QString filePath)
{
  if (file)  {
    delete file;
  }
  file = new Hexfile (filePath.toStdString ().c_str ());
  file->loadFile ();
  bar->setFileStream (file);
  dev_hist->setFileStream (file);
}