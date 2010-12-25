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

#include <functional>
#include <string>
#include <boost/test/unit_test.hpp>
#include "auxiliary/functional.h"

using namespace std;
using namespace Kaimini::functional;

BOOST_AUTO_TEST_SUITE(test_auxiliary_functional)

BOOST_AUTO_TEST_CASE(test_iequal_to)
{
  BOOST_CHECK_EQUAL( equal_to<string>()("aaa", "AAA"), false);
  BOOST_CHECK_EQUAL(iequal_to<string>()("aaa", "AAA"), true);
  BOOST_CHECK_EQUAL(iequal_to<string>()("aaa", "aaa"), true);
  BOOST_CHECK_EQUAL(iequal_to<string>()("aaa", "bbb"), false);
}

BOOST_AUTO_TEST_CASE(test_iless)
{
  BOOST_CHECK_EQUAL( less<string>()("AAA", "aaa"), true);
  BOOST_CHECK_EQUAL(iless<string>()("AAA", "aaa"), false);
  BOOST_CHECK_EQUAL(iless<string>()("aaa", "BBB"), true);
  BOOST_CHECK_EQUAL(iless<string>()("AAA", "bbb"), true);
  BOOST_CHECK_EQUAL(iless<string>()("BBB", "aaa"), false);
  BOOST_CHECK_EQUAL(iless<string>()("bbb", "AAA"), false);
}

BOOST_AUTO_TEST_SUITE_END()
