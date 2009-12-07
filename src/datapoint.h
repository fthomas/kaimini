// FISP - Fitting Interface for SPheno
// Copyright © 2009 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#ifndef FISP_DATAPOINT_H
#define FISP_DATAPOINT_H

#include <cmath>
#include <string>

namespace FISP {

struct DataPoint
{
  std::string name;
  bool use;
  double value;
  double error;
  mutable double calcValue;
  mutable double wSqResidual;

  DataPoint(std::string _name = "", bool _use = false, double _value = 0.,
    double _error = 0.) : name(_name), use(_use), value(_value),
    error(_error), calcValue(0.), wSqResidual(-1.) {}

  double wSquaredResidual() const
  {
    wSqResidual = std::pow(value - calcValue, 2) / std::pow(error, 2);
    return wSqResidual;
  }
};

} // namespace FISP

#endif // FISP

// vim: sw=2 tw=78
