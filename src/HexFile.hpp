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

/**
 * \class EFilesizeExceeded
 * \brief This exception is thrown when the filesize is exceeded.
 */
class EFilesizeExceeded {};

class Hexfile
{
private:
  char *filepath;
  uint64_t fsize;
  std::vector<Blockinfo> parts;
  
public:
  Hexfile (const char*);
  virtual ~Hexfile ();
  
  unsigned int countBlocks ();
  Blockinfo getBlock (const unsigned int);
  
  void loadFile ();
  Blockinfo getBlockAt (const uint64_t);
  std::vector<char> getBlockDataAt (const uint64_t);
  
  uint64_t filesize ();
};

#endif