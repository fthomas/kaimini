// Kaimini, a general purpose fitting and analysis front end
// Copyright © 2010-2011 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#ifndef KAIMINI_UTILITY_STR_TOOLS_H
#define KAIMINI_UTILITY_STR_TOOLS_H

#include <algorithm>
#include <cstddef>
#include <string>

namespace Kaimini {
namespace utility {

template<class CharGenerator> inline std::string
generate_string(std::size_t length, CharGenerator generator)
{
  std::string retval(length, char());
  std::generate(retval.begin(), retval.end(), generator);
  return retval;
}


template<class CharGenerator> inline std::string
generate_string(const std::string& templ, CharGenerator generator,
                char sub = 'X', std::size_t min_length = 6)
{
  std::string retval = templ;

  std::size_t first_sub = retval.find_last_not_of(sub);
  first_sub = (first_sub == std::string::npos) ? 0 : first_sub + 1;
  std::size_t sub_count = retval.length() - first_sub;

  if (sub_count < min_length)
  {
    sub_count = min_length;
    retval.resize(first_sub + sub_count);
  }

  std::generate(retval.begin() + first_sub, retval.end(), generator);
  return retval;
}

} // namespace utility
} // namespace Kaimini

#endif // KAIMINI_UTILITY_STR_TOOLS_H
