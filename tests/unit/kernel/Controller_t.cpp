// Kaimini, a general purpose fitting and analysis front end
// Copyright © 2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <cstring>
#include <boost/test/unit_test.hpp>
#include "kernel/Controller.h"

using namespace std;
using namespace Kaimini;

struct F
{
  F() : argv_size(10), argv(new char*[argv_size])
  {
    for (int i = 0; i < argv_size; ++i) argv[i] = new char[32];
    strcpy(argv[0], "ut");
  }

  ~F()
  {
    for (int i = 0; i < argv_size; ++i) delete[] argv[i];
    delete[] argv;
  }

  int argv_size;
  char** argv;
};

BOOST_FIXTURE_TEST_SUITE(test_kernel_Controller, F)

BOOST_AUTO_TEST_CASE(test_no_options)
{
  int argc = 1;

  Controller controller;
  controller.initializeKaimini(argc, argv);
  controller.terminateKaimini();
}

BOOST_AUTO_TEST_CASE(test_option_seed)
{
  int argc = 3;
  strcpy(argv[1], "--seed");
  strcpy(argv[2], "42");

  Controller controller;
  controller.initializeKaimini(argc, argv);

  strcpy(argv[1], "-s");
  strcpy(argv[2], "42");

  controller.initializeKaimini(argc, argv);

  strcpy(argv[1], "-s");
  strcpy(argv[2], "fortytwo");

  //controller.initializeKaimini(argc, argv);
  controller.terminateKaimini();
}

BOOST_AUTO_TEST_SUITE_END()
