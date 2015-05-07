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
 * \file Hexedit.hpp
 * \author Daniel Steinhauer
 * \brief The widget which provides the hex editor in \c Binaly.
 */

#ifndef HEXEDIT_H
#define HEXEDIT_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QGridLayout>

#include "HexFile.hpp"

/**
 * \class HexWidget
 * \brief The widget that can view and edit a KiB of data.
 */
class HexWidget : public QWidget
{
  Q_OBJECT
private:
  QComboBox *c_encoding;
  QTableWidget *hex_table;
  QGridLayout *lyt;
  
  /**
   * \brief The \c Hexfile to read from and write to.
   */
  Hexfile *stream;
  
  /**
   * \brief The index of the currently loaded block of data.
   */
  unsigned int currentIndex;
  
  /**
   * \brief Whether changes in the table widget are done by computer or user.
   * \details A kind of lock for reacting on cell changes in the table widget.
   * If true: The computer doesn't access the table cells now. It must be
   * a user action, which we should react to.
   * If false: The computer has to write to cells. Our \c editCell method
   * shouldn't do anything.
   */
  bool table_printed;
  
  /**
   * \brief The edited blocks of data.
   */
  std::vector<FilePart> customData;
  
  /**
   * \brief These are the bases our byte numbers can be shown to.
   * \details The available bases are:
   * binary (2), octal (8), decimal (10) and hexadecimal (16)
   */
  static const unsigned int bases[4];
  
public:
  /**
   * \brief The constructor
   * \param parent The parent widget.
   */
  HexWidget (QWidget *parent = 0);
  
  /**
   * \brief The destructor.
   */
  virtual ~HexWidget ();
  
  /**
   * \brief If a character is printable return it, otherwise '.'.
   * \param bare the character to check.
   */
  static QChar printableMatch (const char);
  
  /**
   * \brief Set the \c stream pointer.
   * \param filestream A pointer to the new \c Hexfile.
   */
  void setFileStream (Hexfile*);
  
  /**
   * \brief Load block of data from \c stream.
   * \param position The file position to load the block from.
   */
  void loadData (const uint64_t);
  
  /**
   * \brief Write all changes to file.
   */
  void saveChanges ();
  
  /**
   * \brief Write the data into the QTableWidget.
   * \details ATTENTION: Make sure to lock the \c table_printed lock before calling this function!
   * Otherwise the program will crash!
   */
  void printData ();
  
public slots:
  /**
   * \brief Reload the table widget after the user has changed the byte encoding.
   */
  void encodingChanged (int);
  
  /**
   * \brief This function is called when the user modified data in the table widget.
   * It only does anything if \c table_printed is unlocked.
   */
  void editCell (QTableWidgetItem*);
  
signals:
  /**
   * \brief This signal is emitted to the \c Binaly main window when the file has been modified.
   */
  void fileChanged ();
};

#endif