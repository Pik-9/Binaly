#ifndef HEXEDIT_H
#define HEXEDIT_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QGridLayout>

#include "HexFile.hpp"

class HexWidget : public QWidget
{
  Q_OBJECT
private:
  QComboBox *c_encoding;
  QTableWidget *hex_table;
  QGridLayout *lyt;
  
  Hexfile *stream;
  Blockinfo currentBlockInfo;
  std::vector<char> currentBlockData;
  
  static const unsigned int bases[4];
  
public:
  HexWidget (QWidget *parent = 0);
  virtual ~HexWidget ();
  
  static QChar printableMatch (const char);
  
  void setFileStream (Hexfile*);
  
  void loadData (const uint64_t);
  void printData ();
  
public slots:
  void encodingChanged (int);
};

#endif