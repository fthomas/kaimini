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

#include <algorithm>
#include <functional>
#include <vector>
#include <boost/test/unit_test.hpp>
#include "utility/algorithm.h"

using namespace std;
using namespace Kaimini::utility;

BOOST_AUTO_TEST_SUITE(test_utility_algorithm)

BOOST_AUTO_TEST_CASE(test_count_while)
{
  vector<int> vec;
  for (int i = 1;  i <= 10; ++i) vec.push_back(i);
  for (int i = 10; i >=  1; --i) vec.push_back(i);

  BOOST_CHECK_EQUAL(count_if(   vec.begin(), vec.end(),
    bind2nd(less_equal<int>(), 5)), 10);
  BOOST_CHECK_EQUAL(count_while(vec.begin(), vec.end(),
    bind2nd(less_equal<int>(), 5)),  5);

  BOOST_CHECK_EQUAL(count_if(   vec.begin(), vec.end(),
    bind2nd(greater<int>(), 5)), 10);
  BOOST_CHECK_EQUAL(count_while(vec.begin(), vec.end(),
    bind2nd(greater<int>(), 5)),  0);

  BOOST_CHECK_EQUAL(count_if(   vec.begin(), vec.end(),
    bind2nd(greater<int>(), 0)), 20);
  BOOST_CHECK_EQUAL(count_while(vec.begin(), vec.end(),
    bind2nd(greater<int>(), 0)), 20);
}

BOOST_AUTO_TEST_SUITE_END()
