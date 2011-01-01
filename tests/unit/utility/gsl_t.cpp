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
#include <cstddef>
#include <vector>
#include <gsl/gsl_vector.h>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>
#include "kernel/RandomGenerator.h"
#include "utility/gsl.h"
#include "utility/math.h"

using namespace std;
using namespace Kaimini;
using namespace Kaimini::utility;

BOOST_AUTO_TEST_SUITE(test_utility_gsl)

BOOST_AUTO_TEST_CASE(test_gsl_to_stl_vector)
{
  gsl_vector* gsl_vec = gsl_vector_alloc(3);

  for (size_t i = 0; i < gsl_vec->size; ++i)
  { gsl_vector_set(gsl_vec, i, double(i)); }

  vector<double> stl_vec = gsl_to_stl_vector(gsl_vec);
  BOOST_CHECK_EQUAL(stl_vec[0], 0.0);
  BOOST_CHECK_EQUAL(stl_vec[1], 1.0);
  BOOST_CHECK_EQUAL(stl_vec[2], 2.0);

  gsl_vector_free(gsl_vec);
}

BOOST_AUTO_TEST_CASE(test_stl_to_gsl_vector)
{
  vector<double> stl_vec = {0.0, 1.0, 2.0};
  gsl_vector* gsl_vec = stl_to_gsl_vector(stl_vec);

  BOOST_CHECK_EQUAL(gsl_vector_get(gsl_vec, 0), 0.0);
  BOOST_CHECK_EQUAL(gsl_vector_get(gsl_vec, 1), 1.0);
  BOOST_CHECK_EQUAL(gsl_vector_get(gsl_vec, 2), 2.0);

  gsl_vector_free(gsl_vec);
}

BOOST_AUTO_TEST_CASE(test_gsl_vector_dist)
{
  gsl_vector
    * v1 = gsl_vector_alloc(2),
    * v2 = gsl_vector_alloc(2);

  gsl_vector_set(v1, 0, 0.0);
  gsl_vector_set(v1, 1, 0.0);

  gsl_vector_set(v2, 0, 1.0);
  gsl_vector_set(v2, 1, 0.0);

  BOOST_CHECK_CLOSE(gsl_vector_dist(v1, v2), 1.0, 1.e-3);
  BOOST_CHECK_CLOSE(gsl_vector_dist(static_cast<void*>(v1),
    static_cast<void*>(v2)), 1.0, 1.e-3);

  gsl_vector_set(v1, 0, 0.0);
  gsl_vector_set(v1, 1, 0.0);

  gsl_vector_set(v2, 0, 1.0);
  gsl_vector_set(v2, 1, 1.0);

  BOOST_CHECK_CLOSE(gsl_vector_dist(v1, v2), sqrt2, 1.e-3);
  BOOST_CHECK_CLOSE(gsl_vector_dist(static_cast<void*>(v1),
    static_cast<void*>(v2)), sqrt2, 1.e-3);

  gsl_vector_free(v1);
  gsl_vector_free(v2);

  v1 = gsl_vector_alloc(3);
  v2 = gsl_vector_alloc(3);

  gsl_vector_set(v1, 0, 0.0);
  gsl_vector_set(v1, 1, 0.0);
  gsl_vector_set(v1, 2, 0.0);

  gsl_vector_set(v2, 0, 1.0);
  gsl_vector_set(v2, 1, 1.0);
  gsl_vector_set(v2, 2, 1.0);

  BOOST_CHECK_CLOSE(gsl_vector_minkowski_dist(v1, v2, 3.),
    pow(3., 1./3.), 1.e-3);
  BOOST_CHECK_CLOSE(gsl_vector_minkowski_dist(static_cast<void*>(v1),
    static_cast<void*>(v2), 3.), pow(3., 1./3.), 1.e-3);

  gsl_vector_free(v1);
  gsl_vector_free(v2);
}

BOOST_AUTO_TEST_CASE(test_gsl_vector_step_random)
{
  RandomGenerator rg;
  gsl_vector* v1 = gsl_vector_alloc(1);

  for (int i = 0; i < 10; ++i)
  {
    gsl_vector_set_zero(v1);
    gsl_vector_step_random(rg.gsl_engine, v1, 1.0);

    BOOST_CHECK_GE(gsl_vector_get(v1, 0), -1.0);
    BOOST_CHECK_LE(gsl_vector_get(v1, 0),  1.0);

    gsl_vector_set_zero(v1);
    gsl_vector_step_random(rg.gsl_engine, static_cast<void*>(v1), 1.0);

    BOOST_CHECK_GE(gsl_vector_get(v1, 0), -1.0);
    BOOST_CHECK_LE(gsl_vector_get(v1, 0),  1.0);
  }

  gsl_vector_free(v1);
}

BOOST_AUTO_TEST_SUITE_END()
