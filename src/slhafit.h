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

#ifndef KAIMINI_SLHAFIT_H
#define KAIMINI_SLHAFIT_H

#include <vector>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MinosError.h>
#include "genericfit.h"
#include "parameters.h"
#include "slhaea.h"

namespace Kaimini {

class SLHAFit : public GenericFit
{
protected:
  void setDataPoints(const SLHAea::SLHA& input);
  void setParameters(const SLHAea::SLHA& input);

  void readDataPoints(const SLHAea::SLHA& input) const;
  void writeParameters(const std::vector<double>& v,
                       SLHAea::SLHA& output) const;

  void processParams(const Parameters* intPar);
  void processMinimum(const ROOT::Minuit2::FunctionMinimum* intMin);
  void processErrors(const std::vector<ROOT::Minuit2::MinosError>* intErr);
  const SLHAea::SLHA& result();

private:
  std::vector<SLHAea::SLHAKey> mDataPointsKeys;
  std::vector<SLHAea::SLHAKey> mParamsKeys;
  SLHAea::SLHA mResult;
};

} // namespace Kaimini

#endif // KAIMINI_SLHAFIT_H

// vim: sw=2 tw=78
