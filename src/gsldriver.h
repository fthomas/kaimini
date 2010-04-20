// Kaimini, a general purpose fitting frontend
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

#ifndef KAIMINI_GSLDRIVER_H
#define KAIMINI_GSLDRIVER_H

#include <gsl/gsl_vector.h>
#include "chisqfunction.h"
#include "parameters.h"

namespace Kaimini {

class GSLDriver
{
public:
  explicit GSLDriver(ChiSqFunction* fit);
  ~GSLDriver();

  static double chiSquare(const gsl_vector* v, void* = 0);

  static double chiSquare(void* v)
  { return chiSquare(static_cast<gsl_vector*>(v)); }

  Parameters runSimplex();
  Parameters runSimulatedAnnealing();

private:
  static ChiSqFunction* mspFit;
  static Parameters msPar;
};

} // namespace Kaimini

#endif // KAIMINI_GSLDRIVER_H

// vim: sw=2 tw=78
