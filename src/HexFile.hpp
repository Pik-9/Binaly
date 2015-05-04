#ifndef HEX_FILE_H
#define HEX_FILE_H

#include <vector>
#include <stdint.h>

struct Blockinfo
{
  uint64_t begin, end;
  double avrg, stddev;

  /**
   * \brief Only compare the begin and the end value.
   * We're only interested in the position in the file.
   */
  bool operator== (const Blockinfo&);
  bool operator!= (const Blockinfo&);
  
  static void calculateValues (Blockinfo*, unsigned char*, const uint64_t);
};

struct FilePart
{
  uint64_t begin;
  std::vector<char> data;
};

/**
 * \class EFileException
 * \brief A base class for all file related errors.
 */
class EFileException {};

/**
 * \class EFilesizeExceeded
 * \brief This exception is thrown when the filesize is exceeded.
 */
class EFilesizeExceeded : public EFileException {};

/**
 * \class EReadWriteError
 * \brief This exception is thrown when the file can't be accessed for any reason.
 */
class EReadWriteError : public EFileException {};

class Hexfile
{
private:
  char *filepath;
  uint64_t fsize;
  std::vector<Blockinfo> parts;
  char errorbits;
  
public:
  Hexfile (const char*);
  virtual ~Hexfile ();
  
  unsigned int countBlocks ();
  Blockinfo getBlock (const unsigned int);
  
  void loadFile ();
  
  void fail ();
  char failStatus ();
  
  Blockinfo getBlockAt (const uint64_t);
  std::vector<char> getBlockDataAt (const uint64_t);
  
  uint64_t filesize ();
  
  void writeBlock (const uint64_t, std::vector<char>);
};

#endif