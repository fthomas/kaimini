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

#include <cstddef>
#include <vector>
#include <gsl/gsl_vector.h>
#include <boost/test/unit_test.hpp>
#include "auxiliary/gsl.h"

using namespace std;
using namespace Kaimini::gsl;

BOOST_AUTO_TEST_SUITE(test_auxiliary_gsl)

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

BOOST_AUTO_TEST_SUITE_END()
