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

#ifndef KAIMINI_VALUESTORE_PARSERWRITER_H
#define KAIMINI_VALUESTORE_PARSERWRITER_H

#include <string>

namespace Kaimini {

class ParserWriter
{
public:
  ParserWriter() {}

  virtual
  ~ParserWriter() {}

  virtual void
  parseFile(const std::string& filename) = 0;

  virtual void
  writeFile(const std::string& filename) = 0;
};

} // namespace Kaimini

#endif // KAIMINI_VALUESTORE_PARSERWRITER_H
