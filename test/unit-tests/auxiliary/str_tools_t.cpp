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

#include <boost/test/unit_test.hpp>
#include "auxiliary/str_tools.h"

using namespace std;
using namespace Kaimini::str_tools;

BOOST_AUTO_TEST_SUITE(test_auxiliary_str_tools)

char return_a() { return 'a'; }

BOOST_AUTO_TEST_CASE(test_generate_string)
{
  BOOST_CHECK_EQUAL(generate_string(0, return_a), "");
  BOOST_CHECK_EQUAL(generate_string(1, return_a), "a");
  BOOST_CHECK_EQUAL(generate_string(2, return_a), "aa");
  BOOST_CHECK_EQUAL(generate_string(3, return_a), "aaa");
}

BOOST_AUTO_TEST_SUITE_END()
