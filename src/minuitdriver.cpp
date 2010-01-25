// Kaimini
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

#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MnMinimize.h>
#include <Minuit2/MnSimplex.h>
#include "minuitdriver.h"

using namespace ROOT::Minuit2;

namespace Kaimini {

FunctionMinimum MinuitDriver::runMinimize(unsigned int strategy)
{
  MnMinimize minimizer(*mpFit, mpFit->mParamsInt, strategy);
  FunctionMinimum minimum = minimizer();

  mpFit->chiSquare(minimum.UserParameters().Params());
  return minimum;
}


FunctionMinimum MinuitDriver::runSimplex(unsigned int strategy)
{
  MnSimplex minimizer(*mpFit, mpFit->mParamsInt, strategy);
  FunctionMinimum minimum = minimizer();

  mpFit->chiSquare(minimum.UserParameters().Params());
  return minimum;
}

} // namespace Kaimini

// vim: sw=2 tw=78
