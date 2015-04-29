#include "custom_comp.hpp"

#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>

#include <math.h>
#include <complex>

unsigned int BinaryBar::countSections = 4;

BinaryBar::BinaryBar (QWidget *parent)
  : QWidget (parent), stream (NULL), currentPos (0)
{}

BinaryBar::~BinaryBar ()
{}

void BinaryBar::setFileStream (Hexfile* fstr)
{
  stream = fstr;
  repaint ();
}

uint64_t BinaryBar::positionInFile (const unsigned int widget_x)
{
  return stream->filesize () * widget_x / width ();
}

QString BinaryBar::sizeString (const uint64_t size)
{
  unsigned int dimension = (unsigned int) log2 ((double) size);
  const char *unit_spec[4] = {
    "B",
    "KiB",
    "MiB",
    "GiB"
  };
  
  unsigned int unit_dim = 0;
  if (dimension < 10)  {
    unit_dim = 0;
  } else if (dimension < 20)  {
    unit_dim = 1;
  } else if (dimension < 30)  {
    unit_dim = 2;
  } else {
    unit_dim = 3;
  }
  
  uint64_t unit_size = (uint64_t) pow (1024.0, unit_dim);
  return QString ("%1 %2").arg ((double) size / unit_size).arg (unit_spec[unit_dim]);
}

void BinaryBar::paintEvent (QPaintEvent* event)
{
  QPainter pnt;
  pnt.begin (this);
  if (stream)  {
    /* Draw the sections */
    unsigned barHeight = 0.5 * height ();
    for (unsigned int pixel = 0; pixel < width (); pixel++)  {
      Blockinfo bi = stream->getBlockAt (positionInFile (pixel));
      QColor sectionColor;
      if ((bi.avrg > 64.0) && (bi.stddev < 40.0))  {
        /* This file section is a text section (blue) */
        pnt.setPen (Qt::blue);
      } else if ((bi.avrg > 108.0) && (bi.avrg < 148.0) && (bi.stddev > 60.0) && (bi.stddev < 68.0))  {
        /* This file section is a binary section (yellow) */
        pnt.setPen (Qt::darkYellow);
      } else if (bi.stddev < 2.0)  {
        /* This file section is a homogeneous data section e.g. 0000...0000 (gray) */
        pnt.setPen (Qt::gray);
      } else  {
        /* This file section doesn't fit into any other category (red) */
        pnt.setPen (Qt::red);
      }
      
      pnt.drawLine (pixel, 0, pixel, barHeight);
    }
    
    /* Draw the labels. */
    uint64_t steps = stream->filesize () / countSections;
    pnt.setPen (Qt::black);
    QRect sizeLabel;
    sizeLabel.setWidth (80);
    sizeLabel.setHeight (0.2 * height ());
    for (uint64_t ii = 1; ii <= countSections; ++ii)  {
      unsigned int co_x = ii * width () / countSections;
      pnt.drawLine (co_x, height () * 0.5, co_x, height () * 0.8);
      sizeLabel.moveBottomRight (QPoint (co_x, height () * 0.8));
      pnt.drawText (sizeLabel, Qt::AlignRight, BinaryBar::sizeString (ii * steps));
    }
    
    /* Draw the currently selected position as white line. */
    pnt.setPen (Qt::white);
    pnt.drawLine (currentPos, 0, currentPos, barHeight);
  } else  {
    pnt.drawText (rect (), Qt::AlignCenter, tr ("No data"));
  }
  pnt.end ();
}

void BinaryBar::mousePressEvent (QMouseEvent* event)
{
  if (stream)  {
    currentPos = event->x ();
    emit filePosChanged (positionInFile (currentPos));
    repaint ();
  }
}

/*****************************************************************************/

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
  : Histogram (parent), real_four (0)
{}

FourierSheet::FourierSheet (FourierSheet& other)
  : Histogram (other), real_four (other.real_four)
{}

FourierSheet::~FourierSheet ()
{}

void FourierSheet::loadData (const uint64_t position)
{
  std::vector<char> data = stream->getBlockDataAt (position);
  real_four.clear ();
  real_four.resize (data.size ());
  
  /* Do the discrete Fourier transform. */
  register unsigned int j, k;
  for (k = 0; k < real_four.size (); ++k)  {
    real_four[k] = 0.0;
    for (j = 0; j < data.size (); ++j)  {
      double theta = -2.0 * M_PI * j * k / (double) data.size ();
      std::complex<double> z = std::polar (1.0, theta) * (double) data[j];
      /* We are only interested in |Re(z)|. */
      real_four[k] += fabs (z.real ());
    }
  }
  
  repaint ();
}

QString FourierSheet::currentColStr ()
{
  QString RET = tr ("No data");
  if (currentColumn != (-1))  {
    QString index_str = "";
    index_str.setNum (2.0 * M_PI / (currentColumn + 1));
    QString count_str = "";
    count_str.setNum (real_four[currentColumn]);
    RET = tr ("F(f(%1)) = %2").arg (index_str).arg (count_str);
  }
  return RET;
}

void FourierSheet::paintEvent (QPaintEvent* event)
{
  QPainter pnt;
  pnt.begin (this);
  pnt.fillRect (rect (), Qt::white);
  if (real_four.size ())  {
    double columnWidth = (double) width () / real_four.size ();
    
    /* Search for maximum value */
    double max = 0.0;
    for (unsigned int ii = 0; ii < real_four.size (); ++ii)  {
      if (real_four[ii] > max)  {
        max = real_four[ii];
      }
    }
    
    pnt.setPen (Qt::blue);
    for (unsigned int ii = 1; ii < real_four.size (); ++ii)  {
      unsigned int x_1 = (ii - 1) * width () / real_four.size ();
      unsigned int x_2 = ii * width () / real_four.size ();
      unsigned int y_1 = real_four[ii - 1] * height () / max;
      unsigned int y_2 = real_four[ii] * height () / max;
      pnt.drawLine (x_1, y_1, x_2, y_2);
    }
    unsigned int red_x = currentColumn * width () / real_four.size ();
    pnt.setPen (Qt::red);
    pnt.drawLine (red_x, 0, red_x, height ());
  } else  {
    pnt.setPen (Qt::black);
    pnt.drawText (rect (), Qt::AlignCenter, tr ("No data"));
  }
  pnt.end ();
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

void FourierSheet::mouseMoveEvent (QMouseEvent* event)
{
  if (real_four.size ())  {
    currentColumn = real_four.size () * event->x () / width ();
  }
  setToolTip (currentColStr ());
  repaint ();
}