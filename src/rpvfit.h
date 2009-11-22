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

#ifndef FISP_RPVFIT_H
#define FISP_RPVFIT_H

#include <string>
#include <vector>
#include <gsl/gsl_vector.h>
#include <Minuit2/FCNBase.h>
#include "parameters.h"
#include "slha.h"

namespace FISP {

struct Observable
{
  std::string name;
  bool use;
  double value;
  double error;
  mutable double calcValue;

  /** weighted squared residual: (#value - #calcValue)² / #error² */
  mutable double wSqResidual;
};


class RpvFit : public ROOT::Minuit2::FCNBase
{
public:
  double chiSquare() const { return chiSquare(mPar.getParams()); }
  double chiSquare(const std::vector<double>& v) const;
  static double chiSquare(const gsl_vector* v, void*);

  double operator()(const std::vector<double>& v) const
  { return chiSquare(v); }
  double Up() const { return 1.; }

  Parameters* getParameters() { return &mPar; }
  void setParameters(const Slha& input);
  void setObservables(const Slha& input);
  std::string slhaOutput() const;

  void simpleFitMinuit();
  void simpleFitGsl();

  void simpleMinimizeGsl();

private:
  Parameters mPar;
  mutable double mChiSq;
  std::vector<Observable> mObs;
  static const size_t msObsCnt = 4;
  static RpvFit* mspObj;
};

} // namespace FISP

#endif // FISP_RPVFIT_H

// vim: sw=2 tw=78
