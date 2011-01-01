// Kaimini, a general purpose fitting and analysis front end
// Copyright © 2011 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#ifndef KAIMINI_UTILITY_EXIT_EXCEPTION_H
#define KAIMINI_UTILITY_EXIT_EXCEPTION_H

namespace Kaimini {
namespace utility {

struct exit_exception
{
  explicit
  exit_exception(int exit_status) : status(exit_status) {}

  int status;
};

} // namespace utility
} // namespace Kaimini

#endif // KAIMINI_UTILITY_EXIT_EXCEPTION_H
