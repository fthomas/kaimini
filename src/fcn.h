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

#ifndef FISP_FCN_H
#define FISP_FCN_H

#include <vector>
#include <gsl/gsl_vector.h>
#include <Minuit2/FCNBase.h>
#include <Minuit2/MnUserParameters.h>
#include "slha.h"

namespace FISP
{

class Fcn : public ROOT::Minuit2::FCNBase
{
public:
  ROOT::Minuit2::MnUserParameters upar;

  static    int observInclude[4];
  static double observExpected[4];
  static double observSigma[4];

  static double chiSquare(const std::vector<double>& par);
  static double chiSquare(const gsl_vector* v, void* params);

  double operator()(const std::vector<double>& par) const;
  double Up() const;

  void setParameters(const Slha& input);
  static void setObservables(const Slha& input);
};

} // namespace FISP

#endif // FISP_FCN_H

// vim: sw=2 tw=78
