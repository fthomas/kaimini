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

#include <cstddef>
#include <vector>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MinosError.h>
#include <Minuit2/MnMinimize.h>
#include <Minuit2/MnMinos.h>
#include <Minuit2/MnSimplex.h>
#include "minuitdriver.h"
#include "parameters.h"

using namespace std;
using namespace ROOT::Minuit2;

namespace Kaimini {

FunctionMinimum MinuitDriver::runMinimize(unsigned int stra)
{
  MnMinimize minimizer(*mpFit, mpFit->mParamsInt, stra);
  FunctionMinimum minimum = minimizer();
  Parameters minpar = minimum.UserParameters();

  // Run chiSquare with the minimal parameter values again so that
  // cached variables in mpFit correspond to the found minimum.
  mpFit->chiSquare(minpar);

  mpFit->processResult(mpFit->paramTransformIntToExt(minpar));
  return minimum;
}


FunctionMinimum MinuitDriver::runSimplex(unsigned int stra)
{
  MnSimplex minimizer(*mpFit, mpFit->mParamsInt, stra);
  FunctionMinimum minimum = minimizer();
  Parameters minpar = minimum.UserParameters();

  // Run chiSquare with the minimal parameter values again so that
  // cached variables in mpFit correspond to the found minimum.
  mpFit->chiSquare(minpar);

  mpFit->processResult(mpFit->paramTransformIntToExt(minpar));
  return minimum;
}


vector<MinosError>
MinuitDriver::runMinos(const FunctionMinimum& minimum, unsigned int stra)
{
  MnMinos minos(*mpFit, minimum, stra);

  vector<MinosError> errors;
  for (size_t i = 0; i < mpFit->mParamsInt.getParams().size(); ++i)
  { errors.push_back(minos.Minos(i)); }

  //mpFit->processResult();
  return errors;
}

} // namespace Kaimini

// vim: sw=2 tw=78
