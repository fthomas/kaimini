// Kaimini, a general purpose fitting frontend
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
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>
#include "gslaux.h"

using namespace std;

namespace Kaimini {

vector<double> gsl_to_stl_vector(const gsl_vector* v)
{
  vector<double> retval(v->size);
  for (size_t i = 0; i < v->size; ++i)
  { retval[i] = gsl_vector_get(v, i); }
  return retval;
}


gsl_vector* stl_to_gsl_vector(const vector<double>& v)
{
  gsl_vector* retval = gsl_vector_alloc(v.size());
  for (size_t i = 0; i < v.size(); ++i)
  { gsl_vector_set(retval, i, v[i]); }
  return retval;
}


double gsl_vector_minkowski_dist(void* v1, void* v2, const double p)
{
  gsl_vector* vx = static_cast<gsl_vector*>(v1);
  gsl_vector* vy = static_cast<gsl_vector*>(v2);

  gsl_vector* diff = gsl_vector_alloc(vx->size);
  gsl_vector_memcpy(diff, vx);
  gsl_vector_sub(diff, vy);

  double retval = 0.;
  for (size_t i = 0; i < diff->size; ++i)
  { retval += pow(abs(gsl_vector_get(diff, i)), p); }
  retval = pow(retval, 1./p);

  gsl_vector_free(diff);
  return retval;
}


void gsl_vector_step_random(const gsl_rng* r, void* v, const double step_size)
{
  gsl_vector* v_old = static_cast<gsl_vector*>(v);
  const size_t n = v_old->size;
  gsl_vector* v_new = gsl_vector_alloc(n);

  // Set normal distributed random numbers as elements of v_new and
  // compute the euclidean norm of this vector.
  double length = 0.;
  for (size_t i = 0; i < n; ++i)
  {
    double* v_new_i = gsl_vector_ptr(v_new, i);
    *v_new_i = gsl_ran_ugaussian(r);
    length += pow(*v_new_i, 2);
  }
  length = sqrt(length);

  // Scale v_new so that the elements of v_new are uniformly distributed
  // within an n-sphere of radius step_size.
  const double scale = pow(pow(step_size, static_cast<int>(n))
    * gsl_rng_uniform_pos(r), 1.0/n) / length;
  gsl_vector_scale(v_new, scale);

  gsl_vector_add(v_old, v_new);
}

} // namespace Kaimini

// vim: sw=2 tw=78
