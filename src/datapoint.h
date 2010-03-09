// Kaimini, a general purpose fitting frontend
// Copyright © 2009-2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#ifndef KAIMINI_DATAPOINT_H
#define KAIMINI_DATAPOINT_H

#include <algorithm>
#include <cmath>
#include <ostream>
#include <string>
#include <vector>
#include "random.h"

namespace Kaimini {

class DataPoint
{
public:
  explicit DataPoint(
    const std::string& name_ = "",
    bool use_ = false,
    double value_ = 0.,
    double error_ = 0.)
    : mName(name_),
      mUse(use_),
      mValue(value_),
      mError(error_),
      mErrorSq(error_ * error_),
      mCachedValue(0.) {}

  const std::string& name(const std::string& newName)
  {
    return mName = newName;
  }

  const std::string& name() const
  {
    return mName;
  }

  bool use(bool useDataPoint)
  {
    return mUse = useDataPoint;
  }

  bool use() const
  {
    return mUse;
  }

  double value(double newValue)
  {
    return mValue = newValue;
  }

  double value() const
  { return mValue; }

  double error(double newError)
  {
    mErrorSq = newError * newError;
    return mError = newError;
  }

  double error() const
  {
    return mError;
  }

  double cachedValue(double newCachedValue) const
  {
    return mCachedValue = newCachedValue;
  }

  double cachedValue() const
  {
    return mCachedValue;
  }

  double wtSqResidual() const
  {
    return std::pow(mValue - mCachedValue, 2) / mErrorSq;
  }

  void swapValues()
  {
    std::swap(mValue, mCachedValue);
  }

  double smearValueUniform()
  {
    return mValue += g_rnd.randUniformReal(-mError / 2., mError / 2.);
  }

  double smearValueNormal()
  {
    return mValue += g_rnd.randNormal(mError);
  }

  double randErrorUniform(double width)
  {
    return error(g_rnd.randUniformReal(width));
  }

  double randErrorNormal(double stddev)
  {
    return error(std::abs(g_rnd.randNormal(stddev)));
  }

private:
  std::string mName;
  bool mUse;
  double mValue;
  double mError;
  double mErrorSq;
  mutable double mCachedValue;

friend std::ostream& operator<<(std::ostream&, const DataPoint&);
friend std::ostream& operator<<(std::ostream&, const std::vector<DataPoint>&);
};


inline double
dps_add_residuals(const std::vector<DataPoint>& dps)
{
  double sum = 0.;
  for (std::vector<DataPoint>::const_iterator dp = dps.begin();
       dp != dps.end(); ++dp)
  {
    if (dp->use()) sum += dp->wtSqResidual();
  }
  return sum;
}


inline void
dps_swap_values(std::vector<DataPoint>& dps)
{
  for (std::vector<DataPoint>::iterator dp = dps.begin();
       dp != dps.end(); ++dp)
  {
    if (dp->use()) dp->swapValues();
  }
}


inline void
dps_smear_normal(std::vector<DataPoint>& dps)
{
  for (std::vector<DataPoint>::iterator dp = dps.begin();
       dp != dps.end(); ++dp)
  {
    if (dp->use()) dp->smearValueNormal();
  }
}


// stream operators
inline std::ostream&
operator<<(std::ostream& os, const DataPoint& dp)
{
  os << "DataPoint:"                               << std::endl
     << "    name         : " << dp.mName          << std::endl
     << "    use          : " << dp.mUse           << std::endl
     << "    value        : " << dp.mValue         << std::endl
     << "    error        : " << dp.mError         << std::endl
     << "    cachedValue  : " << dp.mCachedValue   << std::endl
     << "    wtSqResidual : " << dp.wtSqResidual() << std::endl;
  return os;
}


inline std::ostream&
operator<<(std::ostream& os, const std::vector<DataPoint>& dps)
{
  os << "DataPoints:" << std::endl;
  for (std::vector<DataPoint>::const_iterator dp = dps.begin();
       dp != dps.end(); ++dp)
  {
    os << "    - name         : " << dp->mName          << std::endl
       << "      use          : " << dp->mUse           << std::endl
       << "      value        : " << dp->mValue         << std::endl
       << "      error        : " << dp->mError         << std::endl
       << "      cachedValue  : " << dp->mCachedValue   << std::endl
       << "      wtSqResidual : " << dp->wtSqResidual() << std::endl
       <<                                                  std::endl;
  }
  return os;
}

} // namespace Kaimini

#endif // KAIMINI_DATAPOINT_H

// vim: sw=2 tw=78
