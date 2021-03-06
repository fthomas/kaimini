Kaimini, a general purpose fitting and analysis front end
=========================================================

Introduction
------------

Documentation
-------------

Currently there is only the `API documentation`_ and a very `incomplete
manual`_.

.. _API documentation: http://www.physik.uni-wuerzburg.de/~fthomas/kaimini/doc/
.. _incomplete manual: http://www.physik.uni-wuerzburg.de/~fthomas/kaimini/kaimini.pdf

Dependencies
------------

Kaimini is written in C++. To build it, the C++ Standard Library, some
parts of the `Boost C++ Libraries`_, `Minuit2`_, the `GNU Scientific
Library (GSL)`_, and `CMake`_ are required. Instructions about
installing these dependencies are available in the `Kaimini wiki`_.

.. _Boost C++ Libraries: http://www.boost.org/
.. _Minuit2: http://www.cern.ch/minuit
.. _GNU Scientific Library (GSL): http://www.gnu.org/software/gsl/
.. _CMake: http://www.cmake.org/
.. _Kaimini wiki: http://wiki.github.com/fthomas/kaimini/installing-dependencies

Installation
------------

To compile Kaimini and to install the ``kaimini`` executable (by
default into ``/usr/local/bin`` on Unix), run these commands in
Kaimini's source tree::

  mkdir build
  cd build
  cmake ..
  make
  sudo make install

If Minuit2 is not installed into standard directories (e.g. under
``/usr/`` or ``/usr/local/``), CMake won't find it. In this case cmake
must be invoked with the full path to the Minuit2 installation or source
directory::

  cmake -D Minuit2_ROOT=/path/to/Minuit2/installation/dir ..

Download
--------

You can download Kaimini in either `tar.gz`_ or `zip`_ formats.

.. _tar.gz: http://github.com/fthomas/kaimini/tarball/master
.. _zip:    http://github.com/fthomas/kaimini/zipball/master

The version control system used for development of Kaimini is Git. The
`Git repository`_ can be inspected and browsed online at GitHub and it
can be cloned by running::

  git clone git://github.com/fthomas/kaimini.git

.. _Git repository: http://github.com/fthomas/kaimini

Issues and feedback
-------------------

For bug reports, feature requests, or general feedback either use the
`issue tracker`_ or `write me an email`_.

.. _issue tracker: http://github.com/fthomas/kaimini/issues
.. _write me an email: fthomas@physik.uni-wuerzburg.de

License
-------

Kaimini is `free software`_ and licensed under the `GPLv3`_. The full
text of the license can be found in the file ``COPYING`` in Kaimini's
source tree.

.. _free software: http://www.gnu.org/philosophy/free-sw.html
.. _GPLv3: http://www.gnu.org/licenses/gpl-3.0.html

Author
------

Kaimini was written by
Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>.
