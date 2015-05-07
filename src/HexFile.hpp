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
 * \file HexFile.hpp
 * \author Daniel Steinhauer
 * \brief This file contains all classes and functions to read from and write to file.
 */

#ifndef HEX_FILE_H
#define HEX_FILE_H

#include <vector>
#include <stdint.h>

/**
 * \struct Blockinfo
 * \brief This struct contains information about a KiB of data in the file.
 * ATTENTION: It does not contain the data itself!
 */
struct Blockinfo
{
  /**
   * \brief The data block's start position.
   */
  uint64_t begin;
  
  /**
   * \brief The data block's end position.
   */
  uint64_t end;
  
  /**
   * \brief The average byte value in this data block.
   */
  double avrg;
  
  /**
   * \brief The standard deviation of this block's byte values.
   */
  double stddev;

  /**
   * \brief Only compare the begin and the end value.
   * We're only interested in the position in the file.
   */
  bool operator== (const Blockinfo&);
  
  /**
   * \brief Only compare the begin and the end value.
   * We're only interested in the position in the file.
   */
  bool operator!= (const Blockinfo&);
  
  /**
   * Calculate average and standard deviation of a block of data.
   * \param block A pointer to the \c Blockinfo to write the results to.
   * \param buffer The block of bytes to calculate with.
   * \param buffer_length The number of bytes to analyze.
   */
  static void calculateValues (Blockinfo*, unsigned char*, const uint64_t);
};

/**
 * \struct FilePart
 * \brief A data part of the file.
 */
struct FilePart
{
  /**
   * \brief The position in the file where the block starts.
   */
  uint64_t begin;
  
  /**
   * \brief The data.
   */
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

/**
 * \class Hexfile
 * \brief This class manages the \c Blockinfo structs of the file to analyze.
 */
class Hexfile
{
private:
  char *filepath;
  uint64_t fsize;
  std::vector<Blockinfo> parts;
  char errorbits;
  
public:
  /**
   * \brief The constructor.
   * \param file The file path to the file to analyze.
   */
  Hexfile (const char*);
  
  /**
   * \brief The destructor.
   */
  virtual ~Hexfile ();
  
  /**
   * \brief How many blocks are in this file?
   * \return The size of the \c Blockinfo vector.
   */
  unsigned int countBlocks ();
  
  /**
   * \brief Get the \c Blockinfo struct.
   * \param block_nr The block number, not the file position!
   * \return The \c Blockinfo.
   */
  Blockinfo getBlock (const unsigned int);
  
  /**
   * \brief Load file and create the \c Blockinfo parts.
   * ATTENTION: The \c Blockinfo blocks will not be available until this function is called!
   */
  void loadFile ();
  
  /**
   * \brief Set an error bit.
   * ATTENTION: This has to be done from outside this class.
   */
  void fail ();
  
  /**
   * \brief Get the error bits.
   */
  char failStatus ();
  
  /**
   * \brief Get a \c Blockinfo part at a specific file position.
   * \param position The file position which lies in the wanted \c Blockinfo.
   */
  Blockinfo getBlockAt (const uint64_t);
  
  /**
   * \brief Get the bytes of the block in file at a specific position.
   * \param position The position which lies in the wanted data block.
   */
  std::vector<char> getBlockDataAt (const uint64_t);
  
  /**
   * \brief Get the file size.
   * \return The file size \c fsize.
   */
  uint64_t filesize ();
  
  /**
   * \brief Overwrite the file at a position with given data.
   * \param position The file position to start writing.
   * \param data The vector of bytes to write to file.
   */
  void writeBlock (const uint64_t, std::vector<char>);
};

#endif