// Kaimini, a general purpose fitting frontend
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
#include "error.h"
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
  { exit_block_not_found("KaiminiDataPoints"); }

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

    DataPoint dp;
    try
    {
      dp.number(   to_<int>((*line)[0]) );
      dp.name(              (*line)[1]  );
      dp.use(     to_<bool>((*line)[3]) );
      dp.value( to_<double>((*line)[4]) );
      dp.error( parse_error_string(dp.value(), (*line)[5]) );
    }
    catch (bad_lexical_cast&)
    {
      exit_line_not_parsed("KaiminiDataPoints", line->str());
    }

    mDataPoints.push_back(dp);
    mDataPointsKeys.push_back((*line)[2]);
  }
}


void SLHAFit::setParameters(const SLHA& input)
{
  SLHA::const_iterator block = input.find("KaiminiParameters");
  if (input.end() == block)
  { exit_block_not_found("KaiminiParameters"); }

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
      mParamsExt.SetError(name,
        parse_error_string(mParamsExt.Value(name), (*line)[4]));

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
      mDataPoints[i].cachedValue(
        to_<double>(input.field(mDataPointsKeys[i])));
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


void SLHAFit::processParametersImpl(const Parameters* intPar)
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
      format(" %1% %|4t|%2% %|16t|%3% %4$16.8E %5$16.8E   %6%")
        % (mp->Number()+1)
        % mp->GetName()
        % !mp->IsFixed()
        % mp->Value()
        % mp->Error()
        % limits.str());
  }
}


void SLHAFit::processDataPointsImpl()
{
  if (mDataPoints.empty()) return;

  string block = "KaiminiDataPointsOut";
  mResult[block]["BLOCK"] = "BLOCK " + block;

  vector<DataPoint>::const_iterator dp;
  for (dp = mDataPoints.begin(); dp != mDataPoints.end(); ++dp)
  {
    mResult[block][""] = str(
      format(" %1% %|4t|%2% %|16t|%3% %4$16.8E %5$16.8E %6$16.8E")
        % dp->number()
        % dp->name()
        % dp->use()
        % dp->cachedValue()
        % dp->value()
        % dp->error());
  }

  block = "KaiminiChiSquare";
  mResult[block]["BLOCK"] = "BLOCK " + block;

  string fmt_str = " %1% %|4t|%2% %|16t|%3% %4$16.8E";
  mResult[block][""] = str(format(fmt_str)
    % "0"
    % "chi^2"
    % "1"
    % dps_add_residuals(mDataPoints));

  for (dp = mDataPoints.begin(); dp != mDataPoints.end(); ++dp)
  {
    mResult[block][""] = str(format(fmt_str)
      % dp->number()
      % dp->name()
      % dp->use()
      % dp->wtSqResidual());
  }
}


void SLHAFit::processMinimumImpl(const FunctionMinimum* minimum)
{
  Parameters min_par = minimum->UserParameters();
  processParameters(&min_par);

  string block;

  if (minimum->HasCovariance() && minimum->HasValidCovariance())
  {
    const MnUserCovariance& covar = minimum->UserCovariance();

    block = "KaiminiCovarianceMatrix";
    mResult[block]["BLOCK"] = "BLOCK " + block;

    for (unsigned int row = 0; row < covar.Nrow(); ++row)
    {
      for (unsigned int col = row; col < covar.Nrow(); ++col)
      {
        mResult[block][""] = str(format(" %1% %|4t|%2% %3$16.8E")
          % (row+1) % (col+1) % covar(row, col));
      }
    }
  }
}


void SLHAFit::processErrorsImpl(const vector<MinosError>* errors)
{
  if (errors->empty()) return;

  string block = "KaiminiMinosErrors";
  mResult[block]["BLOCK"] = "BLOCK " + block;

  for (vector<MinosError>::const_iterator me = errors->begin();
       me != errors->end(); ++me)
  {
    MinuitParameter mp = mParamsExt.Parameter(me->Parameter());

    mResult[block][""] = str(
      format(" %1% %|4t|%2% %|15t|%3% %4$16.8E %5$16.8E")
        % (me->Parameter()+1)
        % mp.GetName()
        % me->IsValid()
        % me->Upper()
        % me->Lower());
  }
}


void SLHAFit::processBootstrapImpl(const vector<vector<Error> >* errors,
                                   const unsigned int iterations)
{
  if (errors->empty()) return;

  string block = "KaiminiBootstrap";
  mResult[block]["BLOCK"] << "BLOCK " << block << "# iterations= "
                          << iterations;

  for (vector<vector<Error> >::const_iterator vec_err = errors->begin();
       vec_err != errors->end(); ++ vec_err)
  {
    size_t i = 0;
    for (vector<Error>::const_iterator err = vec_err->begin();
         err != vec_err->end(); ++err)
    {
      mResult[block][""] = str(
        format(" %1% %|4t|%2% %|8t|%3% %4$16.8E %5$16.8E %6$16.8E")
          % (err->number()+1)
          % ++i
          % err->name()
          % err->upper()
          % err->lower()
          % err->mean());
    }
  }
}


const SLHA& SLHAFit::result()
{
  string block;

  block = "KaiminiInfo";
  mResult[block]["BLOCK"] = "BLOCK " + block;
  mResult[block][""] << "1" << g_kaimini_version << "# version number";
  return mResult;
}

} // namespace Kaimini

// vim: sw=2 tw=78
