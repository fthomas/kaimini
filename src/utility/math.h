// Kaimini, a general purpose fitting and analysis front end
// Copyright © 2009-2011 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#ifndef KAIMINI_UTILITY_MATH_H
#define KAIMINI_UTILITY_MATH_H

#include <cassert>
#include <cmath>
#include <limits>

namespace Kaimini {
namespace utility {

const long double pi =
  3.141592653589793238462643383279502884197169399375L;  // pi
const long double sqrt2 =
  1.414213562373095048801688724209698078569671875377L;  // sqrt(2)

const long double normal_1sigma =
  0.6826894921370858971704650912640758449558259334532L; // erf(1/sqrt(2))
const long double normal_2sigma =
  0.9544997361036415855994347256669331250564475525966L; // erf(2/sqrt(2))
const long double normal_3sigma =
  0.9973002039367398109466963704648100452443412636832L; // erf(3/sqrt(2))


template<class Float> inline bool
close_to_zero(const Float& x, double factor = 100.)
{
  const Float eps = std::numeric_limits<Float>::epsilon() * factor;
  return std::abs(x) <= eps;
}


template<class Float> inline bool
close_to_one(const Float& x, double factor = 100.)
{ return close_to_zero(1. - x, factor); }


template<class InputIterator> inline double
p_norm(InputIterator first, InputIterator last, double p)
{
  assert(p >= 1.);
  double retval = 0.;
  for (; first != last; ++first)
  {
    retval += std::pow(std::abs(*first), p);
  }
  return std::pow(retval, 1./p);
}


template<class InputIterator> inline double
euclidean_norm(InputIterator first, InputIterator last)
{ return p_norm(first, last, 2.); }

} // namespace utility
} // namespace Kaimini

#endif // KAIMINI_UTILITY_MATH_H
