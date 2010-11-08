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

#ifndef KAIMINI_SLHAINTERFACE_H
#define KAIMINI_SLHAINTERFACE_H

#include <map>
#include <string>
#include <vector>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MinosError.h>
#include "chisqfunction.h"
#include "error.h"
#include "parameters.h"
#include "slhaea.h"

namespace Kaimini {

class SLHAInterface : public ChiSqFunction
{
public:
  SLHAInterface();

  using ChiSqFunction::setDataPoints;
  using ChiSqFunction::setParameters;

protected:
  void setDataPoints(const SLHAea::Coll& input);
  void setParameters(const SLHAea::Coll& input);

  void readDataPoints(const SLHAea::Coll& input) const;
  void writeParameters(const std::vector<double>& params,
                       SLHAea::Coll& output) const;

  void processDataPointsImpl();

  void processParametersImpl(const Parameters* params);

  void processMinimumImpl(const ROOT::Minuit2::FunctionMinimum* minimum);

  void processErrorsImpl(
          const std::vector<ROOT::Minuit2::MinosError>* errors);

  void processBootstrapImpl(
          const std::vector<std::vector<Error> >* errors,
          unsigned int iterations);

  void processChiSqContribImpl(
          const std::map<int, std::map<int, double> >* single,
          const std::map<int, double>* total);

  void processDriverInfoImpl(const std::map<std::string, std::string>* infos);

  void processRuntimeImpl(double wallTime, double procTime);

  void clearResultsImpl();

  const SLHAea::Coll& result();

private:
  std::vector<std::vector<SLHAea::Key> > mDataPointsKeys;
  std::vector<SLHAea::Key> mParamsKeys;
  SLHAea::Coll mResult;
};

} // namespace Kaimini

#endif // KAIMINI_SLHAINTERFACE_H

// vim: sw=2 tw=78
