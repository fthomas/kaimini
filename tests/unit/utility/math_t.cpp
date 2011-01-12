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

#include <cmath>
#include <vector>
#include <boost/math/special_functions/erf.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>
#include "utility/math.h"

using namespace std;
using namespace boost::math;
using namespace Kaimini::utility;

BOOST_AUTO_TEST_SUITE(test_utility_math)

BOOST_AUTO_TEST_CASE(test_close_to_zero)
{
  BOOST_CHECK(close_to_zero(sqrt(2.) - double(sqrt2)));
  BOOST_CHECK(close_to_zero(acos(-1.) - double(pi)));
}

BOOST_AUTO_TEST_CASE(test_close_to_one)
{
  BOOST_CHECK(close_to_one(erf(1/sqrt(2.)) / double(normal_1sigma)));
  BOOST_CHECK(close_to_one(erf(2/sqrt(2.)) / double(normal_2sigma)));
  BOOST_CHECK(close_to_one(erf(3/sqrt(2.)) / double(normal_3sigma)));
}

BOOST_AUTO_TEST_CASE(test_p_norm)
{
  vector<int> vi;

  BOOST_CHECK_CLOSE(p_norm(vi.begin(), vi.end(), 1.),     0., 1E-4);
  BOOST_CHECK_CLOSE(p_norm(vi.begin(), vi.end(), 2.),     0., 1E-4);
  BOOST_CHECK_CLOSE(euclidean_norm(vi.begin(), vi.end()), 0., 1E-4);

  vi.push_back(3);
  vi.push_back(4);

  BOOST_CHECK_CLOSE(p_norm(vi.begin(), vi.end(), 1.),     7., 1E-4);
  BOOST_CHECK_CLOSE(p_norm(vi.begin(), vi.end(), 2.),     5., 1E-4);
  BOOST_CHECK_CLOSE(euclidean_norm(vi.begin(), vi.end()), 5., 1E-4);

  vi.at(0) = 1;
  vi.at(1) = 1;

  BOOST_CHECK_CLOSE(p_norm(vi.begin(), vi.end(), 2.),     sqrt2, 1E-4);
  BOOST_CHECK_CLOSE(euclidean_norm(vi.begin(), vi.end()), sqrt2, 1E-4);
}

BOOST_AUTO_TEST_SUITE_END()
