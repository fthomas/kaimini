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

#include <string>
#include "kaimini.h"

using namespace std;
using namespace Kaimini;

BOOST_AUTO_TEST_SUITE(TestKaimini)

BOOST_AUTO_TEST_CASE(test_random_string)
{
  for (size_t i = 0; i < 100; ++i)
  {
    BOOST_CHECK(random_string(i).length() == i);
    BOOST_CHECK(random_string() != random_string());
  }
}

BOOST_AUTO_TEST_CASE(test_parse_error_string)
{
  double value  = 1.;
  string errstr = "0.5";
  BOOST_CHECK_CLOSE(parse_error_string(value, errstr), 0.5, 1e-6);

  errstr = "5%";
  BOOST_CHECK_CLOSE(parse_error_string(value, errstr), 0.05, 1e-6);

  errstr = "110%";
  BOOST_CHECK_CLOSE(parse_error_string(value, errstr), 1.1, 1e-6);

  errstr = "uniform:0.25";
  BOOST_CHECK_LE(parse_error_string(value, errstr), 0.25);
  BOOST_CHECK_LE(0., parse_error_string(value, errstr));

  errstr = "uniform:50%";
  BOOST_CHECK_LE(parse_error_string(value, errstr), 0.50);
  BOOST_CHECK_LE(0., parse_error_string(value, errstr));

  errstr = "normal:0.25";
  BOOST_CHECK_LE(parse_error_string(value, errstr), 4.*0.25);
  BOOST_CHECK_LE(0., parse_error_string(value, errstr));

  errstr = "normal:50%";
  BOOST_CHECK_LE(parse_error_string(value, errstr), 4.*0.50);
  BOOST_CHECK_LE(0., parse_error_string(value, errstr));
}

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
