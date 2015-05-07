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
 *  along with Filalyzer.  If not, see <http://www.gnu.org/licenses/>.           *
 ******************************************************************************/

/**
 * \file Filalyzer.hpp
 * \author Daniel Steinhauer
 * \brief This file contains the the main window \c Filalyzer.
 */

#ifndef Filalyzer_H
#define Filalyzer_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QSplitter>
#include <QLabel>
#include <QFileDialog>
#include <QMenu>
#include <QTabWidget>
#include <QPushButton>
#include <QSettings>

#include "custom_comp.hpp"
#include "Hexedit.hpp"
#include "Threading.hpp"
#include "SettingsDialog.hpp"

/**
 * \class Filalyzer
 * \brief The main window derived from QMainWindow.
 * 
 * \details The main window is splitted in three parts:
 * - The binary bar where the whole file is shown on a colorful stripe.
 * - The histogram with a histogram of the bytes in the current block of 1 KiB as well as a Fourier transform.
 * - The editor where the current block can be viewed and edited.
 */
class Filalyzer : public QMainWindow
{
  Q_OBJECT
private:
  /**
   * \brief The global settings for this application.
   */
  QSettings *settings;
  
  /**
   * \brief The current position in the file.
   */
  uint64_t filePosition;
  
  /**
   * \brief Whether the file has been modified in the editor.
   */
  bool modified_file;
  
  /**
   * \brief The file stream to analyze.
   */
  Hexfile *file;
  QString path;
  
  /**
   * \brief The \c BackGroundWorker thread which loads the file in the background.
   */
  BackGroundWorker *streamLoader;
  
  /**
   * \brief The file dialog the user can open files with.
   */
  QFileDialog *fdia;
  
  /**
   * \brief A \c SettingsDialog where the user can make global settings.
   */
  SettingsDialog *setDia;
  
  QMenu *filemenu, *settingsmenu, *helpmenu;
  
  QSplitter *sp_main, *sp_sub;
  
  /* The file overview widget. */
  QWidget *overview_widget;
  QGridLayout *overview_lyt;
  BinaryBar *bar;
  QLabel *l_plain, *l_bin, *l_text, *l_other,*l_pos;
  QPushButton *prev_btn, *next_btn;
  
  /* The histogram widget */
  QWidget *hist_widget;
  QGridLayout *hist_lyt;
  QTabWidget *hist_tabs;
  Histogram *dev_hist, *fourier_hist;
  
  /* The hexedit widget */
  HexWidget *hexw;
public:
  /**
   * \brief The constructor.
   * \param appSettings A pointer to the global settings.
   */
  Filalyzer (QSettings*);
  
  /**
   * \brief The destructor.
   */
  virtual ~Filalyzer ();
  
  /**
   * \brief Ask user whether file should be saved.
   * \return True if user made a decision. False if user clicked 'Cancel'.
   */
  bool askFileSave ();
  
protected:
  /**
   * \brief React to user's key press.
   * 
   * User can navigate through file with [Q] and [W].
   */
  void keyPressEvent (QKeyEvent*);
  
  /**
   * \brief Ask user whether to save file before closing, if file has been modified.
   */
  void closeEvent (QCloseEvent*);
  
public slots:
  /**
   * \brief Pick another KiB in file.
   * \param newPos The new position in file.
   */
  void changeFilePosition (uint64_t);
  
  /**
   * \brief Show the about Qt dialog.
   */
  void showAboutQt ();
  
  /**
   * \brief Open a file.
   * \param filePath The file path to open.
   */
  void openFile (QString);
  
  /**
   * \brief This slot is called, when the file has been edited.
   */
  void fileModified ();
  
  /**
   * \brief Write the changes to file.
   */
  void saveFile ();
  
  /**
   * \brief This slot is called by the \c streamLoader when the file is loaded.
   */
  void fileLoaded ();
  
  /**
   * \brief This slot is called when an \c EFileException has been caught.
   */
  void error ();
  
  /**
   * \brief Pick next KiB in file.
   */
  void prevKiB ();
  
  /**
   * \brief Pick previous KiB in file.
   */
  void nextKiB ();
};

#endif // Filalyzer_H