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
#include "auxiliary/GSL.h"
//#include "random.h"

using namespace std;
using namespace Kaimini;
using namespace Kaimini::GSL;

BOOST_AUTO_TEST_SUITE(TestGSLAux)

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
    //gsl_vector_step_random(g_rnd.gsl_engine, x, radius);

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
