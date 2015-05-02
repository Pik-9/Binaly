#ifndef CUSTOM_COMPONENTS_H
#define CUSTOM_COMPONENTS_H

#include <QWidget>
#include <QPaintEvent>

#include <vector>
#include <complex>

#include "HexFile.hpp"

class BinaryBar : public QWidget
{
  Q_OBJECT
private:
  Hexfile *stream;
  
  /**
   * \brief The colors of the binary strip are temporarily stored here.
   * So we don't have to calculate them again everytime repaint() is called!
   */
  std::vector<QColor> colorStrip;
  
  unsigned int currentPos;
  
public:
  BinaryBar (QWidget *parent = 0);
  virtual ~BinaryBar ();
  
  void setFileStream (Hexfile*);
  
  uint64_t positionInFile (const unsigned int);
  void setPositionOnBar (const uint64_t);
  
  /**
   * \brief Calculate the temporarily stored color in \c colorStrip.
   */
  void drawColorStrip ();
  
  static QString sizeString (const uint64_t);
  
public:
  static unsigned int countSections;
  
protected:
  void paintEvent (QPaintEvent*);
  void mousePressEvent (QMouseEvent*);
  void mouseMoveEvent (QMouseEvent*);
  void mouseReleaseEvent (QMouseEvent*);
  void resizeEvent (QResizeEvent*);
  
signals:
  void filePosChanged (uint64_t);
};

/*****************************************************************************/

class Histogram : public QWidget
{
  Q_OBJECT
protected:
  std::vector<unsigned int> deviation;
  Hexfile *stream;
  
  /* The column the cursor currently points to. */
  signed int currentColumn;
  
  Histogram *daughter;
  
public:
  Histogram (QWidget *parent = 0);
  Histogram (Histogram&);
  virtual ~Histogram ();
  
  void setFileStream (Hexfile*);
  
  virtual void loadData (const uint64_t);
  virtual QString currentColStr ();
  
protected:
  void paintEvent (QPaintEvent*);
  void mousePressEvent (QMouseEvent*);
  void mouseMoveEvent (QMouseEvent*);
};

/*****************************************************************************/

class FourierSheet : public Histogram
{
  Q_OBJECT
protected:
  std::vector<std::complex < double > > real_four;

public:
  FourierSheet (QWidget *parent = 0);
  FourierSheet (FourierSheet&);
  virtual ~FourierSheet ();
  
  void loadData (const uint64_t);
  QString currentColStr ();
  
protected:
  void paintEvent (QPaintEvent*);
  void mousePressEvent (QMouseEvent*);
  void mouseMoveEvent (QMouseEvent*);
};

#endif