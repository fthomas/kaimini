// Kaimini, a general purpose fitting and analysis front end
// Copyright © 2009-2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#ifndef KAIMINI_AUXILIARY_MATH_H
#define KAIMINI_AUXILIARY_MATH_H

#include <cmath>
#include <limits>

namespace Kaimini {
namespace math {

const long double pi =
  3.1415926535897932384626433832795L;  // pi
const long double sqrt2 =
  1.4142135623730950488016887242097L;  // sqrt(2)

const long double normal_1sigma =
  0.68268949213708589717046509126408L; // erf(1/sqrt(2))
const long double normal_2sigma =
  0.95449973610364158559943472566693L; // erf(2/sqrt(2))
const long double normal_3sigma =
  0.99730020393673981094669637046481L; // erf(3/sqrt(2))


template<class Float> inline bool
close_to_zero(const Float& x, const Float& factor = 100.)
{
  const Float eps = std::numeric_limits<Float>::epsilon() * factor;
  return std::abs(x) <= eps;
}


template<class Float> inline bool
close_to_one(const Float& x, const Float& factor = 100.)
{ return is_close_to_zero(1. - x, factor); }

} // namespace math
} // namespace Kaimini

#endif // KAIMINI_AUXILIARY_MATH_H
