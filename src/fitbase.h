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

#ifndef FISP_FITBASE_H
#define FISP_FITBASE_H

#include <string>
#include <vector>
#include <gsl/gsl_vector.h>
#include <Minuit2/FCNBase.h>
#include "datapoint.h"
#include "parameters.h"
#include "slhaea.h"

namespace FISP {

class FitBase : public ROOT::Minuit2::FCNBase
{
public:
  virtual void setParameters(const SLHAea::SLHA& input) = 0;
  virtual void setObservables(const SLHAea::SLHA& input) = 0;

  virtual double chiSquare(const std::vector<double>& v) const = 0;
  double chiSquare() const
  { return chiSquare(mPar.getParams()); }
  static double chiSquare(const gsl_vector* v, void*);

  double operator()(const std::vector<double>& v) const
  { return chiSquare(v); }
  double Up() const
  { return 1.; }

  void fitMinuitMinimize();
  void fitMinuitSimplex();
  void fitGslSimplex();

  Parameters getParameters() const
  { return mPar; }
  FitBase& setParameters(const Parameters& par)
  { mPar = par; return *this; }

  SLHAea::SLHA& result();

  virtual std::string getName() const
  { return std::string("FitBase"); }

protected:
  Parameters mPar;
  std::vector<DataPoint> mObs;
  SLHAea::SLHA mResult;

  mutable double mChiSq;
  static FitBase* mspObj;
};

} // namespace FISP

#endif // FISP_FITBASE_H

// vim: sw=2 tw=78
