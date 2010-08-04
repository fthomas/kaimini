Kaimini, a general purpose fitting frontend
===========================================

Introduction
------------

Documentation
-------------
  The API documentation can be found [here]
    (http://www.physik.uni-wuerzburg.de/~fthomas/kaimini/doc/).

  And the (at the moment very incomplete) manual can be found [here](
    http://www.physik.uni-wuerzburg.de/~fthomas/kaimini/kaimini.pdf).

Dependencies
------------
  Kaimini is written in C++. To build it, the C++ Standard Library, some
  parts of the Boost C++ Libraries[1], Minuit2[2], the GNU Scientific
  Library (GSL)[3], and CMake[4] are required. Instructions about
  installing these dependencies are available in the Kaimini wiki[5].

  [1] http://www.boost.org/
  [2] http://www.cern.ch/minuit
  [3] http://www.gnu.org/software/gsl/
  [4] http://www.cmake.org/
  [5] http://wiki.github.com/fthomas/kaimini/installing-dependencies

Installation
------------
  To compile Kaimini and to install the 'kaimini' executable (by default
  into /usr/local/bin on Unix), run these commands in Kaimini's source
  tree:
    mkdir build
    cd build
    cmake ..
    make
    sudo make install

  If Minuit2 is not installed into standard directories (e.g. under
  /usr/ or /usr/local/), CMake won't find it. In this case cmake must be
  invoked with the full path to the Minuit2 installation or source
  directory:
    cmake -D Minuit2_ROOT=/path/to/Minuit2/installation/dir ..

Source code
-----------
  For Kaimini's development the Git version control system is used. The
  Git repository can be inspected and browsed online at GitHub:
    http://github.com/fthomas/kaimini

  To clone the repository use this command:
    git clone git://github.com/fthomas/kaimini.git

  The latest source code is also available as tar and ZIP archive:
    http://github.com/fthomas/kaimini/tarball/master (tar.gz)
    http://github.com/fthomas/kaimini/zipball/master (zip)

Issues and feedback:
  For bug reports, feature requests, or general feedback either use the
  issue tracker[6] or write me an email[7].

  [6] http://github.com/fthomas/kaimini/issues
  [7] fthomas@physik.uni-wuerzburg.de

License
-------
  Kaimini is free software[8] and licensed under the GPLv3[9].

  [8] http://www.gnu.org/philosophy/free-sw.html
  [9] http://www.gnu.org/licenses/gpl-3.0.html
      or see the file COPYING in Kaimini's source tree

Author
------
  Kaimini was written by
  Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
