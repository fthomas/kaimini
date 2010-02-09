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

#ifndef KAIMINI_PARAMETERS_H
#define KAIMINI_PARAMETERS_H

#include <ostream>
#include <vector>
#include <gsl/gsl_vector.h>
#include <Minuit2/MinuitParameter.h>
#include <Minuit2/MnUserParameters.h>

namespace Kaimini {

class Parameters : public ROOT::Minuit2::MnUserParameters
{
public:
  Parameters() {}

  Parameters(const ROOT::Minuit2::MnUserParameters& par)
  { ROOT::Minuit2::MnUserParameters::operator=(par); }

  Parameters& operator=(const ROOT::Minuit2::MnUserParameters& par)
  {
    ROOT::Minuit2::MnUserParameters::operator=(par);
    return *this;
  }

  const std::vector<ROOT::Minuit2::MinuitParameter>&
  getMinuitParameters() const
  { return ROOT::Minuit2::MnUserParameters::Parameters(); }

  std::vector<double> getParams() const
  { return Params(); }

  std::vector<double> getStepSizes() const
  { return Errors(); }

  std::vector<double> getVarParams() const;
  std::vector<double> getVarStepSizes() const;

  gsl_vector* getVarParamsGSLVec() const;
  gsl_vector* getVarStepSizesGSLVec() const;

  Parameters& setVarParams(const std::vector<double>& v);
  Parameters& setVarParams(const gsl_vector* v);
};


std::ostream&
operator<<(std::ostream& os, const ROOT::Minuit2::MinuitParameter& mp);

std::ostream&
operator<<(std::ostream& os, const Parameters& par);

} // namespace Kaimini

#endif // KAIMINI_PARAMETERS_H

// vim: sw=2 tw=78
