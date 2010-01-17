// Kaimini
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

#ifndef KAIMINI_FITBASE_H
#define KAIMINI_FITBASE_H

#include <vector>
#include <Minuit2/FCNBase.h>
#include "datapoint.h"
#include "parameters.h"

namespace Kaimini {

class FitBase : public ROOT::Minuit2::FCNBase
{
public:
  virtual double chiSquare(const std::vector<double>& v) const = 0;

  double chiSquare() const
  { return chiSquare(mParamsInt.getParams()); }

  double operator()(const std::vector<double>& v) const
  { return chiSquare(v); }

  virtual double Up() const
  { return 1.; }

  Parameters getExtParameters() const
  { return mParamsExt; }

  Parameters getIntParameters() const
  { return mParamsInt; }

  void setExtParameters(const Parameters& extPar)
  {
    mParamsExt = extPar;
    mParamsInt = paramTransformExtToInt(extPar);
  }

  void setIntParameters(const Parameters& intPar)
  {
    mParamsInt = intPar;
    mParamsExt = paramTransformIntToExt(intPar);
  }

protected:
  virtual Parameters
  paramTransformExtToInt(const Parameters& extPar) const
  { return extPar; }

  virtual Parameters
  paramTransformIntToExt(const Parameters& intPar) const
  { return intPar; }

  virtual std::vector<double>
  paramTransformIntToExt(const std::vector<double>& intPar) const
  { return intPar; }

protected:
  Parameters mParamsExt;
  Parameters mParamsInt;

  std::vector<DataPoint> mDataPoints;
  mutable double mChiSq;
};

} // namespace Kaimini

#endif // KAIMINI_FITBASE_H

// vim: sw=2 tw=78
