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

#ifndef KAIMINI_GSLAUX_H
#define KAIMINI_GSLAUX_H

#include <vector>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>

namespace Kaimini {

std::vector<double>
gsl_to_stl_vector(const gsl_vector* v);

gsl_vector*
stl_to_gsl_vector(const std::vector<double>& v);

double
gsl_vector_minkowski_dist(const gsl_vector* v1, const gsl_vector* v2,
                          double p);

inline double
gsl_vector_minkowski_dist(void* v1, void* v2, double p)
{
  return gsl_vector_minkowski_dist(static_cast<gsl_vector*>(v1),
                                   static_cast<gsl_vector*>(v2), p);
}

inline double
gsl_vector_dist(const gsl_vector* v1, const gsl_vector* v2)
{
  return gsl_vector_minkowski_dist(v1, v2, 2.);
}

inline double
gsl_vector_dist(void* v1, void* v2)
{
  return gsl_vector_minkowski_dist(v1, v2, 2.);
}

void
gsl_vector_step_random(const gsl_rng* r, gsl_vector* v, double step_size);

inline void
gsl_vector_step_random(const gsl_rng* r, void* v, double step_size)
{
  return gsl_vector_step_random(r, static_cast<gsl_vector*>(v), step_size);
}

} // namespace Kaimini

#endif // KAIMINI_GSLAUX_H

// vim: sw=2 tw=78
