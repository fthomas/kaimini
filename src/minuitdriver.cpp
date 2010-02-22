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

#include <cstddef>
#include <iostream>
#include <ostream>
#include <vector>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MinosError.h>
#include <Minuit2/MinuitParameter.h>
#include <Minuit2/MnMigrad.h>
#include <Minuit2/MnMinimize.h>
#include <Minuit2/MnMinos.h>
#include <Minuit2/MnPrint.h>
#include <Minuit2/MnScan.h>
#include <Minuit2/MnSimplex.h>
#include "kaimini.h"
#include "minuitdriver.h"
#include "parameters.h"

using namespace std;
using namespace ROOT::Minuit2;

namespace Kaimini {

Parameters MinuitDriver::runMigrad(const unsigned int stra)
{
  MnMigrad minimizer(*mpFit, mpFit->getIntParameters(), stra);
  mpMinimum.reset(new FunctionMinimum(minimizer()));
  mpFit->processMinimum(mpMinimum.get());

  sanitize();
  if (g_verbose_output) cout << *mpMinimum;
  return mpMinimum->UserParameters();
}


Parameters MinuitDriver::runMinimize(const unsigned int stra)
{
  MnMinimize minimizer(*mpFit, mpFit->getIntParameters(), stra);
  mpMinimum.reset(new FunctionMinimum(minimizer()));
  mpFit->processMinimum(mpMinimum.get());

  sanitize();
  if (g_verbose_output) cout << *mpMinimum;
  return mpMinimum->UserParameters();
}


Parameters MinuitDriver::runScan(const unsigned int stra)
{
  MnScan minimizer(*mpFit, mpFit->getIntParameters(), stra);
  mpMinimum.reset(new FunctionMinimum(minimizer()));
  mpFit->processMinimum(mpMinimum.get());

  sanitize();
  if (g_verbose_output) cout << *mpMinimum;
  return mpMinimum->UserParameters();
}


Parameters MinuitDriver::runSimplex(const unsigned int stra)
{
  MnSimplex minimizer(*mpFit, mpFit->getIntParameters(), stra);
  mpMinimum.reset(new FunctionMinimum(minimizer()));
  mpFit->processMinimum(mpMinimum.get());

  sanitize();
  if (g_verbose_output) cout << *mpMinimum;
  return mpMinimum->UserParameters();
}


FunctionMinimum MinuitDriver::getFunctionMinimum()
{
  if (!mpMinimum) runMinimize();
  return *mpMinimum;
}


vector<MinosError>
MinuitDriver::runMinos(const FunctionMinimum& minimum,
                       const unsigned int stra)
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

  mpFit->processErrors(&errors);
  sanitize();
  if (g_verbose_output) cout << errors;
  return errors;
}


vector<MinosError>
MinuitDriver::runMinos(const unsigned int stra)
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

} // namespace Kaimini

// vim: sw=2 tw=78
