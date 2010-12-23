// Kaimini, a general purpose fitting and analysis front end
// Copyright © 2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#ifndef KAIMINI_PARAMETERANALYSIS_H
#define KAIMINI_PARAMETERANALYSIS_H

#include <vector>
#include "chisqfunction.h"
#include "driver.h"
#include "error.h"
#include "parameters.h"

namespace Kaimini {

std::vector<std::vector<Error> >
bootstrap(Driver* driver, Driver::minimizer_t minFunc,
          const Parameters& minParams, unsigned int iterations);

void jolt_parameters(ChiSqFunction* chiSqFunc, const Parameters& minParams);

} // namespace Kaimini

#endif // KAIMINI_PARAMETERANALYSIS_H

// vim: sw=2 tw=78
