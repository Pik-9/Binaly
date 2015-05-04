#include "Threading.hpp"

BackGroundWorker::BackGroundWorker ()
  : QThread ()
{}

BackGroundWorker::~BackGroundWorker ()
{}

void BackGroundWorker::setFileStream (Hexfile* filestream)
{
  stream = filestream;
}

void BackGroundWorker::run ()
{
  try  {
    stream->loadFile ();
  } catch (EFileException& exc)  {
    emit errorOccured ();
  }
}