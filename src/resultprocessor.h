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

#ifndef KAIMINI_RESULTPROCESSOR_H
#define KAIMINI_RESULTPROCESSOR_H

#include <vector>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MinosError.h>
#include "parameters.h"

namespace Kaimini {

class ResultProcessor
{
public:
  ResultProcessor() : mProcess(true) {}

  virtual ~ResultProcessor() {}

  void enableProcessing()
  { mProcess = true; }

  void disableProcessing()
  { mProcess = false; }

  void processParams(const Parameters* params)
  { if (mProcess) processParamsImpl(params); }

  void processMinimum(const ROOT::Minuit2::FunctionMinimum* minimum)
  { if (mProcess) processMinimumImpl(minimum); }

  void processErrors(const std::vector<ROOT::Minuit2::MinosError>* errors)
  { if (mProcess) processErrorsImpl(errors); }

protected:
  virtual void
  processParamsImpl(const Parameters*) {}

  virtual void
  processMinimumImpl(const ROOT::Minuit2::FunctionMinimum*) {}

  virtual void
  processErrorsImpl(const std::vector<ROOT::Minuit2::MinosError>*) {}

private:
  bool mProcess;
};

} // namespace Kaimini

#endif // KAIMINI_RESULTPROCESSOR_H

// vim: sw=2 tw=78
