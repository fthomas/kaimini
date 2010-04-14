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
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
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
#include "slhainterface.h"

using namespace std;
using namespace boost;
using namespace ROOT::Minuit2;
using namespace SLHAea;

namespace Kaimini {

SLHAInterface::SLHAInterface()
{
  string block = "KaiminiInfo";
  mResult[block][""] << "BLOCK" << block;
  mResult[block][""] = str(format(" 1 %|4t|%1% %|16t|%2%")
    % g_kaimini_version
    % "# version number");
}


void SLHAInterface::setDataPoints(const SLHA& input)
{
  SLHA::const_iterator block = input.find("KaiminiDataPoints");
  if (input.end() == block) exit_block_not_found("KaiminiDataPoints");

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
    mDataPointsKeys.push_back(SLHAKey((*line)[2]));
  }
}


void SLHAInterface::setParameters(const SLHA& input)
{
  SLHA::const_iterator block = input.find("KaiminiParameters");
  if (input.end() == block) exit_block_not_found("KaiminiParameters");

  mParams = Parameters();
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
    string lower = "";
    string upper = "";

    if (bounds.length() > 1)
    {
      const size_t colon = bounds.find(':');
      lower = bounds.substr(0, colon);
      if (string::npos != colon) upper = bounds.substr(colon+1);
    }

    const string name = (*line)[1];
    const SLHAKey key((*line)[2]);

    mParams.Add(name, 0., 0.);
    mParamsKeys.push_back(key);

    try
    {
      mParams.SetValue(name, to_<double>(input.field(key)));
    }
    catch (out_of_range&)
    {
      exit_field_not_found(key.str());
    }
    catch (bad_lexical_cast&)
    {
      exit_value_not_parsed(key.str(), input.field(key));
    }

    try
    {
      double error = parse_error_string(mParams.Value(name), (*line)[4]);
      mParams.SetError(name, error);

      if (!to_<bool>((*line)[3])) mParams.Fix(name);

      if (!lower.empty() && !upper.empty())
      {
        mParams.SetLimits(name, to_<double>(lower), to_<double>(upper));
      }
      else if (!lower.empty())
      {
        mParams.SetLowerLimit(name, to_<double>(lower));
      }
      else if (!upper.empty())
      {
        mParams.SetUpperLimit(name, to_<double>(upper));
      }
    }
    catch (bad_lexical_cast&)
    {
      exit_line_not_parsed("KaiminiParameters", line->str());
    }
  }
}


void SLHAInterface::readDataPoints(const SLHA& input) const
{
  assert(mDataPoints.size() == mDataPointsKeys.size());

  vector<DataPoint>::const_iterator dp     = mDataPoints.begin();
  vector<SLHAKey>::const_iterator   dp_key = mDataPointsKeys.begin();

  for (; dp != mDataPoints.end() && dp_key != mDataPointsKeys.end();
       ++dp, ++dp_key)
  {
    string value;
    try
    {
      value = input.field(*dp_key);
      dp->cachedValue(to_<double>(value));
    }
    catch (out_of_range&)
    {
      exit_field_not_found(dp_key->str());
    }
    catch (bad_lexical_cast&)
    {
      if ("NaN" == value) continue;

      exit_value_not_parsed(dp_key->str(), value);
    }
  }
}


void SLHAInterface::writeParameters(const vector<double>& params,
                                    SLHA& output) const
{
  assert(params.size() == mParamsKeys.size());

  stringstream par_out;
  par_out.precision(8);
  par_out.setf(ios_base::scientific);

  vector<double>::const_iterator  par     = params.begin();
  vector<SLHAKey>::const_iterator par_key = mParamsKeys.begin();

  for (; par != params.end() && par_key != mParamsKeys.end();
       ++par, ++par_key)
  {
    par_out.str("");
    par_out << *par;

    try
    {
      output.field(*par_key) = par_out.str();
    }
    catch (out_of_range&)
    {
      exit_field_not_found(par_key->str());
    }
  }
}


void SLHAInterface::processDataPointsImpl()
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


void SLHAInterface::processParametersImpl(const Parameters* params)
{
  if (params->getParams().empty()) return;

  string block = "KaiminiParametersOut";
  mResult[block]["BLOCK"] = "BLOCK " + block;

  const vector<MinuitParameter>& mps = params->getMinuitParameters();
  vector<MinuitParameter>::const_iterator mp;

  for (mp = mps.begin(); mp != mps.end(); ++mp)
  {
    stringstream limits;
    if (mp->HasLowerLimit()) limits << mp->LowerLimit();
    limits << ":";
    if (mp->HasUpperLimit()) limits << mp->UpperLimit();

    mResult[block][""] = str(
      format(" %1% %|4t|%2% %|16t|%3% %4$16.8E %5$16.8E   %6%")
        % (mp->Number()+1)
        %  mp->GetName()
        % !mp->IsFixed()
        %  mp->Value()
        %  mp->Error()
        %  limits.str());
  }

  block = "KaiminiParameterDifferences";
  mResult[block]["BLOCK"] = "BLOCK " + block;

  const vector<MinuitParameter>& orig_mps = mParams.getMinuitParameters();
  vector<MinuitParameter>::const_iterator orig_mp;

  for(mp = mps.begin(), orig_mp = orig_mps.begin();
      mp != mps.end() && orig_mp != orig_mps.end(); ++mp, ++orig_mp)
  {
    double abs_diff = mp->Value() - orig_mp->Value();
    double rel_diff = abs_diff / orig_mp->Value() * 100.;

    mResult[block][""] = str(
      format(" %1% %|4t|%2% %|14t|%3$16.8E %4$16.8E %5$10.2E%%")
        % (mp->Number()+1)
        %  mp->GetName()
        %  orig_mp->Value()
        %  abs_diff
        %  rel_diff);
  }
}


void SLHAInterface::processMinimumImpl(const FunctionMinimum* minimum)
{
  Parameters params = minimum->UserParameters();
  processParameters(&params);

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
          % (row+1)
          % (col+1)
          % covar(row, col));
      }
    }
  }
}


void SLHAInterface::processErrorsImpl(const vector<MinosError>* errors)
{
  if (errors->empty()) return;

  string block = "KaiminiMinosErrors";
  mResult[block]["BLOCK"] = "BLOCK " + block;

  for (vector<MinosError>::const_iterator me = errors->begin();
       me != errors->end(); ++me)
  {
    MinuitParameter mp = mParams.Parameter(me->Parameter());

    mResult[block][""] = str(
      format(" %1% %|4t|%2% %|15t|%3% %4$16.8E %5$16.8E")
        % (me->Parameter()+1)
        %  mp.GetName()
        %  me->IsValid()
        %  me->Upper()
        %  me->Lower());
  }
}


void SLHAInterface::processBootstrapImpl(
        const vector<vector<Error> >* errors, const unsigned int iterations)
{
  if (errors->empty()) return;

  string block = "KaiminiBootstrap";
  mResult[block]["BLOCK"] = "BLOCK " + block + "# iterations= "
                            + to_string(iterations);

  for (vector<vector<Error> >::const_iterator vec_err = errors->begin();
       vec_err != errors->end(); ++ vec_err)
  {
    int i = 0;
    for (vector<Error>::const_iterator err = vec_err->begin();
         err != vec_err->end(); ++err)
    {
      mResult[block][""] = str(
        format(" %1% %|4t|%2% %|8t|%3% %4$16.8E %5$16.8E %6$16.8E")
          % (err->number()+1)
          %  ++i
          %  err->name()
          %  err->upper()
          %  err->lower()
          %  err->mean());
    }
  }
}


void SLHAInterface::processDriverInfoImpl(const map<string, string>* infos)
{
  if (infos->empty()) return;

  string number = infos->find("number")->second;
  string name   = infos->find("name")->second;

  string block = "KaiminiInfo";
  mResult[block][""] << number << name;

  for (map<string, string>::const_iterator entry = infos->begin();
       entry != infos->end(); ++entry)
  {
    if (entry->first == "number" || entry->first == "name") continue;

    mResult[block][""] << number << ("- " + entry->first) << entry->second;
  }
}


void SLHAInterface::processRuntimeImpl(const double wallTime,
                                       const double procTime)
{
  string block = "KaiminiInfo";

  vector<string> version_key;
  version_key.push_back("1");
  SLHABlock::iterator after_version = mResult[block].find(version_key);
  if (after_version != mResult[block].end()) after_version++;

  vector<SLHALine> lines;
  lines.push_back(SLHALine(str(
    format(" 2 %|4t|%1% %|16t|%2%")
      % wallTime
      % "# approx. wall clock time [s]")));

  lines.push_back(SLHALine(str(
    format(" 3 %|4t|%1$.3f %|16t|%2%")
      % procTime
      % "# approx. process time [s]")));

  mResult[block].insert(after_version, lines.begin(), lines.end());
}


const SLHA& SLHAInterface::result()
{
  return mResult;
}

} // namespace Kaimini

// vim: sw=2 tw=78
