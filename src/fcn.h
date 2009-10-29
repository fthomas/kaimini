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

template<int n> struct Observables
{
  int use[n];
  double value[n];
  double error[n];
};


class Fcn : public ROOT::Minuit2::FCNBase
{
public:
  ROOT::Minuit2::MnUserParameters upar;

  static const int nObs = 4;
  static Observables<nObs> obs;

  static double chiSquare(const std::vector<double>& par);
  static double chiSquare(const gsl_vector* v, void* params);

  double operator()(const std::vector<double>& par) const
  { return chiSquare(par); }
  double Up() const { return 1.; }

  void setParameters(const Slha& input);
  static void setObservables(const Slha& input);
  void simpleFitMinuit();
  void simpleFitGsl();
};

} // namespace FISP

#endif // FISP_FCN_H

// vim: sw=2 tw=78
