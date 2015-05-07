/******************************************************************************
 *  Filalyzer                                                                 *
 *                                                                            *
 *   (C) 2015 by Daniel Steinhauer <pik-9@users.sourceforge.net>              *
 *                                                                            *
 *                                                                            *
 *  This file is part of Filalyzer.                                           *
 *                                                                            *
 *  Filalyzer is free software: you can redistribute it and/or modify         *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  Filalyzer is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with Filalyzer.  If not, see <http://www.gnu.org/licenses/>.        *
 ******************************************************************************/

/**
 * \file custom_comp.hpp
 * \author Daniel Steinhauer
 * \brief This file contains some custom GUI elements used in this program.
 */

#ifndef CUSTOM_COMPONENTS_H
#define CUSTOM_COMPONENTS_H

#include <QWidget>
#include <QPaintEvent>
#include <QSettings>

#include <vector>
#include <complex>

#include "HexFile.hpp"

/**
 * \class BinaryBar
 * \brief A widget that shows the file content on a stripe with different colors.
 */
class BinaryBar : public QWidget
{
  Q_OBJECT
private:
  /**
   * \var settings
   * \brief The application's global settings.
   */
  QSettings *settings;
  
  /**
   * \var stream
   * \brief The \c Hexfile to load from.
   */
  Hexfile *stream;
  
  /**
   * \var colorStrip
   * \brief The colors of the binary strip are temporarily stored here.
   * So we don't have to calculate them again everytime repaint() is called!
   */
  std::vector<QColor> colorStrip;
  
  /**
   * \var currentPos
   * \brief The current file position.
   */
  unsigned int currentPos;
  
public:
  /**
   * \brief The constructor.
   * \param appSettings The application's global settings.
   * \param parent The parent widget.
   */
  BinaryBar (QSettings*, QWidget *parent = 0);
  
  /**
   * The destructor.
   */
  virtual ~BinaryBar ();
  
  /**
   * \brief Set the \c stream pointer.
   */
  void setFileStream (Hexfile*);
  
  /**
   * \brief Get the file postion from position in widget.
   * \param widget_x The x coordinate to calculate the file position from.
   */
  uint64_t positionInFile (const unsigned int);
  
  /**
   * \brief Set the current position on widget from given file position.
   * \param filepos The file position.
   */
  void setPositionOnBar (const uint64_t);
  
  /**
   * \brief Calculate the temporarily stored color in \c colorStrip.
   */
  void drawColorStrip ();
  
  /**
   * \brief Turn a file position into a human readable statement.
   * \param size The file size/position.
   */
  static QString sizeString (const uint64_t);
  
public:
  /**
   * \var countSections
   * \brief Into how many parts the bar should be subdivided when drawing x axes.
   */
  static unsigned int countSections;
  
protected:
  void paintEvent (QPaintEvent*);
  void mousePressEvent (QMouseEvent*);
  void mouseMoveEvent (QMouseEvent*);
  void mouseReleaseEvent (QMouseEvent*);
  void resizeEvent (QResizeEvent*);
  
signals:
  /**
   * \brief This signal is emitted ot \c Filalyzer when the user changed the file position.
   */
  void filePosChanged (uint64_t);
  
  /**
   * \brief This signal is emitted when an \c EFileException is caught while loading from \c Hexfile.
   */
  void error ();
};

/*****************************************************************************/

/**
 * \class Histogram
 * \brief A widget that shows a histogram of the byte deviation in the current KiB.
 */
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
  /**
   * \brief The constructor
   * \param parent The parent widget.
   */
  Histogram (QWidget *parent = 0);
  
  /**
   * \brief The copy constructor
   * \param other The \c Histogram to copy.
   */
  Histogram (Histogram&);
  
  /**
   * \brief The destructor.
   */
  virtual ~Histogram ();
  
  /**
   * \brief Set the \c Hexfile pointer.
   * \param fstr The new \c Hexfile pointer.
   */
  void setFileStream (Hexfile*);
  
  /**
   * \brief Load block of data from \c Hexfile at a file position and make a histogram.
   * \param position The file position to load the block from.
   */
  virtual void loadData (const uint64_t);
  
  /**
   * \brief Get the description string for a hovered column.
   */
  virtual QString currentColStr ();
  
protected:
  void paintEvent (QPaintEvent*);
  void mousePressEvent (QMouseEvent*);
  void mouseMoveEvent (QMouseEvent*);
  
signals:
  /**
   * \brief This signal is emitted when an \c EFileException is caught while loading from \c Hexfile.
   */
  void error ();
};

/*****************************************************************************/

/**
 * \class FourierSheet
 * \brief A widget that plots the results of a discrete Fourier transform of a data block.
 */
class FourierSheet : public Histogram
{
  Q_OBJECT
protected:
  std::vector<std::complex < double > > fourier_coeffs;

public:
  /**
   * \brief The constructor.
   * \param parent The parent widget.
   */
  FourierSheet (QWidget *parent = 0);
  
  /**
   * \brief The copy constructor.
   * \param other The \c FourierSheet to copy.
   */
  FourierSheet (FourierSheet&);
  
  /**
   * \brief The destructor.
   */
  virtual ~FourierSheet ();
  
  /**
   * \brief Load block of data from \c Hexfile at a position and do a discrete Fourier transform.
   * \param position The file position to load the data from.
   */
  void loadData (const uint64_t);
  
  /**
   * \brief Get the Fourier coefficients at the current graph position.
   */
  QString currentColStr ();
  
protected:
  void paintEvent (QPaintEvent*);
  void mousePressEvent (QMouseEvent*);
  void mouseMoveEvent (QMouseEvent*);
};

#endif