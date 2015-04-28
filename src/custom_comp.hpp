#ifndef CUSTOM_COMPONENTS_H
#define CUSTOM_COMPONENTS_H

#include <QWidget>
#include <QPaintEvent>

#include "HexFile.hpp"

class BinaryBar : public QWidget
{
  Q_OBJECT
private:
  Hexfile *stream;
  
  unsigned int currentPos;
  
public:
  BinaryBar (QWidget *parent = 0);
  virtual ~BinaryBar ();
  
  void setFileStream (Hexfile*);
  
  uint64_t positionInFile (const unsigned int);
  
  static QString sizeString (const uint64_t);
  
public:
  static unsigned int countSections;
  
protected:
  void paintEvent (QPaintEvent*);
  void mousePressEvent (QMouseEvent*);
  
signals:
  void filePosChanged (uint64_t);
};

#endif