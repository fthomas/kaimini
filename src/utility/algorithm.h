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

#ifndef KAIMINI_UTILITY_ALGORITHM_H
#define KAIMINI_UTILITY_ALGORITHM_H

#include <iterator>

namespace Kaimini {
namespace utility {

template<class InputIterator, class Predicate>
typename std::iterator_traits<InputIterator>::difference_type
count_while(InputIterator first, InputIterator last, Predicate pred)
{
  typename std::iterator_traits<InputIterator>::difference_type n = 0;
  for (; first != last && pred(*first); ++first) ++n;
  return n;
}

} // namespace utility
} // namespace Kaimini

#endif // KAIMINI_UTILITY_ALGORITHM_H
