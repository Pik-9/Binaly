#ifndef HEXEDIT_H
#define HEXEDIT_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
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
  /*Blockinfo currentBlockInfo;
  std::vector<char> currentBlockData;*/
  
  /**
   * The index of the currently loaded block of data.
   */
  unsigned int currentIndex;
  
  /**
   * \details A kind of lock for reacting on cell changes in the table widget.
   * If true: The computer doesn't access the table cells now. It must be
   * a user action, which we should react to.
   * If false: The computer has to write to cells. Our \c editCell method
   * shouldn't do anything.
   */
  bool table_printed;
  
  /**
   * The edited blocks of data.
   */
  std::vector<FilePart> customData;
  
  static const unsigned int bases[4];
  
public:
  HexWidget (QWidget *parent = 0);
  virtual ~HexWidget ();
  
  static QChar printableMatch (const char);
  
  void setFileStream (Hexfile*);
  
  void loadData (const uint64_t);
  void saveChanges ();
  void printData ();
  
public slots:
  void encodingChanged (int);
  void editCell (QTableWidgetItem*);
  
signals:
  void fileChanged ();
};

#endif