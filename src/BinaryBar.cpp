#include "custom_comp.hpp"

#include <QPainter>

#include <math.h>

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

#include "moc_custom_comp.cpp"