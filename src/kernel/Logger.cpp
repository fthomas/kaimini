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

#include <iostream>
#include <string>
#include "Logger.h"

namespace Kaimini {

Logger::Logger() : verbosity_(1) {}


void
Logger::logMessage(const std::string& message)
{
  if (verbosityLevel() < 1) return;

  std::cout << message;
}


void
Logger::logInfo(const std::string& message)
{
  if (verbosityLevel() < 2) return;

  std::cout << message;
}

 
void
Logger::logDebug(const std::string& message)
{
  if (verbosityLevel() < 3) return;

  std::cout << message;
}


void
Logger::logWarning(const std::string& message)
{
  if (verbosityLevel() < 1) return;

  std::cerr << message;
}


void
Logger::logError(const std::string& message)
{
  if (verbosityLevel() < 1) return;

  std::cerr << message;
}


void
Logger::logCritical(const std::string& message)
{
  if (verbosityLevel() < 1) return;

  std::cerr << message;
}

} // namespace Kaimini
