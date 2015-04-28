#include "custom_comp.hpp"
#include <QPainter>

Histogram::Histogram (QWidget *parent)
  : QWidget (parent)
{}

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
      pnt.fillRect (col, Qt::blue);
    }
  } else  {
    pnt.setPen (Qt::black);
    pnt.drawText (rect (), Qt::AlignCenter, tr ("No data"));
  }
  pnt.end ();
}

//#include "moc_custom_comp.cpp"