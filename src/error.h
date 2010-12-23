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

#ifndef KAIMINI_ERROR_H
#define KAIMINI_ERROR_H

#include <ostream>
#include <string>
#include <vector>

namespace Kaimini {

class Error
{
public:
  explicit Error(
    int number_ = 0,
    const std::string& name_ = "",
    double mean_ = 0.)
    : mNumber(number_),
      mName(name_),
      mMean(mean_),
      mUpper(mean_),
      mLower(mean_) {}

  Error(
    int number_,
    const std::string& name_,
    double upper_,
    double lower_)
    : mNumber(number_),
      mName(name_),
      mMean((upper_ + lower_) / 2.),
      mUpper(upper_),
      mLower(lower_) {}

  int number(int newNumber)
  {
    return mNumber = newNumber;
  }

  int number() const
  {
    return mNumber;
  }

  const std::string& name(const std::string& newName)
  {
    return mName = newName;
  }

  const std::string& name() const
  {
    return mName;
  }

  double mean(double newMean)
  {
    return mMean = (mLower = (mUpper = newMean));
  }

  double mean() const
  {
    return mMean;
  }

  double upper(double newUpper)
  {
    mMean = (newUpper + mLower) / 2.;
    return mUpper = newUpper;
  }

  double upper() const
  {
    return mUpper;
  }

  double lower(double newLower)
  {
    mMean = (mUpper + newLower) / 2.;
    return mLower = newLower;
  }

  double lower() const
  {
    return mLower;
  }

private:
  int mNumber;
  std::string mName;
  double mMean;
  double mUpper;
  double mLower;

friend std::ostream& operator<<(std::ostream&, const Error&);
friend std::ostream& operator<<(std::ostream&, const std::vector<Error>&);
};


// stream operators
inline std::ostream&
operator<<(std::ostream& os, const Error& error)
{
  os << "Error:"                         << std::endl
     << "    number : " << error.mNumber << std::endl
     << "    name   : " << error.mName   << std::endl
     << "    mean   : " << error.mMean   << std::endl
     << "    upper  : " << error.mUpper  << std::endl
     << "    lower  : " << error.mLower  << std::endl;
  return os;
}


inline std::ostream&
operator<<(std::ostream& os, const std::vector<Error>& errors)
{
  os << "Errors:" << std::endl;
  for (std::vector<Error>::const_iterator error = errors.begin();
       error != errors.end(); ++error)
  {
    os << "    - number : " << error->mNumber << std::endl
       << "      name   : " << error->mName   << std::endl
       << "      mean   : " << error->mMean   << std::endl
       << "      upper  : " << error->mUpper  << std::endl
       << "      lower  : " << error->mLower  << std::endl
       <<                                        std::endl;
  }
  return os;
}

} // namespace Kaimini

#endif // KAIMINI_ERROR_H
