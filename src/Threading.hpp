#ifndef BACKGROUND_WORKING_H
#define BACKGROUND_WORKING_H

#include <QThread>

#include "HexFile.hpp"

class BackGroundWorker : public QThread
{
  Q_OBJECT
private:
  Hexfile *stream;
  
  void run ();
  
public:
  BackGroundWorker ();
  virtual ~BackGroundWorker ();
  
  void setFileStream (Hexfile*);
  
signals:
  void errorOccured ();
};

#endif