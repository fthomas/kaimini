// Kaimini, a general purpose fitting and analysis front end
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
#include <boost/algorithm/string.hpp>
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


void SLHAInterface::setDataPoints(const Coll& input)
{
  Coll::const_iterator block = input.find("KaiminiDataPoints");
  if (input.end() == block) exit_block_not_found("KaiminiDataPoints");

  mDataPoints.clear();
  mDataPointsKeys.clear();

  for (Block::const_iterator line = block->begin();
       line != block->end(); ++line)
  {
    if (!line->is_data_line()) continue;
    if (line->size() > 1 && "+" == (*line)[0])
    {
      if (mDataPointsKeys.end() != mDataPointsKeys.begin())
      {
        mDataPointsKeys.back().push_back(Key((*line)[1]));
      }
      continue;
    }
    if (line->size() < 6)
    {
      warn_line_ignored(block->name(), line->str());
      continue;
    }

    vector<string> flags_str;
    split(flags_str, (*line)[3], is_any_of(","));

    DataPoint dp;
    try
    {
      dp.number(   to<int>((*line)[0]) );
      dp.name(              (*line)[1]  );
      dp.use(     to<bool>(flags_str[0]) );
      dp.value( to<double>((*line)[4]) );
      dp.error( parse_error_string(dp.value(), (*line)[5]) );

      if (flags_str.size() > 1 && !flags_str[1].empty())
      {
        dp.ifAbsent(to<int>(flags_str[1]));
      }
      if (flags_str.size() > 2 && !flags_str[2].empty())
      {
        dp.ifNaN(to<int>(flags_str[2]));
      }
    }
    catch (bad_lexical_cast&)
    {
      exit_line_not_parsed(block->name(), line->str());
    }

    mDataPoints.push_back(dp);
    mDataPointsKeys.push_back(vector<Key>(1, Key((*line)[2])));
  }
}


void SLHAInterface::setParameters(const Coll& input)
{
  Coll::const_iterator block = input.find("KaiminiParameters");
  if (input.end() == block) exit_block_not_found("KaiminiParameters");

  mParams = Parameters();
  mParamsKeys.clear();

  for (Block::const_iterator line = block->begin();
       line != block->end(); ++line)
  {
    if (!line->is_data_line()) continue;
    if (line->size() < 6)
    {
      warn_line_ignored(block->name(), line->str());
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
    const Key key((*line)[2]);

    mParams.Add(name, 0., 0.);
    mParamsKeys.push_back(key);

    try
    {
      mParams.SetValue(name, to<double>(input.field(key)));
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

      if (!to<bool>((*line)[3])) mParams.Fix(name);

      if (!lower.empty() && !upper.empty())
      {
        mParams.SetLimits(name, to<double>(lower), to<double>(upper));
      }
      else if (!lower.empty())
      {
        mParams.SetLowerLimit(name, to<double>(lower));
      }
      else if (!upper.empty())
      {
        mParams.SetUpperLimit(name, to<double>(upper));
      }
    }
    catch (bad_lexical_cast&)
    {
      exit_line_not_parsed(block->name(), line->str());
    }
  }
}


void SLHAInterface::readDataPoints(const Coll& input) const
{
  assert(mDataPoints.size() == mDataPointsKeys.size());

  vector<DataPoint>::const_iterator dp = mDataPoints.begin();
  vector<vector<Key> >::const_iterator dp_keys = mDataPointsKeys.begin();

  for (; dp != mDataPoints.end() && dp_keys != mDataPointsKeys.end();
       ++dp, ++dp_keys)
  {
    string value_str;
    double value = 0.;

    for (vector<Key>::const_iterator key = dp_keys->begin();
         key != dp_keys->end(); ++key)
    {
      try
      {
        value_str = input.field(*key);
        value += to<double>(value_str);
      }
      catch (out_of_range&)
      {
        switch (dp->ifAbsent())
        {
        case 0: exit_field_not_found(key->str());
                break;

        case 1: info_ignore_absent_field(key->str());
                continue;

        case 2: info_include_absent_field(key->str());
                break;
        }
      }
      catch (bad_lexical_cast&)
      {
        if ("NaN" == value_str)
        {
          switch (dp->ifNaN())
          {
          case 1: info_ignore_nan(key->str());
                  continue;

          case 2: info_include_nan(key->str());
                  break;
          }
        }
        exit_value_not_parsed(key->str(), value_str);
      }
      dp->cachedValue(value);
    } // foreach key
  } // foreach data point
}


void SLHAInterface::writeParameters(const vector<double>& params,
                                    Coll& output) const
{
  assert(params.size() == mParamsKeys.size());

  stringstream par_out;
  par_out.precision(8);
  par_out.setf(ios_base::scientific);

  vector<double>::const_iterator  par     = params.begin();
  vector<Key>::const_iterator par_key = mParamsKeys.begin();

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
      format(" %1% %|4t|%2% %|16t|%3% %|19t|%4$15.8E %5$16.8E %6$16.8E")
        % dp->number()
        % dp->name()
        % dp->use()
        % dp->cachedValue()
        % dp->value()
        % dp->error());
  }

  block = "KaiminiChiSquare";
  mResult[block]["BLOCK"] = "BLOCK " + block;

  string fmt_str = " %1% %|4t|%2% %|16t|%3% %|20t|%4$14.8E";
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
        % (mp->Number() + 1)
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

  if (minimum->HasCovariance() && minimum->HasValidCovariance())
  {
    const MnUserCovariance& covar = minimum->UserCovariance();

    string block_covar = "KaiminiCovarianceMatrix";
    mResult[block_covar]["BLOCK"] = "BLOCK " + block_covar;

    string block_corr  = "KaiminiCorrelationMatrix";
    mResult[block_corr]["BLOCK"]  = "BLOCK " + block_corr;

    const vector<MinuitParameter>& mps = params.getMinuitParameters();
    vector<MinuitParameter>::const_iterator mp_row, mp_col;
    unsigned int row = 0, col = 0;

    for (mp_row = mps.begin(); mp_row != mps.end(); ++mp_row)
    {
      if (mp_row->IsFixed() || mp_row->IsConst()) continue;

      col = row;
      for (mp_col = mp_row; mp_col != mps.end(); ++mp_col)
      {
        if (mp_col->IsFixed() || mp_col->IsConst()) continue;

        mResult[block_covar][""] = str(
          format(" %1% %|4t|%2% %3$16.8E")
            % (row+1)
            % (col+1)
            % covar(row, col));

        mResult[block_corr][""] = str(
          format(" %1% %|4t|%2% %3$16.8E")
            % (row+1)
            % (col+1)
            % (covar(row, col) / (mp_row->Error() * mp_col->Error())));

        ++col;
      }
      ++row;
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
      format(" %1% %|4t|%2% %|16t|%3% %4$16.8E %5$16.8E")
        % (me->Parameter() + 1)
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
          % (err->number() + 1)
          %  ++i
          %  err->name()
          %  err->upper()
          %  err->lower()
          %  err->mean());
    }
  }
}


void SLHAInterface::processChiSqContribImpl(
        const map<int, map<int, double> >* single,
        const map<int, double>* total)
{
  string block;

  if (!total->empty())
  {
    block = "KaiminiChiSquareContrib";
    mResult[block]["BLOCK"] = "BLOCK " + block;

    for (map<int, double>::const_iterator it = total->begin();
         it != total->end(); ++it)
    {
      mResult[block][""] = str(
        format(" %1% %|6t|%2$16.8E %|28t|# %3%")
          % (it->first + 1)
          %  it->second
          %  mParams.Name(it->first));
    }
  }

  if (!single->empty())
  {
    block = "KaiminiChiSquareContribSingle";
    mResult[block]["BLOCK"] = "BLOCK " + block;

    for (map<int, map<int, double> >::const_iterator it1 = single->begin();
         it1 != single->end(); ++it1)
    {
      for (map<int, double>::const_iterator it2 = it1->second.begin();
           it2 != it1->second.end(); ++it2)
      {
        mResult[block][""] = str(
          format(" %1% %|4t|%2% %|8t|%3$14.8E %|28t|# %4% %|44t|%5%")
            % (it1->first + 1)
            %  mDataPoints.at(it2->first).number()
            %  it2->second
            %  mParams.Name(it1->first)
            %  mDataPoints.at(it2->first).name());
      }
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
  Block::iterator after_version = mResult[block].find(version_key);
  if (after_version != mResult[block].end()) after_version++;

  vector<Line> lines;
  lines.push_back(Line(str(
    format(" 2 %|4t|%1% %|16t|%2%")
      % wallTime
      % "# approx. wall clock time [s]")));

  lines.push_back(Line(str(
    format(" 3 %|4t|%1$.3f %|16t|%2%")
      % procTime
      % "# approx. process time [s]")));

  mResult[block].insert(after_version, lines.begin(), lines.end());
}


void SLHAInterface::clearResultsImpl()
{
  for (Coll::iterator block = mResult.begin(); block != mResult.end(); )
  {
    if (!iequals(block->name(), "KaiminiInfo") &&
        istarts_with(block->name(), "Kaimini"))
    {
      block = mResult.erase(block);
    }
    else ++block;
  }
}


const Coll& SLHAInterface::result()
{
  return mResult;
}

} // namespace Kaimini

// vim: sw=2 tw=78
