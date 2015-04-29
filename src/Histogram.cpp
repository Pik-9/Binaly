#include "custom_comp.hpp"

#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>

#include <iostream>

Histogram::Histogram (QWidget *parent)
  : QWidget (parent), currentColumn (-1)
{
  setCursor (Qt::PointingHandCursor);
  setMouseTracking (true);
  daughter = new Histogram (*this);
}

Histogram::Histogram (Histogram& other)
  : QWidget (0),
    deviation (other.deviation),
    stream (other.stream),
    daughter (NULL),
    currentColumn (-1)
{
  resize (1024, 768);
  setCursor (Qt::ArrowCursor);
  setMouseTracking (true);
  QRect winrect = geometry ();
  winrect.moveCenter (QApplication::desktop()->geometry ().center ());
  setGeometry (winrect);
}

Histogram::~Histogram ()
{
  deviation.clear ();
}

void Histogram::setFileStream (Hexfile* fstr)
{
  stream = fstr;
  repaint ();
}

void Histogram::loadData (const uint64_t position)
{
  /* In this special case we just want to know the deviation of the bytes. */
  deviation.clear ();
  deviation.resize (256);
  
  std::vector<char> data = stream->getBlockDataAt (position);
  
  for (unsigned int ii = 0; ii < data.size (); ++ii)  {
    unsigned char byte = data[ii];
    deviation[(unsigned char) data[ii]]++;
  }
  repaint ();
}

QString Histogram::currentColStr ()
{
  QString RET = tr ("No data");
  if (currentColumn != (-1))  {
    QString index_str = "";
    index_str.setNum (currentColumn);
    QString count_str = "";
    count_str.setNum (deviation[currentColumn]);
    RET = tr ("Byte #%1 has %2 elements.").arg (index_str).arg (count_str);
  }
  return RET;
}

void Histogram::paintEvent (QPaintEvent* event)
{
  QPainter pnt;
  pnt.begin (this);
  pnt.fillRect (rect (), Qt::white);
  if (deviation.size ())  {
    double columnWidth = (double) width () / deviation.size ();
    
    /* Search for maximum value */
    unsigned int max = 0;
    for (unsigned int ii = 0; ii < deviation.size (); ++ii)  {
      if (deviation[ii] > max)  {
        max = deviation[ii];
      }
    }
    
    QRect col;
    col.setWidth ((unsigned int) columnWidth);
    for (unsigned int ii = 0; ii < deviation.size (); ++ii)  {
      col.setHeight (height () * deviation[ii] / max);
      col.moveBottomLeft (QPoint ((unsigned int) (ii * columnWidth), height ()));
      QColor colColor = (ii == currentColumn ? Qt::red : Qt::blue);
      pnt.fillRect (col, colColor);
    }
  } else  {
    pnt.setPen (Qt::black);
    pnt.drawText (rect (), Qt::AlignCenter, tr ("No data"));
  }
  pnt.end ();
}

void Histogram::mousePressEvent (QMouseEvent* event)
{
  if (daughter)  {
    daughter->hide ();
    delete daughter;
    daughter = new Histogram (*this);
    daughter->show ();
  }
}

void Histogram::mouseMoveEvent (QMouseEvent* event)
{
  if (deviation.size () > 0)  {
    currentColumn = deviation.size () * event->x () / width ();
  }
  setToolTip (currentColStr ());
  repaint ();
}

/*****************************************************************************/

FourierSheet::FourierSheet (QWidget *parent)
  : Histogram (parent)
{}

FourierSheet::FourierSheet (Histogram& other)
  : Histogram (other)
{}

FourierSheet::~FourierSheet ()
{}

void FourierSheet::loadData (const uint64_t position)
{
  /* Fourier transform not implemented yet. */
}

QString FourierSheet::currentColStr ()
{
  QString RET = tr ("No data");
  if (currentColumn != (-1))  {
    QString index_str = "";
    index_str.setNum (currentColumn);
    QString count_str = "";
    count_str.setNum (deviation[currentColumn]);
    RET = tr ("F(f(%1)) = %2").arg (index_str).arg (count_str);
  }
  return RET;
}

void FourierSheet::mousePressEvent (QMouseEvent* event)
{
  if (daughter)  {
    daughter->hide ();
    delete daughter;
    daughter = new FourierSheet (*this);
    daughter->show ();
  }
}