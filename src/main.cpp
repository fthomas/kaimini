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

#include <cstddef>
#include <fstream>
#include <ostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <Minuit2/MnPrint.h>
#include "genericdriver.h"
#include "gsldriver.h"
#include "kaimini.h"
#include "minuitdriver.h"
#include "parameteranalysis.h"
#include "slhaea.h"
#include "slhaworker.h"

using namespace std;
using namespace boost;
using namespace Kaimini;
using namespace SLHAea;
namespace fs = boost::filesystem;

int main(int argc, char* argv[])
{
  cout.precision(8);
  cout.setf(ios_base::scientific);

  string input_file  = "kaimini.in";
  string output_file = "kaimini.out";
  parse_command_line(argc, argv, &input_file, &output_file);

  if (!fs::exists(input_file)) exit_file_nonexistent(input_file);

  ifstream input_fs(input_file.c_str());
  const SLHA input_slha(input_fs);
  input_fs.close();

  SLHAWorker fit(input_slha);

  GenericDriver gen_driver(&fit);
  GSLDriver gsl_driver(&fit);
  MinuitDriver mn_driver(&fit);

  Parameters min_params = fit.getParameters();

  if (input_slha.count("KaiminiControl") == 0)
  {
    mn_driver.runMinimize(2);
    fit.shutdown(output_file);
    return 0;
  }

  const SLHABlock kaimini_control = input_slha.at("KaiminiControl");

  // Default number of iterations for bootstrapping.
  unsigned int bootstrap_iter = 1000;

  for (SLHABlock::const_iterator line = kaimini_control.begin();
       line != kaimini_control.end(); ++line)
  {
    if (line->data_size() < 2 || (*line)[0] != "0") continue;

    const size_t data_size = line->data_size();
    const string key = (*line)[1];

    try
    {
      if (boost::iequals(key, "MinuitErrorDef") && data_size > 2)
      {
        fit.SetErrorDef(to_<double>((*line)[2]));
      }
      else if (boost::iequals(key, "BootstrapIter") && data_size > 2)
      {
        bootstrap_iter = to_<unsigned int>((*line)[2]);
      }
      else warn_line_ignored(kaimini_control.name(), line->str());
    }
    catch (bad_lexical_cast&)
    {
      exit_line_not_parsed(kaimini_control.name(), line->str());
    }
  }

  for (SLHABlock::const_iterator line = kaimini_control.begin();
       line != kaimini_control.end(); ++line)
  {
    const size_t data_size = line->data_size();
    if (data_size < 3 || (*line)[0] != "1") continue;

    const string action = (*line)[1];
    const string action_switch = boost::to_lower_copy((*line)[2]);

    if ("off" == action_switch) continue;
    else if ("on" != action_switch)
    {
      warn_unrecognized_switch(line->str(), action_switch);
      continue;
    }

    unsigned int mn_strategy = 2;

    try
    {
      if (mn_driver.minimizer2Map.find(action)
          != mn_driver.minimizer2Map.end())
      {
        if (data_size > 3) mn_strategy = to_<unsigned int>((*line)[3]);

        MinuitDriver::minimizer2_t min_func = mn_driver.minimizer2Map[action];
        min_params = (mn_driver.*min_func)(min_params, mn_strategy);
      }
      else if (boost::iequals(action, "MinuitMinos"))
      {
        if (data_size > 3) mn_strategy = to_<unsigned int>((*line)[3]);
        mn_driver.runMinos(mn_strategy);
      }
      else if (boost::iequals(action, "GSLSimplex"))
      {
        min_params = gsl_driver.runSimplex(min_params);
      }
      else if (boost::iequals(action, "GSLSimulatedAnnealing"))
      {
        min_params = gsl_driver.runSimulatedAnnealing(min_params);
      }
      else if (boost::iequals(action, "SimulatedAnnealing"))
      {
        bool temp = fit.getSaveAllPoints();
        fit.setSaveAllPoints(true);

        min_params = gen_driver.runSimulatedAnnealing(min_params);

        fit.setSaveAllPoints(temp);
      }
      else if (boost::iequals(action, "Metropolis"))
      {
        bool temp = fit.getSaveAllPoints();
        fit.setSaveAllPoints(true);

        min_params = gen_driver.runMetropolis(min_params);

        fit.setSaveAllPoints(temp);
      }
      else if (boost::iequals(action, "Bootstrap"))
      {
        // Cast &MinuitDriver::runMinimize to Driver::minimizer_t.
        // The cast to MinuitDriver::minimizer0_t is required because
        // runMinimize is an overloaded function and the compiler has
        // no type information to select the version we want.
        Driver::minimizer_t min_func =
          static_cast<Driver::minimizer_t>(
            static_cast<MinuitDriver::minimizer0_t>(
              &MinuitDriver::runMinimize));

        bootstrap(&mn_driver, min_func, min_params, bootstrap_iter);
      }
      else if (boost::iequals(action, "ChiSquareContrib"))
      {
        jolt_parameters(&fit, min_params);
      }
      else warn_line_ignored(kaimini_control.name(), line->str());
    }
    catch (bad_lexical_cast&)
    {
      warn_line_not_parsed(kaimini_control.name(), line->str());
    }
  }

  fit.shutdown(output_file);
  return 0;
}

// vim: sw=2 tw=78
