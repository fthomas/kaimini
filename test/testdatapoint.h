// Kaimini, a general purpose fitting and analysis front end
// Copyright © 2009-2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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
#include "datapoint.h"

using namespace std;
using namespace Kaimini;

BOOST_AUTO_TEST_SUITE(TestDataPoint)

BOOST_AUTO_TEST_CASE(testRandomNormalError)
{
  // The three-sigma rule (or the 68-95-99.7 rule) holds,
  // if the random errors are normally distributed:
  DataPoint dp;
  int lt1 = 0, lt2 = 0, lt3 = 0;

  for (int i = 0; i < 10000; ++i)
  {
    double error = abs(dp.randomNormalError(1.));
    if      (error < 1.) ++lt1;
    else if (error < 2.) ++lt2;
    else if (error < 3.) ++lt3;
  }
  BOOST_CHECK(6720 < lt1         && lt1         <  6920);
  BOOST_CHECK(9440 < lt1+lt2     && lt1+lt2     <  9640);
  BOOST_CHECK(9870 < lt1+lt2+lt3 && lt1+lt2+lt3 <= 10000);
}

BOOST_AUTO_TEST_CASE(testRandomUniformError)
{
  // Rough testing for uniform distribution.
  DataPoint dp;
  int lt1 = 0, lt2 = 0, lt3 = 0;

  for (int i = 0; i < 1000; ++i)
  {
    double error = dp.randomUniformError(1.);
    if      (error < 1./3.) ++lt1;
    else if (error < 2./3.) ++lt2;
    else if (error < 3./3.) ++lt3;
  }
  BOOST_CHECK(233 <= lt1 && lt1 < 433);
  BOOST_CHECK(233 <= lt2 && lt2 < 433);
  BOOST_CHECK(233 <= lt3 && lt3 < 433);
}

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
