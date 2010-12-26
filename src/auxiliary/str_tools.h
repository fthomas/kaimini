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

#ifndef KAIMINI_AUXILIARY_STR_TOOLS_H
#define KAIMINI_AUXILIARY_STR_TOOLS_H

#include <algorithm>
#include <cstddef>
#include <string>

namespace Kaimini {
namespace str_tools {

template<class CharGenerator> std::string
generate_string(std::size_t length, CharGenerator generator)
{
  std::string retval(length, char());
  std::generate(retval.begin(), retval.end(), generator);
  return retval;
}


template<class CharGenerator> std::string
generate_string(const std::string& templ, CharGenerator generator,
                char sub = 'X', std::size_t min_length = 6)
{
  std::string retval = templ;

  std::size_t pos = retval.find_last_not_of(sub);
  pos = (pos == std::string::npos) ? 0 : pos + 1;
  std::size_t sub_count = retval.length() - pos;

  if (sub_count < min_length)
  {
    sub_count = min_length;
    retval.resize(pos + sub_count);
  }

  std::generate(retval.begin() + pos, retval.end(), generator);
  return retval;
}

} // namespace str_tools
} // namespace Kaimini

#endif // KAIMINI_AUXILIARY_STR_TOOLS_H
