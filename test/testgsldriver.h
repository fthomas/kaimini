// Kaimini
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
#include <gsl/gsl_vector.h>
#include "gsldriver.h"

using namespace std;
using namespace Kaimini;

BOOST_AUTO_TEST_SUITE(TestGSLDriver)

BOOST_AUTO_TEST_CASE(testDistance)
{
  // Test with 1D vectors.
  gsl_vector* d1x = gsl_vector_alloc(1);
  gsl_vector* d1y = gsl_vector_alloc(1);

  gsl_vector_set(d1x, 0, 1.);
  gsl_vector_set(d1y, 0, 2.);
  BOOST_CHECK_CLOSE(pNormDist(d1x, d1y), 1., 1e-6);

  gsl_vector_set(d1x, 0, -1*sqrt(2.));
  gsl_vector_set(d1y, 0, sqrt(8.));
  BOOST_CHECK_CLOSE(pNormDist(d1x, d1y), 3*sqrt(2.), 1e-6);

  gsl_vector_free(d1x);
  gsl_vector_free(d1y);

  // Test with 2D vectors.
  gsl_vector* d2x = gsl_vector_alloc(2);
  gsl_vector* d2y = gsl_vector_alloc(2);

  gsl_vector_set(d2x, 0, 0.);
  gsl_vector_set(d2x, 1, 0.);
  gsl_vector_set(d2y, 0, 1.);
  gsl_vector_set(d2y, 1, 1.);
  BOOST_CHECK_CLOSE(pNormDist(d1x, d1y), sqrt(2.), 1e-6);
  BOOST_CHECK_CLOSE(pNormDist(d1x, d1y, 3), pow(2., 1./3.), 1e-6);
  BOOST_CHECK_CLOSE(pNormDist(d1x, d1y, 9), pow(2., 1./9.), 1e-6);

  gsl_vector_set(d2x, 0, 0.47);
  gsl_vector_set(d2x, 1, -2.3);
  gsl_vector_set(d2y, 0, 19.9);
  gsl_vector_set(d2y, 1, 0.01);
  BOOST_CHECK_CLOSE(pNormDist(d1x, d1y),
    pow((pow(abs(0.47 - 19.9), 2.0) + pow(abs(-2.3 - 0.01), 2.0)),
      1./2.0), 1e-6);
  BOOST_CHECK_CLOSE(pNormDist(d1x, d1y, 3.5),
    pow((pow(abs(0.47 - 19.9), 3.5) + pow(abs(-2.3 - 0.01), 3.5)),
      1./3.5), 1e-6);
  BOOST_CHECK_CLOSE(pNormDist(d1x, d1y, 90.),
    pow((pow(abs(0.47 - 19.9), 90.) + pow(abs(-2.3 - 0.01), 90.)),
      1./90.), 1e-6);

  gsl_vector_free(d2x);
  gsl_vector_free(d2y);
}

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
