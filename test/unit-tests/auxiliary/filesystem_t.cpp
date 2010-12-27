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

#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>
#include "auxiliary/filesystem.h"
#include "kernel/RandomGenerator.h"

using namespace boost::filesystem;
using namespace Kaimini;
using namespace Kaimini::filesystem;

BOOST_AUTO_TEST_SUITE(test_auxiliary_filesystem)

BOOST_AUTO_TEST_CASE(test_create_temp_directory)
{
  RandomGenerator rg;

  path test_path = create_temp_directory("testXXX", rg);
  BOOST_CHECK(exists(test_path));

  remove(test_path);
  BOOST_CHECK(!exists(test_path));
}

BOOST_AUTO_TEST_SUITE_END()
