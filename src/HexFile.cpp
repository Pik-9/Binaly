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
 * \file HexFile.cpp
 * \author Daniel Steinhauer
 */

#include "HexFile.hpp"

#include <string.h>
#include <fstream>
#include <math.h>

using namespace std;

bool Blockinfo::operator== (const Blockinfo& other)
{
  return ((begin == other.begin) && (end == other.end));
}

bool Blockinfo::operator!= (const Blockinfo& other)
{
  return !(*this == other);
}

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
  : parts (0), fsize (0), errorbits (0)
{
  filepath = new char[strlen (file) + 1];
  strcpy (filepath, file);
  filepath[strlen (file)] = 0;
}

Hexfile::~Hexfile ()
{
  delete[] filepath;
}

unsigned int Hexfile::countBlocks ()
{
  return parts.size ();
}

Blockinfo Hexfile::getBlock (const unsigned int block_nr)
{
  if ((block_nr > parts.size ()) || (!parts.size ()))  {
    throw EFilesizeExceeded ();
  }
  return parts[block_nr];
}

void Hexfile::loadFile ()
{
  parts.clear ();
  fstream in;
  in.open (filepath, ios::in);
  if (in.fail ())  {
    throw EReadWriteError ();
  }
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

void Hexfile::fail ()
{
  errorbits = 1;
}

char Hexfile::failStatus ()
{
  return errorbits;
}

Blockinfo Hexfile::getBlockAt (const uint64_t position)
{
  register uint64_t RET_pos = 0;
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
  static Blockinfo lastUsed;
  static vector<char> lastUsedData (0);
  
  Blockinfo bi;
  try  {
    bi = getBlockAt (position);
  } catch (EFilesizeExceeded& exc)  {
    bi = parts[0];
  }

  if ((bi != lastUsed) || (!lastUsedData.size ()))  {
    lastUsedData.clear ();
    fstream in;
    in.open (filepath, ios::in);
    in.seekg (bi.begin);
    for (uint64_t ii = bi.begin; ii < bi.end; ++ii)  {
      lastUsedData.push_back (in.get ());
    }
    in.close ();
    lastUsed = bi;
  }
  return lastUsedData;
}

uint64_t Hexfile::filesize ()
{
  if (!fsize)  {
    throw EFilesizeExceeded ();
  }
  return fsize;
}

void Hexfile::writeBlock (const uint64_t position, vector<char> data)
{
  fstream out;
  out.open (filepath, ios::in | ios::out);
  out.seekp (position);
  for (unsigned int ii = 0; ii < data.size ();  ++ii)  {
    char outC = data[ii];
    out.write (&outC, 1);
  }
  out.close ();
  if (out.fail ())  {
    throw EReadWriteError ();
  }
}