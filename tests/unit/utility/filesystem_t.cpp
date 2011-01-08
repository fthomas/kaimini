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
#include "kernel/RandomGenerator.h"
#include "utility/filesystem.h"

using namespace boost::filesystem;
using namespace Kaimini;
using namespace Kaimini::utility;

BOOST_AUTO_TEST_SUITE(test_utility_filesystem)

BOOST_AUTO_TEST_CASE(test_create_temp_directory)
{
  RandomGenerator rg;

  rg.seed(42);
  path test_path1 = create_temp_directory("testXXX", rg);
  rg.seed(42);
  path test_path2 = create_temp_directory("testXXX", rg);
  rg.seed(42);
  path test_path3 = create_temp_directory("testXXX", rg);

  BOOST_CHECK(exists(test_path1));
  BOOST_CHECK(exists(test_path2));
  BOOST_CHECK(exists(test_path3));
  BOOST_CHECK(test_path1 != test_path2);
  BOOST_CHECK(test_path1 != test_path3);
  BOOST_CHECK(test_path2 != test_path3);

  remove(test_path1);
  remove(test_path2);
  remove(test_path3);

  BOOST_CHECK(!exists(test_path1));
  BOOST_CHECK(!exists(test_path2));
  BOOST_CHECK(!exists(test_path3));
}

BOOST_AUTO_TEST_SUITE_END()
