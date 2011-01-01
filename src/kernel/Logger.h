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

#ifndef KAIMINI_KERNEL_LOGGER_H
#define KAIMINI_KERNEL_LOGGER_H

#include <string>

namespace Kaimini {

class Logger
{
public:
  Logger();

  void
  logMessage(const std::string& message);

  void
  logInfo(const std::string& message);

  void
  logDebug(const std::string& message);

  void
  logWarning(const std::string& message);

  void
  logError(const std::string& message);

  void
  logCritical(const std::string& message);

  int
  verbosityLevel() const
  { return verbosity_; }

  void
  verbosityLevel(int level)
  { verbosity_ = level; }

private:
  int verbosity_;
};

} // namespace Kaimini

#endif // KAIMINI_KERNEL_LOGGER_H
