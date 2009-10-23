// RPVFit
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

#ifndef RPVFIT_FCN_H
#define RPVFIT_FCN_H

#include <vector>
#include <Minuit2/FCNBase.h>
#include <Minuit2/MnUserParameters.h>
#include "slha.h"

extern double Fcn_observExp[2];
extern double Fcn_sigma[2];

 
class Fcn : public ROOT::Minuit2::FCNBase
{
public:
  ROOT::Minuit2::MnUserParameters upar;

  double operator()(const std::vector<double>& par) const;
  double Up() const;
  void setUserParameters(const Slha& input);
  void setFixedParameters(const Slha& input) const;
};

#endif // RPVFIT_FCN_H

// vim: sw=2 tw=78
