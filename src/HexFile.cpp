#include "HexFile.hpp"

#include <string.h>
#include <fstream>
#include <math.h>

using namespace std;

void Blockinfo::calculateValues (Blockinfo* block, unsigned char* buffer, const uint64_t buffer_length)
{
  uint64_t sum_a = 0;
  uint64_t ii;
  for (ii = 0; ii < buffer_length; ++ii)  {
    sum_a += buffer[ii];
  }
  block->avrg = (double) sum_a / buffer_length;
  
  double sum_s = 0.0;
  for (ii = 0; ii < buffer_length; ++ii)  {
    sum_s += fabs (block->avrg - (double) buffer[ii]);
  }
  block->stddev = sum_s / buffer_length;
}

Hexfile::Hexfile (const char* file)
  : parts (0)
{
  filepath = new char[strlen (file) + 1];
  strcpy (filepath, file);
  filepath[strlen (file)] = 0;
}

Hexfile::~Hexfile ()
{
  delete[] filepath;
}

void Hexfile::loadFile ()
{
  parts.clear ();
  fstream in;
  in.open (filepath, ios::in);
  unsigned char block[1024];
  register uint64_t counter = 0;
  while (in.peek () != (-1))  {
    Blockinfo bi;
    bi.begin = counter;
    in.read ((char*) block, 1024);
    counter += in.gcount ();
    bi.end = counter;
    Blockinfo::calculateValues (&bi, block, in.gcount ());
    parts.push_back (bi);
  }
  in.close ();
  fsize = counter;
}

Blockinfo Hexfile::getBlockAt (const uint64_t position)
{
  uint64_t RET_pos = 0;
  if (position < filesize ())  {
    for (RET_pos = 0; RET_pos < parts.size (); ++RET_pos)  {
      if ((parts[RET_pos].begin <= position) && (parts[RET_pos].end > position))  {
        break;
      }
    }
  } else  {
    throw EFilesizeExceeded ();
  }
  return parts[RET_pos];
}

vector<char> Hexfile::getBlockDataAt (const uint64_t position)
{
  Blockinfo bi = getBlockAt (position);
  vector<char> RET (0);
  fstream in;
  in.open (filepath, ios::in);
  in.seekg (bi.begin);
  for (uint64_t ii = bi.begin; ii < bi.end; ++ii)  {
    RET.push_back (in.get ());
  }
  in.close ();
  return RET;
}

uint64_t Hexfile::filesize ()
{
  return fsize;
}