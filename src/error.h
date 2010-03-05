// Kaimini, a general purpose fitting frontend
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

#ifndef KAIMINI_ERROR_H
#define KAIMINI_ERROR_H

#include <ostream>
#include <string>

namespace Kaimini {

class Error
{
public:
  Error(unsigned int _number, const std::string& _name, double _mean)
    : mNumber(_number), mName(_name), mMean(_mean), mUpper(_mean),
      mLower(_mean) {}

  Error(unsigned int _number, const std::string& _name, double _upper,
        double _lower)
    : mNumber(_number), mName(_name), mMean((_upper + _lower)/2.),
      mUpper(_upper), mLower(_lower) {}

  unsigned int number() const
  { return mNumber; }

  const std::string& name() const
  { return mName; }

  double mean() const
  { return mMean; }

  double upper() const
  { return mUpper; }

  double lower() const
  { return mLower; }

private:
  unsigned int mNumber;
  std::string mName;
  double mMean;
  double mUpper;
  double mLower;
};


inline std::ostream&
operator<<(std::ostream& os, const Error& err)
{
  os << "Error:"                        << std::endl
     << "    number : " << err.number() << std::endl
     << "    name   : " << err.name()   << std::endl
     << "    mean   : " << err.mean()   << std::endl
     << "    upper  : " << err.upper()  << std::endl
     << "    lower  : " << err.lower()  << std::endl;
  return os;
}

} // namespace Kaimini

#endif // KAIMINI_ERROR_H

// vim: sw=2 tw=78
