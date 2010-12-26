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

#include <cctype>
#include <string>
#include <boost/test/unit_test.hpp>
#include "kernel/RandomGenerator.h"

using namespace std;
using namespace Kaimini;

BOOST_AUTO_TEST_SUITE(test_kernel_RandomGenerator)

BOOST_AUTO_TEST_CASE(test_seed)
{
  RandomGenerator rg;
  rg.seed(42);

  auto rint_gen = rg.randUniformInt(0, 99);
  int rint1 = rint_gen();
  int rint2 = rint_gen();
  string rstr1 = rg.randAlnumString();
  string rstr2 = rg.randAlnumString();

  rg.seed(42);

  BOOST_CHECK_EQUAL(rint1, rint_gen());
  BOOST_CHECK_EQUAL(rint2, rint_gen());
  BOOST_CHECK_EQUAL(rstr1, rg.randAlnumString());
  BOOST_CHECK_EQUAL(rstr2, rg.randAlnumString());
}

BOOST_AUTO_TEST_CASE(test_randUniformReal)
{
  RandomGenerator rg;
  auto gen = rg.randUniformReal(1., 2.);

  BOOST_CHECK_GE(gen(), 1.);
  BOOST_CHECK_GE(gen(), 1.);
  BOOST_CHECK_GE(gen(), 1.);
  BOOST_CHECK_LT(gen(), 2.);
  BOOST_CHECK_LT(gen(), 2.);
  BOOST_CHECK_LT(gen(), 2.);
}

BOOST_AUTO_TEST_CASE(test_randUniformInSphere)
{
  RandomGenerator rg;
  auto gen = rg.randUniformInSphere(1, 1.);

  BOOST_CHECK_GE(gen().at(0), -1.);
  BOOST_CHECK_GE(gen().at(0), -1.);
  BOOST_CHECK_GE(gen().at(0), -1.);
  BOOST_CHECK_LE(gen().at(0),  1.);
  BOOST_CHECK_LE(gen().at(0),  1.);
  BOOST_CHECK_LE(gen().at(0),  1.);
}

BOOST_AUTO_TEST_CASE(test_randDigitChar)
{
  RandomGenerator rg;
  BOOST_CHECK(isdigit(rg.randDigitChar()));
  BOOST_CHECK(isdigit(rg.randDigitChar()));
}

BOOST_AUTO_TEST_CASE(test_randAlphaChar)
{
  RandomGenerator rg;
  BOOST_CHECK(isalpha(rg.randAlphaChar()));
  BOOST_CHECK(isalpha(rg.randAlphaChar()));
}

BOOST_AUTO_TEST_CASE(test_randAlnumChar)
{
  RandomGenerator rg;
  BOOST_CHECK(isalnum(rg.randAlnumChar()));
  BOOST_CHECK(isalnum(rg.randAlnumChar()));
}

BOOST_AUTO_TEST_CASE(test_randDigitString)
{
  RandomGenerator rg;
  const string rstr = rg.randDigitString();

  for (auto it = rstr.begin(); it != rstr.end(); ++it)
  { BOOST_CHECK(isdigit(*it)); }
}

BOOST_AUTO_TEST_CASE(test_randAlphaString)
{
  RandomGenerator rg;
  const string rstr = rg.randAlphaString();

  for (auto it = rstr.begin(); it != rstr.end(); ++it)
  { BOOST_CHECK(isalpha(*it)); }
}

BOOST_AUTO_TEST_CASE(test_randAlnumString)
{
  RandomGenerator rg;
  const string rstr = rg.randAlnumString();

  for (auto it = rstr.begin(); it != rstr.end(); ++it)
  { BOOST_CHECK(isalnum(*it)); }
}

BOOST_AUTO_TEST_SUITE_END()
