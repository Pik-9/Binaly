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
#include <QPushButton>

#include "custom_comp.hpp"
#include "Hexedit.hpp"

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
  QPushButton *prev_btn, *next_btn;
  
  /* The histogram widget */
  QWidget *hist_widget;
  QGridLayout *hist_lyt;
  QTabWidget *hist_tabs;
  Histogram *dev_hist, *fourier_hist;
  
  /* The hexedit widget */
  HexWidget *hexw;
public:
  Filalyzer ();
  virtual ~Filalyzer ();
  
public slots:
  void changeFilePosition (uint64_t);
  void openFile (QString);
  void prevKiB ();
  void nextKiB ();
};

#endif // Filalyzer_H