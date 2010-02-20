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

#include <cmath>
#include <ostream>
#include <string>
#include "kaimini.h"

namespace Kaimini {

struct DataPoint
{
  std::string name;
  bool use;
  double value;
  double error;
  mutable double calcValue;
  mutable double wtSqResidual;

  DataPoint(
    const std::string& _name = "",
    bool _use = false,
    double _value = 0.,
    double _error = 0.)
    : name(_name),
      use(_use),
      value(_value),
      error(_error),
      calcValue(0.),
      wtSqResidual(-1.) {}

  double calcWtSqResidual() const
  {
    wtSqResidual = std::pow(value - calcValue, 2) / std::pow(error, 2);
    return wtSqResidual;
  }

  double randomUniformError(double width)
  {
    error = random_uniform(width);
    return error;
  }

  double randomNormalError(double stddev)
  {
    error = std::abs(random_normal(stddev));
    return error;
  }

  void reset()
  {
    calcValue    =  0.;
    wtSqResidual = -1.;
  }
};


template<typename InputIterator> inline double
sumWtSqResiduals(InputIterator first, InputIterator last)
{
  double sum = 0.;
  for (; first != last; ++first)
  {
    if (first->use) sum += first->calcWtSqResidual();
  }
  return sum;
}


inline std::ostream&
operator<<(std::ostream& os, const DataPoint& dp)
{
  os << "DataPoint:"                             << std::endl
     << "    name         : " << dp.name         << std::endl
     << "    use          : " << dp.use          << std::endl
     << "    value        : " << dp.value        << std::endl
     << "    error        : " << dp.error        << std::endl
     << "    calcValue    : " << dp.calcValue    << std::endl
     << "    wtSqResidual : " << dp.wtSqResidual << std::endl;
  return os;
}

inline std::ostream&
operator<<(std::ostream& os, const std::vector<DataPoint>& dps)
{
  os << "DataPoints:" << std::endl;
  for (std::vector<DataPoint>::const_iterator dp = dps.begin();
       dp != dps.end(); ++dp)
  {
    os << "    - name         : " << dp->name         << std::endl
       << "      use          : " << dp->use          << std::endl
       << "      value        : " << dp->value        << std::endl
       << "      error        : " << dp->error        << std::endl
       << "      calcValue    : " << dp->calcValue    << std::endl
       << "      wtSqResidual : " << dp->wtSqResidual << std::endl
       << std::endl;
  }
  return os;
}

} // namespace Kaimini

#endif // KAIMINI_DATAPOINT_H

// vim: sw=2 tw=78
