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
#include "kaimini.h"
#include "random.h"

namespace Kaimini {

class DataPoint
{
public:
  DataPoint(
    const std::string& _name = "",
    bool _use = false,
    double _value = 0.,
    double _error = 0.)
    : mName(_name),
      mUse(_use),
      mValue(_value),
      mError(_error),
      mErrorSq(_error * _error),
      mCachedValue(0.) {}

  const std::string& name(const std::string& newName)
  { return mName = newName; }

  const std::string& name() const
  { return mName; }

  bool use(bool useDataPoint)
  { return mUse = useDataPoint; }

  bool use() const
  { return mUse; }

  double value(double newValue)
  { return mValue = newValue; }

  double value() const
  { return mValue; }

  double error(double newError)
  {
    mErrorSq = newError * newError;
    return mError = newError;
  }

  double error() const
  { return mError; }

  double cachedValue(double newCachedValue) const
  { return mCachedValue = newCachedValue; }

  double cachedValue() const
  { return mCachedValue; }

  double wtSqResidual() const
  { return std::pow(mValue - mCachedValue, 2) / mErrorSq; }

  void swapValues()
  { std::swap(mValue, mCachedValue); }

  double smearValueUniform()
  { return mValue += g_rnd.randUniformReal(-mError/2., mError/2.); }

  double smearValueNormal()
  { return mValue += g_rnd.randNormal(mError); }

  double randUniformError(double width)
  { return error(g_rnd.randUniformReal(width)); }

  double randNormalError(double stddev)
  { return error(std::abs(g_rnd.randNormal(stddev))); }

private:
  std::string mName;
  bool mUse;
  double mValue;
  double mError;
  double mErrorSq;
  mutable double mCachedValue;
};


inline double
dps_add_up_wt_sq_residuals(const std::vector<DataPoint>& dps)
{
  double sum = 0.;
  std::vector<DataPoint>::const_iterator dp = dps.begin();
  for (; dp != dps.end(); ++dp) if (dp->use()) sum += dp->wtSqResidual();
  return sum;
}


inline void
dps_swap_values_with_cached_values(std::vector<DataPoint>& dps)
{
  std::vector<DataPoint>::iterator dp = dps.begin();
  for (; dp != dps.end(); ++dp) if (dp->use()) dp->swapValues();
}


inline void
dps_smear_values_normal(std::vector<DataPoint>& dps)
{
  std::vector<DataPoint>::iterator dp = dps.begin();
  for (; dp != dps.end(); ++dp) if (dp->use()) dp->smearValueNormal();
}


inline std::ostream&
operator<<(std::ostream& os, const DataPoint& dp)
{
  os << "DataPoint:"                               << std::endl
     << "    name         : " << dp.name()         << std::endl
     << "    use          : " << dp.use()          << std::endl
     << "    value        : " << dp.value()        << std::endl
     << "    error        : " << dp.error()        << std::endl
     << "    cachedValue  : " << dp.cachedValue()  << std::endl
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
    os << "    - name         : " << dp->name()         << std::endl
       << "      use          : " << dp->use()          << std::endl
       << "      value        : " << dp->value()        << std::endl
       << "      error        : " << dp->error()        << std::endl
       << "      cachedValue  : " << dp->cachedValue()  << std::endl
       << "      wtSqResidual : " << dp->wtSqResidual() << std::endl
       << std::endl;
  }
  return os;
}

} // namespace Kaimini

#endif // KAIMINI_DATAPOINT_H

// vim: sw=2 tw=78
