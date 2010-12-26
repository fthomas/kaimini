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

#include <boost/filesystem.hpp>
#include "kernel/RandomGenerator.h"
#include "filesystem.h"

namespace Kaimini {
namespace filesystem {

namespace fs = boost::filesystem;

fs::path
temp_path(const fs::path& path_templ, RandomGenerator& rg)
{
  return path_templ.branch_path() / rg.randAlnumString(path_templ.leaf());
}


fs::path
create_temp_directory(const fs::path& dir_templ, RandomGenerator& rg)
{
  fs::path temp_dir;
  do temp_dir = temp_path(dir_templ, rg);
  while (fs::exists(temp_dir));

  fs::create_directory(temp_dir);
  return temp_dir;
}

} // namespace filesystem
} // namespace Kaimini
