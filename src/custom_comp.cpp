/******************************************************************************
 *  Binaly                                                                    *
 *                                                                            *
 *   (C) 2015 by Daniel Steinhauer <pik-9@users.sourceforge.net>              *
 *                                                                            *
 *                                                                            *
 *  This file is part of Binaly.                                              *
 *                                                                            *
 *  Binaly is free software: you can redistribute it and/or modify            *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  Binaly is distributed in the hope that it will be useful,                 *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with Binaly.  If not, see <http://www.gnu.org/licenses/>.           *
 ******************************************************************************/

/**
 * \file custom_comp.cpp
 * \author Daniel Steinhauer
 */

#include "custom_comp.hpp"

#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>

#include <math.h>

unsigned int BinaryBar::countSections = 4;

BinaryBar::BinaryBar (QSettings* appSettings, QWidget *parent)
  : QWidget (parent), stream (NULL), currentPos (0), colorStrip (0), settings (appSettings)
{}

BinaryBar::~BinaryBar ()
{}

void BinaryBar::setFileStream (Hexfile* fstr)
{
  stream = fstr;
  drawColorStrip ();
  repaint ();
}

uint64_t BinaryBar::positionInFile (const unsigned int widget_x)
{
  return (stream->filesize () - 1) * widget_x / width ();
}

void BinaryBar::setPositionOnBar (const uint64_t filepos)
{
  currentPos = width () * filepos / stream->filesize ();
  repaint ();
  emit filePosChanged (filepos);
}

void BinaryBar::drawColorStrip ()
{
  colorStrip.resize (width ());
  if (stream)  {
    settings->beginGroup ("Colors");
    QColor textColor (settings->value ("TextSec", "#0000FF").toString ());
    QColor binColor (settings->value ("BinarySection", "#AAAA00").toString ());
    QColor homColor (settings->value ("HomoSection", "#808080").toString ());
    QColor miscColor (settings->value ("MiscColor", "#804000").toString ());
    settings->endGroup ();
    register unsigned int pixel;
    for (pixel = 0; pixel < width (); ++pixel)  {
      Blockinfo bi;
      try  {
        bi = stream->getBlockAt (positionInFile (pixel));
      } catch (EFileException& exc)  {
        stream = NULL;
        emit error ();
      }
      if ((bi.avrg > 64.0) && (bi.stddev < 40.0))  {
        /* This file section is a text section (blue) */
        colorStrip[pixel] = textColor;
      } else if ((bi.avrg > 108.0) && (bi.avrg < 148.0) && (bi.stddev > 60.0) && (bi.stddev < 68.0))  {
        /* This file section is a binary section (yellow) */
        colorStrip[pixel] = binColor;
      } else if (bi.stddev < 2.0)  {
        /* This file section is a homogeneous data section e.g. 0000...0000 (gray) */
        colorStrip[pixel] = homColor;
      } else  {
        /* This file section doesn't fit into any other category (red) */
        colorStrip[pixel] = miscColor;
      }
    }
  }
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
    
    for (unsigned int pixel = 0; pixel < width (); ++pixel)  {
      pnt.setPen (colorStrip[pixel]);
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
    
    /* Draw the currently selected position as white rectangle. */
    pnt.setPen (Qt::white);
    QRect selRect;
    selRect.setHeight (barHeight);
    selRect.setWidth (width () * 1024 / stream->filesize ());
    selRect.moveTopLeft (QPoint (currentPos, 0));
    pnt.drawRect (selRect);
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

void BinaryBar::mouseMoveEvent (QMouseEvent* event)
{
  if (stream)  {
    signed int pos_x = event->x ();
    if (pos_x < 0)  {
      currentPos = 0;
    } else if (pos_x > width ())  {
      currentPos = width ();
    } else  {
      currentPos = pos_x;
    }
    setCursor (Qt::ClosedHandCursor);
    repaint ();
  }
}

void BinaryBar::mouseReleaseEvent (QMouseEvent* event)
{
  setCursor (Qt::ArrowCursor);
  if (stream)  {
    emit filePosChanged (positionInFile (currentPos));
  }
}

void BinaryBar::resizeEvent (QResizeEvent* event)
{
  drawColorStrip ();
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
  
  std::vector<char> data;
  try  {
    data = stream->getBlockDataAt (position);
  } catch (EFileException& exc)  {
    stream = NULL;
    emit error ();
  }
  
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
    index_str.setNum (currentColumn, 16);
    QString count_str = "";
    count_str.setNum (deviation[currentColumn]);
    RET = tr ("%2 x Byte 0x%1").arg (index_str).arg (count_str);
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
      QColor colColor;
      
      /* Draw the label */
      if (ii == currentColumn)  {
        colColor = Qt::red;
        pnt.setPen (colColor);
        QRect red_label;
        red_label.setSize (QSize (100, 30));
        if (currentColumn < deviation.size () / 2)  {
          red_label.moveTopLeft (col.topLeft ());
          red_label.setY (0);
        } else  {
          red_label.moveTopRight (col.topRight ());
          red_label.setY (0);
        }
        pnt.drawText (red_label, currentColStr ());
      } else  {
        colColor = Qt::blue;
      }
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
  repaint ();
}

/*****************************************************************************/

FourierSheet::FourierSheet (QWidget *parent)
  : Histogram (parent), fourier_coeffs (0)
{}

FourierSheet::FourierSheet (FourierSheet& other)
  : Histogram (other), fourier_coeffs (other.fourier_coeffs)
{}

FourierSheet::~FourierSheet ()
{}

void FourierSheet::loadData (const uint64_t position)
{
  std::vector<char> data;
  try  {
    data = stream->getBlockDataAt (position);
  } catch (EFileException& exc)  {
    stream = NULL;
    emit error ();
  }
  fourier_coeffs.clear ();
  fourier_coeffs.resize (data.size ());
  
  /* Do the discrete Fourier transform. */
  register unsigned int j, k;
  for (k = 0; k < fourier_coeffs.size (); ++k)  {
    fourier_coeffs[k] = 0.0;
    for (j = 0; j < data.size (); ++j)  {
      double theta = -2.0 * M_PI * j * k / (double) data.size ();
      std::complex<double> z = std::polar (1.0, theta) * (double) data[j];
      fourier_coeffs[k] += z;
    }
  }
  
  if (daughter)  {
    daughter->hide ();
    delete daughter;
    daughter = new FourierSheet (*this);
  }
  
  repaint ();
}

QString FourierSheet::currentColStr ()
{
  QString RET = tr ("No data");
  if (currentColumn != (-1))  {
    QString index_str = "";
    index_str.setNum (currentColumn + 1);
    QString real_str = "";
    real_str.setNum (fourier_coeffs[currentColumn].real ());
    QString imag_str = "";
    imag_str.setNum (fourier_coeffs[currentColumn].imag ());
    RET = tr ("F(f(1/%1)) = %2 + %3i").arg (index_str).arg (real_str).arg (imag_str);
  }
  return RET;
}

void FourierSheet::paintEvent (QPaintEvent* event)
{
  QPainter pnt;
  pnt.begin (this);
  pnt.fillRect (rect (), Qt::white);
  if (fourier_coeffs.size ())  {
    double columnWidth = (double) width () / fourier_coeffs.size ();
    
    /* Search for maximum and minimum value */
    double max = 0.0, min = 0.0;
    register unsigned int ii;
    for (ii = 1; ii < fourier_coeffs.size (); ++ii)  {
      if (fourier_coeffs[ii].real () > max)  {
        max = fourier_coeffs[ii].real ();
      }
      if (fourier_coeffs[ii].imag () > max)  {
        max = fourier_coeffs[ii].imag ();
      }
      
      if (fourier_coeffs[ii].real () < min)  {
        min = fourier_coeffs[ii].real ();
      }
      if (fourier_coeffs[ii].imag () < min)  {
        min = fourier_coeffs[ii].imag ();
      }
    }
    double range = max - min;
    
    /* Draw the X axes (y = 0) */
    pnt.setPen (Qt::black);
    unsigned int y_0 = height () + (min / range) * height ();
    pnt.drawLine (0, y_0, width (), y_0);
    
    /* Draw the Fourier coefficients. */
    for (ii = 1; ii < fourier_coeffs.size (); ++ii)  {
      /* Real part */
      pnt.setPen (Qt::blue);
      unsigned int x_1 = (ii - 1) * width () / fourier_coeffs.size ();
      unsigned int x_2 = ii * width () / fourier_coeffs.size ();
      unsigned int y_1 = height () - (fourier_coeffs[ii - 1].real () - min) * height () / range;
      unsigned int y_2 = height () - (fourier_coeffs[ii].real () - min) * height () / range;
      pnt.drawLine (x_1, y_1, x_2, y_2);
      
      /* Imaginary part */
      pnt.setPen (Qt::green);
      y_1 = height () - (fourier_coeffs[ii - 1].imag () - min) * height () / range;
      y_2 = height () - (fourier_coeffs[ii].imag () - min) * height () / range;
      pnt.drawLine (x_1, y_1, x_2, y_2);
    }
    
    /* Draw the red position line and it's value. */
    unsigned int red_x = currentColumn * width () / fourier_coeffs.size ();
    pnt.setPen (Qt::red);
    QRect red_label;
    red_label.setSize (QSize (200, 30));
    if (red_x < width () / 2)  {
      red_label.moveTopLeft (QPoint (red_x, 0));
    } else  {
      red_label.moveTopRight (QPoint (red_x, 0));
    }
    pnt.drawLine (red_x, 0, red_x, height ());
    pnt.drawText (red_label, currentColStr ());
    
    /* Draw the key */
    pnt.setPen (Qt::blue);
    QRect key_rect;
    key_rect.setSize (QSize (80, 20));
    key_rect.moveTopRight (QPoint (width (), 0));
    pnt.drawText (key_rect, tr ("Real"));
    pnt.setPen (Qt::green);
    key_rect.moveTopRight (QPoint (width (), 20));
    pnt.drawText (key_rect, tr ("Imaginary"));
    pnt.setPen (Qt::black);
    key_rect.setHeight (40);
    key_rect.setWidth (85);
    key_rect.moveTopRight (QPoint (width (), 0));
    pnt.drawRect (key_rect);
  } else  {
    pnt.setPen (Qt::black);
    pnt.drawText (rect (), Qt::AlignCenter, tr ("No data"));
  }
  pnt.end ();
}

void FourierSheet::mousePressEvent (QMouseEvent* event)
{
  if (daughter)  {
    daughter->show ();
  }
}

void FourierSheet::mouseMoveEvent (QMouseEvent* event)
{
  if (fourier_coeffs.size ())  {
    currentColumn = fourier_coeffs.size () * event->x () / width ();
  }
  repaint ();
}