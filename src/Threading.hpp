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
 * \brief A thread class that does the data loading for a \c Hexfile and emits signals to \c Filalyzer.
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
   * \brief This signal is emitted to \c Filalyzer when an \c EFileException occured.
   */
  void errorOccured ();
};

#endif