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
#include <cmath>
#include <functional>
#include <vector>
#include <gsl/gsl_vector.h>
#include "gslaux.h"
#include "random.h"

using namespace std;
using namespace Kaimini;

BOOST_AUTO_TEST_SUITE(TestGSLAux)

BOOST_AUTO_TEST_CASE(test_gsl_vector_minkowski_dist)
{
  // Test with 1D vectors.
  gsl_vector* d1x = gsl_vector_alloc(1);
  gsl_vector* d1y = gsl_vector_alloc(1);

  gsl_vector_set(d1x, 0, 1.);
  gsl_vector_set(d1y, 0, 2.);
  BOOST_CHECK_CLOSE(gsl_vector_dist(d1x, d1y), 1., 1e-6);

  gsl_vector_set(d1x, 0, -1*sqrt(2.));
  gsl_vector_set(d1y, 0, sqrt(8.));
  BOOST_CHECK_CLOSE(gsl_vector_dist(d1x, d1y), 3*sqrt(2.), 1e-6);

  gsl_vector_free(d1x);
  gsl_vector_free(d1y);

  // Test with 2D vectors.
  gsl_vector* d2x = gsl_vector_alloc(2);
  gsl_vector* d2y = gsl_vector_alloc(2);

  gsl_vector_set(d2x, 0, 0.);
  gsl_vector_set(d2x, 1, 0.);
  gsl_vector_set(d2y, 0, 1.);
  gsl_vector_set(d2y, 1, 1.);
  BOOST_CHECK_CLOSE(gsl_vector_dist(d1x, d1y), sqrt(2.), 1e-6);
  BOOST_CHECK_CLOSE(gsl_vector_minkowski_dist(d1x, d1y, 3),
    pow(2., 1./3.), 1e-6);
  BOOST_CHECK_CLOSE(gsl_vector_minkowski_dist(d1x, d1y, 9),
    pow(2., 1./9.), 1e-6);

  gsl_vector_set(d2x, 0, 0.47);
  gsl_vector_set(d2x, 1, -2.3);
  gsl_vector_set(d2y, 0, 19.9);
  gsl_vector_set(d2y, 1, 0.01);
  BOOST_CHECK_CLOSE(gsl_vector_dist(d1x, d1y),
    pow((pow(abs(0.47 - 19.9), 2.0) + pow(abs(-2.3 - 0.01), 2.0)),
      1./2.0), 1e-6);
  BOOST_CHECK_CLOSE(gsl_vector_minkowski_dist(d1x, d1y, 3.5),
    pow((pow(abs(0.47 - 19.9), 3.5) + pow(abs(-2.3 - 0.01), 3.5)),
      1./3.5), 1e-6);
  BOOST_CHECK_CLOSE(gsl_vector_minkowski_dist(d1x, d1y, 90.),
    pow((pow(abs(0.47 - 19.9), 90.) + pow(abs(-2.3 - 0.01), 90.)),
      1./90.), 1e-6);

  gsl_vector_free(d2x);
  gsl_vector_free(d2y);
}

void test_gsl_vector_step_random_helper(int dim, double radius)
{
  const int bins = 5;
  const int n = 5000;
  vector<double> shell(bins, 0.);

  gsl_vector* zero = gsl_vector_alloc(dim);
  gsl_vector* x    = gsl_vector_alloc(dim);
  gsl_vector_set_zero(zero);

  for (int i = 0; i < n; ++i)
  {
    gsl_vector_set_zero(x);
    gsl_vector_step_random(g_rnd.gsl_engine, x, radius);

    for (int j = 0; j < bins; ++j)
    {
      double f = static_cast<double>(j+1) / bins;
      if (gsl_vector_dist(zero, x) < (pow(f, 1./dim) * radius))
      {
        shell[j] += 1.;
        break;
      }
    }
  }

  transform(shell.begin(), shell.end(), shell.begin(),
            bind2nd(divides<double>(), static_cast<double>(n)));

  for (vector<double>::const_iterator it = shell.begin(); it != shell.end();
       ++it)
  { BOOST_CHECK_CLOSE(*it, 1./bins, 10); }

  gsl_vector_free(zero);
  gsl_vector_free(x);
}

BOOST_AUTO_TEST_CASE(test_gsl_vector_step_random)
{
  test_gsl_vector_step_random_helper(2, 1.);
  test_gsl_vector_step_random_helper(3, 2.);
  test_gsl_vector_step_random_helper(4, 3.5);
  test_gsl_vector_step_random_helper(10, 0.02);
  test_gsl_vector_step_random_helper(20, 5.1);
}

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
