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
/*
#ifndef KAIMINI_GSLDRIVER_H
#define KAIMINI_GSLDRIVER_H

#include <boost/utility.hpp>
#include <gsl/gsl_vector.h>
#include "chisqfunction.h"
#include "driver.h"
#include "parameters.h"

namespace Kaimini {

class GSLDriver : public Driver, private boost::noncopyable
{
public:
  explicit GSLDriver(ChiSqFunction* func)
  {
    mspFunc = func;
    msPar   = func->getParameters();
  }

  ~GSLDriver()
  {
    mspFunc = 0;
    msPar   = Parameters();
  }

  ChiSqFunction* getFunction() const
  {
    return mspFunc;
  }

  static double chiSquare(const gsl_vector* v, void* = 0);
  static double chiSquare(void* v)
  {
    return chiSquare(static_cast<gsl_vector*>(v));
  }

  Parameters runSimplex(const Parameters& startParams);
  Parameters runSimplex()
  {
    return runSimplex(mspFunc->getParameters());
  }

  Parameters runSimulatedAnnealing(const Parameters& startParams);
  Parameters runSimulatedAnnealing()
  {
    return runSimulatedAnnealing(mspFunc->getParameters());
  }

private:
  void sanitize(const gsl_vector* v);

private:
  static ChiSqFunction* mspFunc;
  static Parameters msPar;
};

} // namespace Kaimini

#endif // KAIMINI_GSLDRIVER_H
*/
