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

#ifndef KAIMINI_GENERICDRIVER_H
#define KAIMINI_GENERICDRIVER_H

#include <boost/utility.hpp>
#include "chisqfunction.h"
#include "driver.h"
#include "parameters.h"

namespace Kaimini {

class GenericDriver : public Driver, private boost::noncopyable
{
public:
  explicit GenericDriver(ChiSqFunction* func) : mpFunc(func) {}

  ChiSqFunction* getFunction() const
  {
    return mpFunc;
  }

  Parameters runRandomWalk(const Parameters& startParams);
  Parameters runRandomWalk()
  {
    return runRandomWalk(mpFunc->getParameters());
  }

  Parameters runRandomHillClimbing(const Parameters& startParams);
  Parameters runRandomHillClimbing()
  {
    return runRandomHillClimbing(mpFunc->getParameters());
  }

  Parameters runMetropolis(const Parameters& startParams);
  Parameters runMetropolis()
  {
    return runMetropolis(mpFunc->getParameters());
  }

  Parameters runSimulatedAnnealing(const Parameters& startParams);
  Parameters runSimulatedAnnealing()
  {
    return runSimulatedAnnealing(mpFunc->getParameters());
  }

private:
  ChiSqFunction* mpFunc;
};

} // namespace Kaimini

#endif // KAIMINI_GENERICDRIVER_H
