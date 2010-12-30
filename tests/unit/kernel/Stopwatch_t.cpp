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
#include "kernel/Stopwatch.h"

using namespace std;
using namespace Kaimini;

BOOST_AUTO_TEST_SUITE(test_kernel_Stopwatch)

BOOST_AUTO_TEST_CASE(test_normal_use)
{
  Stopwatch sw;

  BOOST_CHECK_EQUAL(sw.running(), false);
  BOOST_CHECK_EQUAL(sw.user(),   0.);
  BOOST_CHECK_EQUAL(sw.system(), 0.);
  BOOST_CHECK_EQUAL(sw.real(),   0.);
  BOOST_CHECK_EQUAL(sw.wall(),   0.);

  sw.start();

  BOOST_CHECK_EQUAL(sw.running(), true);
  BOOST_CHECK_GE(sw.user(),   0.);
  BOOST_CHECK_GE(sw.system(), 0.);
  BOOST_CHECK_GE(sw.real(),   0.);
  BOOST_CHECK_GE(sw.wall(),   0.);

  sw.stop();

  BOOST_CHECK_EQUAL(sw.running(), false);
  BOOST_CHECK_GE(sw.user(),   0.);
  BOOST_CHECK_GE(sw.system(), 0.);
  BOOST_CHECK_GE(sw.real(),   0.);
  BOOST_CHECK_GE(sw.wall(),   0.);

  sw.reset();

  BOOST_CHECK_EQUAL(sw.running(), false);
  BOOST_CHECK_EQUAL(sw.user(),   0.);
  BOOST_CHECK_EQUAL(sw.system(), 0.);
  BOOST_CHECK_EQUAL(sw.real(),   0.);
  BOOST_CHECK_EQUAL(sw.wall(),   0.);
}

BOOST_AUTO_TEST_SUITE_END()
