#ifndef HEX_FILE_H
#define HEX_FILE_H

#include <vector>
#include <stdint.h>

struct Blockinfo
{
  uint64_t begin, end;
  double avrg, stddev;
  
  static void calculateValues (Blockinfo*, unsigned char*, const uint64_t);
};

class Hexfile
{
private:
  char *filepath;
  std::vector<Blockinfo> parts;
  
public:
  Hexfile (const char*);
  virtual ~Hexfile ();
  
  void loadFile ();
  Blockinfo getBlockAt (const uint64_t);
  std::vector<char> getBlockDataAt (const uint64_t);
  
  uint64_t filesize ();
};

#endif