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
#include <map>
#include <string>
#include <ostream>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MinosError.h>
#include <Minuit2/MinuitParameter.h>
#include <Minuit2/MnApplication.h>
#include <Minuit2/MnContours.h>
#include <Minuit2/MnHesse.h>
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
using namespace boost;
using namespace ROOT::Minuit2;

namespace Kaimini {

Parameters MinuitDriver::runMigrad(const Parameters& startParams,
                                   const unsigned int stra)
{
  MnMigrad minimizer(*mpFunc, startParams, stra);
  mpMinimum.reset(new FunctionMinimum(minimizer()));
  runHesse(stra);

  sanitize();
  processResults("10", "Migrad", minimizer);

  if (g_verbose_output) cout << *mpMinimum;
  return mpMinimum->UserParameters();
}


Parameters MinuitDriver::runMinimize(const Parameters& startParams,
                                     const unsigned int stra)
{
  MnMinimize minimizer(*mpFunc, startParams, stra);
  mpMinimum.reset(new FunctionMinimum(minimizer()));
  runHesse(stra);

  sanitize();
  processResults("11", "Minimize", minimizer);

  if (g_verbose_output) cout << *mpMinimum;
  return mpMinimum->UserParameters();
}


Parameters MinuitDriver::runScan(const Parameters& startParams,
                                 const unsigned int stra)
{
  MnScan minimizer(*mpFunc, startParams, stra);
  mpMinimum.reset(new FunctionMinimum(minimizer()));
  runHesse(stra);

  sanitize();
  processResults("12", "Scan", minimizer);

  if (g_verbose_output) cout << *mpMinimum;
  return mpMinimum->UserParameters();
}


Parameters MinuitDriver::runSimplex(const Parameters& startParams,
                                    const unsigned int stra)
{
  MnSimplex minimizer(*mpFunc, startParams, stra);
  mpMinimum.reset(new FunctionMinimum(minimizer()));
  runHesse(stra);

  sanitize();
  processResults("13", "Simplex", minimizer);

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
  MnMinos minos(*mpFunc, minimum, stra);

  vector<MinosError> errors;
  const vector<MinuitParameter>& mps = minimum.UserParameters().Parameters();

  for (vector<MinuitParameter>::const_iterator mp = mps.begin();
       mp != mps.end(); ++mp)
  {
    if (!mp->IsFixed() && !mp->IsConst())
    {
      errors.push_back(minos.Minos(mp->Number()));
    }
  }

  sanitize();
  mpFunc->processErrors(&errors);

  if (g_verbose_output) cout << errors;
  return errors;
}


vector<MinosError>
MinuitDriver::runMinos(const unsigned int stra)
{
  if (!mpMinimum) runMinimize(stra);
  return runMinos(*mpMinimum, stra);
}


void MinuitDriver::runContours(const FunctionMinimum& minimum,
                               const unsigned int stra)
{
  MnContours contours(*mpFunc, minimum, stra);

  const vector<MinuitParameter>& mps = minimum.UserParameters().Parameters();
  vector<MinuitParameter>::const_iterator mp1, mp2;

  for (mp1 = mps.begin(); mp1 != mps.end()-1; ++mp1)
  {
    for (mp2 = mp1+1; mp2 != mps.end(); ++mp2)
    {
      if (!mp1->IsFixed() && !mp1->IsConst() &&
          !mp2->IsFixed() && !mp2->IsConst())
      {
        contours(mp1->Number(), mp2->Number(), 250);
      }
    }
  }

  sanitize();
}


void MinuitDriver::runContours(const unsigned int stra)
{
  if (!mpMinimum) runMinimize(stra);
  return runContours(*mpMinimum, stra);
}


void MinuitDriver::runHesse(const unsigned int stra)
{
  if (mpMinimum->IsValid())
  {
    MnHesse hesse(stra);
    hesse(*mpFunc, *mpMinimum);
  }
}


void MinuitDriver::sanitize()
{
  // Run chiSq with the minimal parameter values again so that cached
  // variables in *mpFunc correspond to the found minimum.
  mpFunc->chiSq(mpMinimum->UserParameters());
}


void MinuitDriver::processResults(const string& number, const string& name,
                                  const MnApplication& app)
{
  if (!mpFunc->processingEnabled()) return;

  map<string, string> info;
  info["number"]   = number;
  info["name"]     = "Minuit" + name;
  info["calls"]    = lexical_cast<string>(app.NumOfCalls());
  info["strategy"] = lexical_cast<string>(app.Strategy().Strategy());
  info["valid_minimum"] = mpMinimum->IsValid() ? "true" : "false";

  mpFunc->clearResults();
  mpFunc->processDriverInfo(&info);
  mpFunc->processMinimum(mpMinimum.get());
  mpFunc->processDataPoints();
}


ostream& operator<<(ostream& os, const vector<MinosError>& errors)
{
  os << "MinosErrors:" << endl;
  for (vector<MinosError>::const_iterator error = errors.begin();
       error != errors.end(); ++error)
  {
    os << "    - number : " << error->Parameter() << endl
       << "      valid  : " << error->IsValid()   << endl
       << "      upper  : " << error->Upper()     << endl
       << "      lower  : " << error->Lower()     << endl
       <<                                            endl;
  }
  return os;
}

} // namespace Kaimini
*/
