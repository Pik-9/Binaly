#ifndef CUSTOM_COMPONENTS_H
#define CUSTOM_COMPONENTS_H

#include <QWidget>
#include <QPaintEvent>

#include <vector>

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

class Histogram : public QWidget
{
private:
  std::vector<unsigned int> deviation;
  Hexfile *stream;
  
public:
  Histogram (QWidget *parent = 0);
  virtual ~Histogram ();
  
  void setFileStream (Hexfile*);
  
  virtual void loadData (const uint64_t);
  
protected:
  void paintEvent (QPaintEvent*);
};

#endif