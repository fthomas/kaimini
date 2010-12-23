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

#ifndef KAIMINI_RESULTPROCESSOR_H
#define KAIMINI_RESULTPROCESSOR_H

#include <map>
#include <string>
#include <vector>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MinosError.h>
#include "error.h"
#include "parameters.h"

namespace Kaimini {

class ResultProcessor
{
public:
  ResultProcessor() : mProcess(true), mOverwrite(true) {}

  virtual ~ResultProcessor() {}

  bool processingEnabled() const
  {
    return mProcess;
  }

  void enableProcessing()
  {
    mProcess = true;
  }

  void disableProcessing()
  {
    mProcess = false;
  }

  bool overwritingEnabled() const
  {
    return mOverwrite;
  }

  void enableOverwriting()
  {
    mOverwrite = true;
  }

  void disableOverwriting()
  {
    mOverwrite = false;
  }

  void processDataPoints()
  {
    if (mProcess) processDataPointsImpl();
  }

  void processParameters(const Parameters* params)
  {
    if (mProcess) processParametersImpl(params);
  }

  void processMinimum(const ROOT::Minuit2::FunctionMinimum* minimum)
  {
    if (mProcess) processMinimumImpl(minimum);
  }

  void processErrors(const std::vector<ROOT::Minuit2::MinosError>* errors)
  {
    if (mProcess) processErrorsImpl(errors);
  }

  void processBootstrap(const std::vector<std::vector<Error> >* errors,
                        unsigned int iterations)
  {
    if (mProcess) processBootstrapImpl(errors, iterations);
  }

  void processChiSqContrib(
      const std::map<int, std::map<int, double> >* single,
      const std::map<int, double>* total)
  {
    if (mProcess) processChiSqContribImpl(single, total);
  }

  void processDriverInfo(const std::map<std::string, std::string>* infos)
  {
    if (mProcess) processDriverInfoImpl(infos);
  }

  void processRuntime(double wallTime, double procTime)
  {
    if (mProcess) processRuntimeImpl(wallTime, procTime);
  }

  void clearResults()
  {
    if (mProcess && mOverwrite) clearResultsImpl();
  }

protected:
  virtual void
  processDataPointsImpl() {}

  virtual void
  processParametersImpl(const Parameters*) {}

  virtual void
  processMinimumImpl(const ROOT::Minuit2::FunctionMinimum*) {}

  virtual void
  processErrorsImpl(const std::vector<ROOT::Minuit2::MinosError>*) {}

  virtual void
  processBootstrapImpl(const std::vector<std::vector<Error> >*,
                       unsigned int) {}

  virtual void
  processChiSqContribImpl(const std::map<int, std::map<int, double> >*,
                          const std::map<int, double>*) {}

  virtual void
  processDriverInfoImpl(const std::map<std::string, std::string>*) {}

  virtual void
  processRuntimeImpl(double, double) {}

  virtual void
  clearResultsImpl() {}

private:
  bool mProcess;
  bool mOverwrite;
};

} // namespace Kaimini

#endif // KAIMINI_RESULTPROCESSOR_H
