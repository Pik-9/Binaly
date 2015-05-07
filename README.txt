Binaly
======

Binaly is a binary/hex editor which shows the whole file on a colorful strip.
It subdivides the file into blocks of 1 Kib (1024 Bytes) and analyses them. It
knows the following kinds of data sections:
  - Random binary data:      The byte's average value is in [108, 148] and the
                             standard deviation is in [60, 68].
  - Text data:               The average value is > 64 and the std. dev. < 40.
  - Homogeneous data:        The std. dev. is < 2.
  - Other data section:      The block doesn't fit into any of the categories
                             above.

This is very useful to get a quick overview over the file and its content.


License
-------

(C) 2015 by Daniel Steinhauer <pik-9@users.sourceforge.net>

Binaly is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Binaly is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Binaly.  If not, see <http://www.gnu.org/licenses/>.


Compiling and installing
------------------------

Requirements: You need to have the Qt4 or Qt5 development files and CMake
installed.
Binaly uses CMake as build system. So you can type (on Unix):
  $ mkdir build && cd build
  $ cmake .. && make
  $ sudo make install

There is a detailed doxygen documentation. You can build it with
  $ make doc
after cmake has finished.


Note for translators
--------------------

If there is no translation for your language for Binaly yet and you want to
contribute one, you can do the following:
  1) Determine your 2 letter country code (de for German, en for English, etc.)
  2) Navigate to the Binaly directory and run:
      $ lupdate src/*.cpp -ts lang/Binaly_XX.ts
     XX is your country code.
     Note: The command "lupdate" might be different on other platforms.
  3) Run the Qt Linguist and translate.
  4) Surrender your translation. This can either be done with a git pull request
     or you can just contact me at pik-9@users.sourceforge.net

Daniel Steinhauer