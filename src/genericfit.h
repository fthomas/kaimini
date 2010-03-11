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

#ifndef KAIMINI_GENERICFIT_H
#define KAIMINI_GENERICFIT_H

#include <vector>
#include <Minuit2/FCNBase.h>
#include "datapoint.h"
#include "parameters.h"
#include "resultprocessor.h"

namespace Kaimini {

class GenericFit : public ROOT::Minuit2::FCNBase, public ResultProcessor
{
public:
  GenericFit() : mErrorDef(1.) {}

  virtual double chiSquare(const std::vector<double>& v) const = 0;

  double chiSquare(const Parameters& par) const
  { return chiSquare(par.getParams()); }

  double operator()(const std::vector<double>& v) const
  { return chiSquare(v); }

  double Up() const
  { return mErrorDef; }

  void setErrorDef(double up)
  { mErrorDef = up; }

  Parameters getParameters() const
  { return mParams; }

  void setParameters(const Parameters& par)
  {
    mParams = par;
  }

  std::vector<DataPoint> getDataPoints() const
  { return mDataPoints; }

  void setDataPoints(const std::vector<DataPoint>& dataPoints)
  { mDataPoints = dataPoints; }

protected:
  Parameters mParams;
  std::vector<DataPoint> mDataPoints;

private:
  double mErrorDef;
};

} // namespace Kaimini

#endif // KAIMINI_GENERICFIT_H

// vim: sw=2 tw=78
