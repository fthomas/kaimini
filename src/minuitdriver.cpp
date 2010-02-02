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
#include <ostream>
#include <vector>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MinosError.h>
#include <Minuit2/MinuitParameter.h>
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
  mpFit->processResult(mpFit->paramTransformIntToExt(minpar));

  mpMinimum.reset(new FunctionMinimum(minimum));
  sanitize();
  return minimum;
}


FunctionMinimum MinuitDriver::runSimplex(unsigned int stra)
{
  MnSimplex minimizer(*mpFit, mpFit->mParamsInt, stra);
  FunctionMinimum minimum = minimizer();

  Parameters minpar = minimum.UserParameters();
  mpFit->processResult(mpFit->paramTransformIntToExt(minpar));

  mpMinimum.reset(new FunctionMinimum(minimum));
  sanitize();
  return minimum;
}


vector<MinosError>
MinuitDriver::runMinos(const FunctionMinimum& minimum, unsigned int stra)
{
  MnMinos minos(*mpFit, minimum, stra);

  vector<MinosError> errors;
  const vector<MinuitParameter>& mps = minimum.UserParameters().Parameters();

  for (vector<MinuitParameter>::const_iterator mp = mps.begin();
       mp != mps.end(); ++mp)
  {
    if (!mp->IsFixed() && !mp->IsConst())
    { errors.push_back(minos.Minos(mp->Number())); }
  }

  //mpFit->processResult();
  sanitize();
  return errors;
}


vector<MinosError>
MinuitDriver::runMinos(unsigned int stra)
{
  if (!mpMinimum) runMinimize();
  return runMinos(*mpMinimum, stra);
}


void MinuitDriver::sanitize()
{
  // Run chiSquare with the minimal parameter values again so that
  // cached variables in mpFit correspond to the found minimum.
  mpFit->chiSquare(mpMinimum->UserParameters());
}


ostream& operator<<(ostream& os, const vector<MinosError>& errors)
{
  os << "MinosErrors:" << endl;
  for (vector<MinosError>::const_iterator err = errors.begin();
       err != errors.end(); ++err)
  {
    os << "    - number : "  << err->Parameter() << endl
       << "      valid  : "  << err->IsValid()   << endl
       << "      lower  : "  << err->Lower()     << endl
       << "      upper  :  " << err->Upper()     << endl
       << endl;
  }
  return os;
}

} // namespace Kaimini

// vim: sw=2 tw=78
