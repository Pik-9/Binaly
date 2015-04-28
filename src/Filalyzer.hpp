#ifndef Filalyzer_H
#define Filalyzer_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QSplitter>
#include <QLabel>
#include <QFileDialog>
#include <QMenu>
#include <QTabWidget>

#include "custom_comp.hpp"

class Filalyzer : public QMainWindow
{
  Q_OBJECT
private:
  /* The current position in the file. */
  uint64_t filePosition;
  
  /* The file stream to analyze. */
  Hexfile *file;
  
  QFileDialog *fdia;
  
  QMenu *filemenu;
  
  QSplitter *sp_main, *sp_sub;
  
  /* The file overview widget. */
  QWidget *overview_widget;
  QGridLayout *overview_lyt;
  BinaryBar *bar;
  QLabel *l_plain, *l_bin, *l_text, *l_other,*l_pos;
  
  /* The histogram widget */
  QTabWidget *hist_tabs;
  Histogram *dev_hist;
  
  /* Just placeholders. */
  QLabel *l_hist, *l_edit;
public:
  Filalyzer ();
  virtual ~Filalyzer ();
  
public slots:
  void changeFilePosition (uint64_t);
  void openFile (QString);
};

#endif // Filalyzer_H