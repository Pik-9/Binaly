#ifndef HEXEDIT_H
#define HEXEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QGridLayout>

#include "HexFile.hpp"

class HexWidget : public QWidget
{
  Q_OBJECT
private:
  QComboBox *c_encoding;
  QTextEdit *t_hexedit, *t_asciiedit;
  QGridLayout *lyt;
  
  Hexfile *stream;
  
  static const unsigned int bases[4];
  
public:
  HexWidget (QWidget *parent = 0);
  virtual ~HexWidget ();
  
  static QChar printableMatch (const char);
  
  void setFileStream (Hexfile*);
  
  void loadData (const uint64_t);
};

#endif