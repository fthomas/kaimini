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

#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>
#include "gsl.h"

namespace Kaimini {
namespace utility {

std::vector<double>
gsl_to_stl_vector(const gsl_vector* v)
{
  std::vector<double> retval(v->size);
  for (std::size_t i = 0; i < v->size; ++i)
  {
    retval[i] = gsl_vector_get(v, i);
  }
  return retval;
}


gsl_vector*
stl_to_gsl_vector(const std::vector<double>& v)
{
  gsl_vector* retval = gsl_vector_alloc(v.size());
  for (std::size_t i = 0; i < v.size(); ++i)
  {
    gsl_vector_set(retval, i, v[i]);
  }
  return retval;
}


double
gsl_vector_minkowski_dist(const gsl_vector* v1,
                          const gsl_vector* v2, const double p)
{
  assert(p > 0.);

  gsl_vector* diff = gsl_vector_alloc(v1->size);
  gsl_vector_memcpy(diff, v1);
  gsl_vector_sub(diff, v2);

  double retval = 0.;
  for (std::size_t i = 0; i < diff->size; ++i)
  {
    retval += std::pow(std::abs(gsl_vector_get(diff, i)), p);
  }
  retval = std::pow(retval, 1./p);

  gsl_vector_free(diff);
  return retval;
}


void
gsl_vector_step_random(const gsl_rng* r, gsl_vector* v,
                       const double step_size)
{
  assert(step_size > 0.);

  const std::size_t n = v->size;
  gsl_vector* step_vector = gsl_vector_alloc(n);
  double length = 0.;

  for (std::size_t i = 0; i < n; ++i)
  {
    double* step_i = gsl_vector_ptr(step_vector, i);
    *step_i = gsl_ran_ugaussian(r);
    length += pow(*step_i, 2);
  }
  length = sqrt(length);

  const double scale = std::pow(std::pow(step_size, static_cast<int>(n))
    * gsl_rng_uniform_pos(r), 1./n) / length;
  gsl_vector_scale(step_vector, scale);

  gsl_vector_add(v, step_vector);
  gsl_vector_free(step_vector);
}

} // namespace utility
} // namespace Kaimini
