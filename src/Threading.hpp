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
 * \file Threading.hpp
 * \author Daniel Steinhauer
 * \brief This file contains the classes and functions neccessary to execute the loading in a separate thread.
 */

#ifndef BACKGROUND_WORKING_H
#define BACKGROUND_WORKING_H

#include <QThread>

#include "HexFile.hpp"

/**
 * \class BackGroundWorker
 * \brief A thread class that does the data loading for a \c Hexfile and emits signals to \c Binaly.
 */
class BackGroundWorker : public QThread
{
  Q_OBJECT
private:
  /**
   * \var stream
   * \brief The \c Hexfile to load from.
   */
  Hexfile *stream;
  
  /**
   * \brief Do the loading.
   */
  void run ();
  
public:
  /**
   * \brief The constructor.
   */
  BackGroundWorker ();
  
  /**
   * \brief The destructor.
   */
  virtual ~BackGroundWorker ();
  
  /**
   * \brief Set the \c stream variable.
   * \param filestream Pointer to the new \c Hexfile.
   */
  void setFileStream (Hexfile*);
  
signals:
  /**
   * \brief This signal is emitted to \c Binaly when an \c EFileException occured.
   */
  void errorOccured ();
};

#endif