// Kaimini
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

#include <cassert>
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <boost/format.hpp>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MinosError.h>
#include <Minuit2/MinuitParameter.h>
#include <Minuit2/MnUserCovariance.h>
#include "datapoint.h"
#include "kaimini.h"
#include "parameters.h"
#include "slhaea.h"
#include "slhafit.h"

using namespace std;
using namespace boost;
using namespace ROOT::Minuit2;
using namespace SLHAea;

namespace Kaimini {

void SLHAFit::setDataPoints(const SLHA& input)
{
  SLHA::const_iterator block = input.find("KaiminiDataPoints");
  if (input.end() == block)
  {
    throw_block_not_found("SLHAFit::setDataPoints()", "KaiminiDataPoints");
  }

  mDataPoints.clear();
  mDataPointsKeys.clear();

  for (SLHABlock::const_iterator line = block->begin();
       line != block->end(); ++line)
  {
    if (!line->is_data_line()) continue;
    if (line->size() < 6)
    {
      warn_line_ignored("KaiminiDataPoints", line->str());
      continue;
    }

    // Parse the error string.
    string error_str = (*line)[5];
    bool normal_dist = false;
    if (error_str.compare(0, 7, "normal:") == 0)
    {
      error_str = error_str.substr(7);
      normal_dist = true;
    }

    string::iterator last_char = error_str.end()-1;
    bool percentage = false;
    if ('%' == *last_char)
    {
      error_str.erase(last_char);
      percentage = true;
    }

    DataPoint dp;
    double error = 0.;

    try
    {
      dp.name  = (*line)[1];
      dp.use   = to_<bool>((*line)[3]);
      dp.value = to_<double>((*line)[4]);
      error    = to_<double>(error_str);
    }
    catch (bad_lexical_cast&)
    {
      exit_line_not_parsed("KaiminiDataPoints", line->str());
    }

    if (percentage)
    { error *= 0.01 * dp.value; }

    if (normal_dist)
    { dp.randomNormalError(error); }
    else
    { dp.error = error; }

    mDataPoints.push_back(dp);
    mDataPointsKeys.push_back((*line)[2]);
  }
}


void SLHAFit::setParameters(const SLHA& input)
{
  SLHA::const_iterator block = input.find("KaiminiParameters");
  if (input.end() == block)
  {
    throw_block_not_found("SLHAFit::setParameters()", "KaiminiParameters");
  }

  mParamsExt = Parameters(); // Erase old parameters.
  mParamsKeys.clear();

  for (SLHABlock::const_iterator line = block->begin();
       line != block->end(); ++line)
  {
    if (!line->is_data_line()) continue;
    if (line->size() < 6)
    {
      warn_line_ignored("KaiminiParameters", line->str());
      continue;
    }

    // Parse lower/upper bounds.
    const string bounds = (*line)[5];
    string lower = "", upper = "";

    if (bounds.length() > 1)
    {
      const size_t colon = bounds.find(':');
      lower = bounds.substr(0, colon);
      if (string::npos != colon) upper = bounds.substr(colon+1);
    }

    const string name = (*line)[1];
    const SLHAKey key((*line)[2]);
    mParamsExt.Add(name, 0., 0.);
    mParamsKeys.push_back(key);

    try
    {
      mParamsExt.SetValue(name, to_<double>(input.field(key)));
    }
    catch (bad_lexical_cast&)
    {
      exit_value_not_parsed(key.str(), input.field(key));
    }
    catch (out_of_range&)
    {
      exit_field_not_found(key.str());
    }

    try
    {
      // Parse the error string.
      double error = 0.;
      string error_str = (*line)[4];
      string::iterator last_char = error_str.end()-1;
      if ('%' == *last_char)
      {
        error_str.erase(last_char);
        error = to_<double>(error_str) * 0.01 * mParamsExt.Value(name);
      }
      else
      { error = to_<double>(error_str); }
      mParamsExt.SetError(name, error);

      if (!to_<bool>((*line)[3])) mParamsExt.Fix(name);

      if (!lower.empty() && !upper.empty())
      { mParamsExt.SetLimits(name, to_<double>(lower), to_<double>(upper)); }
      else if (!lower.empty())
      { mParamsExt.SetLowerLimit(name, to_<double>(lower)); }
      else if (!upper.empty())
      { mParamsExt.SetUpperLimit(name, to_<double>(upper)); }
    }
    catch (bad_lexical_cast&)
    {
      exit_line_not_parsed("KaiminiParameters", line->str());
    }
  }
  mParamsInt = paramTransformExtToInt(mParamsExt);
}


void SLHAFit::readDataPoints(const SLHA& input) const
{
  assert(mDataPoints.size() == mDataPointsKeys.size());

  for (size_t i = 0; i < mDataPoints.size(); ++i)
  {
    try
    {
      mDataPoints[i].calcValue =
        to_<double>(input.field(mDataPointsKeys[i]));
    }
    catch (bad_lexical_cast&)
    {
      exit_value_not_parsed(
        mDataPointsKeys[i].str(), input.field(mDataPointsKeys[i]));
    }
    catch (out_of_range&)
    {
      exit_field_not_found(mDataPointsKeys[i].str());
    }
  }
}


void SLHAFit::writeParameters(const vector<double>& v, SLHA& output) const
{
  assert(v.size() == mParamsKeys.size());

  for (size_t i = 0; i < v.size(); ++i)
  {
    try
    {
      output.field(mParamsKeys[i]) = to_string(v[i]);
    }
    catch (out_of_range&)
    {
      exit_field_not_found(mParamsKeys[i].str());
    }
  }
}


void SLHAFit::processParams(const Parameters* intPar)
{
  if (intPar->getParams().empty()) return;

  Parameters extPar = paramTransformIntToExt(*intPar);
  const vector<MinuitParameter>& mps = extPar.getMinuitParameters();

  string block = "KaiminiParametersOut";
  mResult[block]["BLOCK"] = "BLOCK " + block;
  for (vector<MinuitParameter>::const_iterator mp = mps.begin();
       mp != mps.end(); ++mp)
  {
    stringstream limits;
    if (mp->HasLowerLimit()) limits << mp->LowerLimit();
    limits << ":";
    if (mp->HasUpperLimit()) limits << mp->UpperLimit();

    mResult[block][""] = str(
      format(" %1% %|4t|%2% %|15t|%3% %4$16.8E %5$16.8E   %6%")
        % (mp->Number()+1)
        % mp->GetName()
        % !mp->IsFixed()
        % mp->Value()
        % mp->Error()
        % limits.str());
  }
}


void SLHAFit::processMinimum(const FunctionMinimum* intMin)
{
  Parameters intPar = intMin->UserParameters();
  processParams(&intPar);

  string block;

  if (intMin->HasCovariance() && intMin->HasValidCovariance())
  {
    const MnUserCovariance& intCovar = intMin->UserCovariance();
    // << Transform intCovar to external covariance matrix here. >>

    block = "KaiminiCovarianceMatrix";
    mResult[block]["BLOCK"] = "BLOCK " + block;
    for (unsigned int row = 0; row < intCovar.Nrow(); ++row)
    {
      for (unsigned int col = row; col < intCovar.Nrow(); ++col)
      {
        mResult[block][""] = str(format(" %1% %|4t|%2% %3$16.8E")
          % (row+1) % (col+1) % intCovar(row, col));
      }
    }
  }
}


void SLHAFit::processErrors(const vector<MinosError>* intErr)
{
  if (intErr->empty()) return;

  // << Transform intErr to external errors here. >>

  string block = "KaiminiMinosErrors";
  mResult[block]["BLOCK"] = "BLOCK " + block;
  for (vector<MinosError>::const_iterator me = intErr->begin();
       me != intErr->end(); ++me)
  {
    MinuitParameter mp = mParamsExt.Parameter(me->Parameter());

    mResult[block][""] = str(
      format(" %1% %|4t|%2% %|15t|%3% %4$16.8E %5$16.8E")
        % (me->Parameter()+1)
        % mp.GetName()
        % me->IsValid()
        % me->Lower()
        % me->Upper());
  }
}


const SLHA& SLHAFit::result()
{
  string block;

  if (!mDataPoints.empty())
  {
    block = "KaiminiDataPointsOut";
    mResult[block]["BLOCK"] = "BLOCK " + block;

    for (size_t i = 0; i < mDataPoints.size(); ++i)
    {
      mResult[block][""] = str(
        format(" %1% %|4t|%2% %|15t|%3% %4$16.8E %5$16.8E %6$16.8E")
          % (i+1)
          % mDataPoints[i].name
          % mDataPoints[i].use
          % mDataPoints[i].calcValue
          % mDataPoints[i].value
          % mDataPoints[i].error);
    }

    block = "KaiminiChiSquare";
    mResult[block]["BLOCK"] = "BLOCK " + block;
    mResult[block][""] = str(format(" 0  chi^2 %|15t|%1$15.8E") % mChiSq);

    for (size_t i = 0; i < mDataPoints.size(); ++i)
    {
      mResult[block][""] = str(
        format(" %1% %|4t|%2% %|15t|%3$15.8E")
          % (i+1)
          % mDataPoints[i].name
          % mDataPoints[i].wtSqResidual);
    }
  }

  block = "KaiminiInfo";
  mResult[block]["BLOCK"] = "BLOCK " + block;
  mResult[block][""] << "1" << kaimini_version << "# version number";
  return mResult;
}

} // namespace Kaimini

// vim: sw=2 tw=78
