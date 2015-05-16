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
 * \file About.hpp
 * \author Daniel Steinhauer
 */

#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

/**
 * \class AboutDialog
 * \brief A Dialog which shows a short description of Binaly.
 */
class AboutDialog : public QWidget
{
  Q_OBJECT
private:
  QGridLayout *lyt;
  QLabel *l_icon, *l_desc;
  QGroupBox *g_desc;
  QHBoxLayout *lyt_desc;
  QPushButton *btn_ok;
  
public:
  /**
   * \brief The constructor.
   * \param parent The parent widget.
   */
  AboutDialog (QWidget *parent = 0);
  
  /**
   * \brief The destructor.
   */
  virtual ~AboutDialog ();
};

#endif