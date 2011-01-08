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

  BOOST_CHECK_EQUAL(rg1.uniformIntGen()(),  rg2.uniformIntGen()());
  BOOST_CHECK_EQUAL(rg1.uniformRealGen()(), rg2.uniformRealGen()());
  BOOST_CHECK_EQUAL(rg1.normalGen()(),      rg2.normalGen()());
  BOOST_CHECK_EQUAL(rg1.alnumChar(),        rg2.alnumChar());
  BOOST_CHECK_EQUAL(rg1.alnumString(),      rg2.alnumString());
  BOOST_CHECK_EQUAL(gsl_rng_uniform(rg1.gsl_engine),
                    gsl_rng_uniform(rg2.gsl_engine));

  RandomGenerator rg3;
  rg3 = rg1;

  BOOST_CHECK_EQUAL(rg1.uniformIntGen()(),  rg3.uniformIntGen()());
  BOOST_CHECK_EQUAL(rg1.uniformRealGen()(), rg3.uniformRealGen()());
  BOOST_CHECK_EQUAL(rg1.normalGen()(),      rg3.normalGen()());
  BOOST_CHECK_EQUAL(rg1.alnumChar(),        rg3.alnumChar());
  BOOST_CHECK_EQUAL(rg1.alnumString(),      rg3.alnumString());
  BOOST_CHECK_EQUAL(gsl_rng_uniform(rg1.gsl_engine),
                    gsl_rng_uniform(rg3.gsl_engine));

  RandomGenerator rg4 = rg1;

  BOOST_CHECK_EQUAL(rg1.uniformIntGen()(),  rg4.uniformIntGen()());
  BOOST_CHECK_EQUAL(rg1.uniformRealGen()(), rg4.uniformRealGen()());
  BOOST_CHECK_EQUAL(rg1.normalGen()(),      rg4.normalGen()());
  BOOST_CHECK_EQUAL(rg1.alnumChar(),        rg4.alnumChar());
  BOOST_CHECK_EQUAL(rg1.alnumString(),      rg4.alnumString());
  BOOST_CHECK_EQUAL(gsl_rng_uniform(rg1.gsl_engine),
                    gsl_rng_uniform(rg4.gsl_engine));

}

BOOST_AUTO_TEST_CASE(test_seed)
{
  RandomGenerator rg;
  rg.seed(42);

  auto rint_gen = rg.uniformIntGen(0, 99);
  int rint1 = rint_gen();
  int rint2 = rint_gen();
  string rstr1 = rg.alnumString();
  string rstr2 = rg.alnumString();

  rg.seed(42);

  BOOST_CHECK_EQUAL(rint1, rint_gen());
  BOOST_CHECK_EQUAL(rint2, rint_gen());
  BOOST_CHECK_EQUAL(rstr1, rg.alnumString());
  BOOST_CHECK_EQUAL(rstr2, rg.alnumString());
}

BOOST_AUTO_TEST_CASE(test_uniformIntGen)
{
  RandomGenerator rg;
  auto gen09 = rg.uniformIntGen();

  BOOST_CHECK_GE(gen09(), 0);
  BOOST_CHECK_GE(gen09(), 0);
  BOOST_CHECK_GE(gen09(), 0);
  BOOST_CHECK_LE(gen09(), 9);
  BOOST_CHECK_LE(gen09(), 9);
  BOOST_CHECK_LE(gen09(), 9);
}

BOOST_AUTO_TEST_CASE(test_uniformRealGen)
{
  RandomGenerator rg;
  auto gen = rg.uniformRealGen(1., 2.);

  BOOST_CHECK_GE(gen(), 1.);
  BOOST_CHECK_GE(gen(), 1.);
  BOOST_CHECK_GE(gen(), 1.);
  BOOST_CHECK_LT(gen(), 2.);
  BOOST_CHECK_LT(gen(), 2.);
  BOOST_CHECK_LT(gen(), 2.);

  auto gen01 = rg.uniformRealGen();
  BOOST_CHECK_GE(gen01(), 0.);
  BOOST_CHECK_GE(gen01(), 0.);
  BOOST_CHECK_GE(gen01(), 0.);
  BOOST_CHECK_LT(gen01(), 1.);
  BOOST_CHECK_LT(gen01(), 1.);
  BOOST_CHECK_LT(gen01(), 1.);
}

BOOST_AUTO_TEST_CASE(test_uniformInSphereGen)
{
  RandomGenerator rg;
  auto gen = rg.uniformInSphereGen(1, 1.);

  BOOST_CHECK_GE(gen().at(0), -1.);
  BOOST_CHECK_GE(gen().at(0), -1.);
  BOOST_CHECK_GE(gen().at(0), -1.);
  BOOST_CHECK_LE(gen().at(0),  1.);
  BOOST_CHECK_LE(gen().at(0),  1.);
  BOOST_CHECK_LE(gen().at(0),  1.);

  auto gen3 = rg.uniformInSphereGen();
  vector<double> v3;

  v3 = gen3();
  BOOST_CHECK_LE(euclidean_norm(v3.begin(), v3.end()), 1.);
  v3 = gen3();
  BOOST_CHECK_LE(euclidean_norm(v3.begin(), v3.end()), 1.);
  v3 = gen3();
  BOOST_CHECK_LE(euclidean_norm(v3.begin(), v3.end()), 1.);

  auto gen256 = rg.uniformInSphereGen(256, 10.);
  vector<double> v256;

  v256 = gen256();
  BOOST_CHECK_LE(euclidean_norm(v256.begin(), v256.end()), 10.);
  v256 = gen256();
  BOOST_CHECK_LE(euclidean_norm(v256.begin(), v256.end()), 10.);
  v256 = gen256();
  BOOST_CHECK_LE(euclidean_norm(v256.begin(), v256.end()), 10.);

}

BOOST_AUTO_TEST_CASE(test_normalGen)
{
  RandomGenerator rg;
  auto normal = rg.normalGen();

  BOOST_CHECK((boost::math::isfinite)(normal()));
  BOOST_CHECK((boost::math::isfinite)(normal()));
  BOOST_CHECK((boost::math::isfinite)(normal()));

  BOOST_CHECK((boost::math::isfinite)(rg.normalGen(-1.e6, 1.e6)()));
  BOOST_CHECK((boost::math::isfinite)(rg.normalGen(1.e6, 1.e6)()));
  BOOST_CHECK((boost::math::isfinite)(rg.normalGen(0., 1.e-6)()));
}

BOOST_AUTO_TEST_CASE(test_digitChar)
{
  RandomGenerator rg;
  BOOST_CHECK(isdigit(rg.digitChar()));
  BOOST_CHECK(isdigit(rg.digitChar()));
}

BOOST_AUTO_TEST_CASE(test_alphaChar)
{
  RandomGenerator rg;
  BOOST_CHECK(isalpha(rg.alphaChar()));
  BOOST_CHECK(isalpha(rg.alphaChar()));
}

BOOST_AUTO_TEST_CASE(test_alnumChar)
{
  RandomGenerator rg;
  BOOST_CHECK(isalnum(rg.alnumChar()));
  BOOST_CHECK(isalnum(rg.alnumChar()));
}

BOOST_AUTO_TEST_CASE(test_digitString)
{
  RandomGenerator rg;
  const string rstr = rg.digitString();

  for (auto it = rstr.begin(); it != rstr.end(); ++it)
  { BOOST_CHECK(isdigit(*it)); }
}

BOOST_AUTO_TEST_CASE(test_alphaString)
{
  RandomGenerator rg;
  const string rstr = rg.alphaString();

  for (auto it = rstr.begin(); it != rstr.end(); ++it)
  { BOOST_CHECK(isalpha(*it)); }
}

BOOST_AUTO_TEST_CASE(test_alnumString)
{
  RandomGenerator rg;
  const string rstr = rg.alnumString();

  for (auto it = rstr.begin(); it != rstr.end(); ++it)
  { BOOST_CHECK(isalnum(*it)); }
}

BOOST_AUTO_TEST_CASE(test_digitString_templ)
{
  RandomGenerator rg;
  const string templ = "abcXXXXXX";

  BOOST_CHECK_NE(rg.digitString(templ), templ);
  BOOST_CHECK_NE(rg.digitString(templ), templ);

  BOOST_CHECK_EQUAL(rg.digitString(templ).substr(0, 3), templ.substr(0, 3));
  BOOST_CHECK_EQUAL(rg.digitString(templ).substr(0, 3), templ.substr(0, 3));
}

BOOST_AUTO_TEST_CASE(test_alphaString_templ)
{
  RandomGenerator rg;
  const string templ = "abcXXXXXX";

  BOOST_CHECK_NE(rg.alphaString(templ), templ);
  BOOST_CHECK_NE(rg.alphaString(templ), templ);

  BOOST_CHECK_EQUAL(rg.alphaString(templ).substr(0, 3), templ.substr(0, 3));
  BOOST_CHECK_EQUAL(rg.alphaString(templ).substr(0, 3), templ.substr(0, 3));
}

BOOST_AUTO_TEST_CASE(test_alnumString_templ)
{
  RandomGenerator rg;
  const string templ = "abcXXXXXX";

  BOOST_CHECK_NE(rg.alnumString(templ), templ);
  BOOST_CHECK_NE(rg.alnumString(templ), templ);

  BOOST_CHECK_EQUAL(rg.alnumString(templ).substr(0, 3), templ.substr(0, 3));
  BOOST_CHECK_EQUAL(rg.alnumString(templ).substr(0, 3), templ.substr(0, 3));
}

BOOST_AUTO_TEST_SUITE_END()
