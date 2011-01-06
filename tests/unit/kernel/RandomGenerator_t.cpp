// Kaimini, a general purpose fitting and analysis front end
// Copyright © 2010-2011 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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
#include <vector>
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/test/unit_test.hpp>
#include "kernel/RandomGenerator.h"
#include "utility/math.h"

using namespace std;
using namespace Kaimini;
using namespace Kaimini::utility;

BOOST_AUTO_TEST_SUITE(test_kernel_RandomGenerator)

BOOST_AUTO_TEST_CASE(test_copying)
{
  RandomGenerator rg1;
  RandomGenerator rg2(rg1);

  BOOST_CHECK_EQUAL(rg1.randUniformInt()(),  rg2.randUniformInt()());
  BOOST_CHECK_EQUAL(rg1.randUniformReal()(), rg2.randUniformReal()());
  BOOST_CHECK_EQUAL(rg1.randNormal()(),      rg2.randNormal()());
  BOOST_CHECK_EQUAL(rg1.randAlnumChar(),     rg2.randAlnumChar());
  BOOST_CHECK_EQUAL(rg1.randAlnumString(),   rg2.randAlnumString());
  BOOST_CHECK_EQUAL(gsl_rng_uniform(rg1.gsl_engine),
                    gsl_rng_uniform(rg2.gsl_engine));

  RandomGenerator rg3;
  rg3 = rg1;

  BOOST_CHECK_EQUAL(rg1.randUniformInt()(),  rg3.randUniformInt()());
  BOOST_CHECK_EQUAL(rg1.randUniformReal()(), rg3.randUniformReal()());
  BOOST_CHECK_EQUAL(rg1.randNormal()(),      rg3.randNormal()());
  BOOST_CHECK_EQUAL(rg1.randAlnumChar(),     rg3.randAlnumChar());
  BOOST_CHECK_EQUAL(rg1.randAlnumString(),   rg3.randAlnumString());
  BOOST_CHECK_EQUAL(gsl_rng_uniform(rg1.gsl_engine),
                    gsl_rng_uniform(rg3.gsl_engine));

  RandomGenerator rg4 = rg1;

  BOOST_CHECK_EQUAL(rg1.randUniformInt()(),  rg4.randUniformInt()());
  BOOST_CHECK_EQUAL(rg1.randUniformReal()(), rg4.randUniformReal()());
  BOOST_CHECK_EQUAL(rg1.randNormal()(),      rg4.randNormal()());
  BOOST_CHECK_EQUAL(rg1.randAlnumChar(),     rg4.randAlnumChar());
  BOOST_CHECK_EQUAL(rg1.randAlnumString(),   rg4.randAlnumString());
  BOOST_CHECK_EQUAL(gsl_rng_uniform(rg1.gsl_engine),
                    gsl_rng_uniform(rg4.gsl_engine));

}

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

BOOST_AUTO_TEST_CASE(test_randUniformInt)
{
  RandomGenerator rg;
  auto gen09 = rg.randUniformInt();

  BOOST_CHECK_GE(gen09(), 0);
  BOOST_CHECK_GE(gen09(), 0);
  BOOST_CHECK_GE(gen09(), 0);
  BOOST_CHECK_LE(gen09(), 9);
  BOOST_CHECK_LE(gen09(), 9);
  BOOST_CHECK_LE(gen09(), 9);
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

  auto gen01 = rg.randUniformReal();
  BOOST_CHECK_GE(gen01(), 0.);
  BOOST_CHECK_GE(gen01(), 0.);
  BOOST_CHECK_GE(gen01(), 0.);
  BOOST_CHECK_LT(gen01(), 1.);
  BOOST_CHECK_LT(gen01(), 1.);
  BOOST_CHECK_LT(gen01(), 1.);
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

  auto gen3 = rg.randUniformInSphere();
  vector<double> v3;

  v3 = gen3();
  BOOST_CHECK_LE(euclidean_norm(v3.begin(), v3.end()), 1.);
  v3 = gen3();
  BOOST_CHECK_LE(euclidean_norm(v3.begin(), v3.end()), 1.);
  v3 = gen3();
  BOOST_CHECK_LE(euclidean_norm(v3.begin(), v3.end()), 1.);

  auto gen256 = rg.randUniformInSphere(256, 10.);
  vector<double> v256;

  v256 = gen256();
  BOOST_CHECK_LE(euclidean_norm(v256.begin(), v256.end()), 10.);
  v256 = gen256();
  BOOST_CHECK_LE(euclidean_norm(v256.begin(), v256.end()), 10.);
  v256 = gen256();
  BOOST_CHECK_LE(euclidean_norm(v256.begin(), v256.end()), 10.);

}

BOOST_AUTO_TEST_CASE(test_randNormal)
{
  RandomGenerator rg;
  auto normal = rg.randNormal();

  BOOST_CHECK((boost::math::isfinite)(normal()));
  BOOST_CHECK((boost::math::isfinite)(normal()));
  BOOST_CHECK((boost::math::isfinite)(normal()));

  BOOST_CHECK((boost::math::isfinite)(rg.randNormal(-1.e6, 1.e6)()));
  BOOST_CHECK((boost::math::isfinite)(rg.randNormal(1.e6, 1.e6)()));
  BOOST_CHECK((boost::math::isfinite)(rg.randNormal(0., 1.e-6)()));
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

BOOST_AUTO_TEST_CASE(test_randDigitString_templ)
{
  RandomGenerator rg;
  const string templ = "abcXXXXXX";

  BOOST_CHECK_NE(rg.randDigitString(templ), templ);
  BOOST_CHECK_NE(rg.randDigitString(templ), templ);

  BOOST_CHECK_EQUAL(rg.randDigitString(templ).substr(0, 3), templ.substr(0, 3));
  BOOST_CHECK_EQUAL(rg.randDigitString(templ).substr(0, 3), templ.substr(0, 3));
}

BOOST_AUTO_TEST_CASE(test_randAlphaString_templ)
{
  RandomGenerator rg;
  const string templ = "abcXXXXXX";

  BOOST_CHECK_NE(rg.randAlphaString(templ), templ);
  BOOST_CHECK_NE(rg.randAlphaString(templ), templ);

  BOOST_CHECK_EQUAL(rg.randAlphaString(templ).substr(0, 3), templ.substr(0, 3));
  BOOST_CHECK_EQUAL(rg.randAlphaString(templ).substr(0, 3), templ.substr(0, 3));
}

BOOST_AUTO_TEST_CASE(test_randAlnumString_templ)
{
  RandomGenerator rg;
  const string templ = "abcXXXXXX";

  BOOST_CHECK_NE(rg.randAlnumString(templ), templ);
  BOOST_CHECK_NE(rg.randAlnumString(templ), templ);

  BOOST_CHECK_EQUAL(rg.randAlnumString(templ).substr(0, 3), templ.substr(0, 3));
  BOOST_CHECK_EQUAL(rg.randAlnumString(templ).substr(0, 3), templ.substr(0, 3));
}

BOOST_AUTO_TEST_SUITE_END()
