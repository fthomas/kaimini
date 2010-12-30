// Kaimini, a general purpose fitting and analysis front end
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
/*
#ifndef KAIMINI_CHISQFUNCTION_H
#define KAIMINI_CHISQFUNCTION_H

#include <vector>
#include <Minuit2/FCNBase.h>
#include "datapoint.h"
#include "parameters.h"
#include "resultprocessor.h"

namespace Kaimini {

class ChiSqFunction : public ROOT::Minuit2::FCNBase, public ResultProcessor
{
public:
  ChiSqFunction() : mErrorDef(1.) {}

  virtual double chiSq(const std::vector<double>& v) const = 0;

  double chiSq(const Parameters& par) const
  {
    return chiSq(par.getParams());
  }

  double operator()(const std::vector<double>& v) const
  {
    return chiSq(v);
  }

  double operator()(const Parameters& par) const
  {
    return chiSq(par.getParams());
  }

  double Up() const
  {
    return mErrorDef;
  }

  double ErrorDef() const
  {
    return mErrorDef;
  }

  void SetErrorDef(double up)
  {
    mErrorDef = up;
  }

  const std::vector<DataPoint>& getDataPoints() const
  {
    return mDataPoints;
  }

  void setDataPoints(const std::vector<DataPoint>& dps)
  {
    mDataPoints = dps;
  }

  const Parameters& getParameters() const
  {
    return mParams;
  }

  void setParameters(const Parameters& par)
  {
    mParams = par;
  }

protected:
  std::vector<DataPoint> mDataPoints;
  Parameters mParams;

private:
  double mErrorDef;
};

} // namespace Kaimini

#endif // KAIMINI_CHISQFUNCTION_H
*/
