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

#ifndef KAIMINI_UTILITY_FILESYSTEM_H
#define KAIMINI_UTILITY_FILESYSTEM_H

#include <boost/filesystem.hpp>
#include "kernel/RandomGenerator.h"

namespace Kaimini {
namespace utility {

boost::filesystem::path
temp_path(const boost::filesystem::path& path_templ,
  RandomGenerator& rand_gen);

boost::filesystem::path
create_temp_directory(const boost::filesystem::path& dir_templ,
  RandomGenerator& rand_gen);

} // namespace utility
} // namespace Kaimini

#endif // KAIMINI_UTILITY_FILESYSTEM_H
