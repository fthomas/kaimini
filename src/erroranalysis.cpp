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

#include <algorithm>
#include <iterator>
#include <vector>
#include <utility>
#include <boost/numeric/conversion/cast.hpp>
#include <Minuit2/MinuitParameter.h>
#include "chisqfunction.h"
#include "datapoint.h"
#include "driver.h"
#include "error.h"
#include "erroranalysis.h"
#include "kaimini.h"
#include "parameters.h"
#include "random.h"

using namespace std;
using namespace ROOT::Minuit2;

namespace Kaimini {

vector<vector<Error> >
bootstrap(Driver* driver, Driver::minimizer_t minFunc,
          const Parameters& minParams, const unsigned int iterations)
{
  ChiSqFunction* chisq_func = driver->getFunction();
  chisq_func->disableProcessing();

  const Parameters orig_params = chisq_func->getParameters();
  const vector<DataPoint> orig_dps = chisq_func->getDataPoints();

  chisq_func->setParameters(minParams);

  // Ensure that all cached values of chisq_func's data points were
  // calculated with minParams.
  chisq_func->chiSq(minParams);

  vector<DataPoint> min_dps = chisq_func->getDataPoints();
  dps_swap_values(min_dps);

  vector<DataPoint> synthetic_dps;
  vector<pair<double, Parameters> > sim_map;

  const int iters = boost::numeric_cast<int>(iterations);
  for (int i = 0; i < iters; ++i)
  {
    synthetic_dps = min_dps;
    dps_smear_normal(synthetic_dps);

    // Repeat the fit procedure with the synthetic data points to
    // obtain simulated minimal parameters for these data points.
    chisq_func->setDataPoints(synthetic_dps);
    const Parameters sim_params = (driver->*minFunc)();

    // Use the cached minimal data points to calculate the
    // corresponding chi^2 of the simulated parameters.
    chisq_func->setDataPoints(min_dps);
    const double sim_chisq = chisq_func->chiSq(sim_params);

    // If sim_chisq is close to zero the actual minimal parameters
    // have been reproduced. These should not be taken into account
    // for estimation of the confidence intervalls.
    if (close_to_zero(sim_chisq)) { --i; continue; }

    sim_map.push_back(make_pair(sim_chisq, sim_params));

    if (g_verbose_output)
    {
      cout << "bootstrap_pass:"               << endl
           << "  iteration : "  << (i+1)      << endl
           << "  chi^2     : "  << sim_chisq  << endl
           << "  "              << sim_params << endl;
    }
  }

  sort(sim_map.begin(), sim_map.end());

  if (g_verbose_output)
  {
    cout << "bootstrap_parameters:" << endl;
    for (vector<pair<double, Parameters> >::const_iterator it =
         sim_map.begin(); it != sim_map.end(); ++it)
    {
      const vector<double> vpar = it->second.getVarParams();
      if (vpar.empty()) continue;

      cout << "    - chi^2      : " << it->first << endl;
      cout << "      var_params : [";
      copy(vpar.begin(), vpar.end()-1, ostream_iterator<double>(cout, ", "));
      cout << vpar.back() << "]" << endl << endl;
    }
  }

  // The vector tmp_params is the same as sim_map but without the
  // corresponding chi^2 values.
  vector<Parameters> tmp_params;
  tmp_params.reserve(iterations);
  transform(sim_map.begin(), sim_map.end(), back_inserter(tmp_params),
            pair_select2nd<pair<double, Parameters> >());

  const vector<vector<MinuitParameter> > all_sim_par = transpose(tmp_params);
  vector<vector<Error> > retval;

  const int limit[] = { int(iterations * g_normal_1sigma),
                        int(iterations * g_normal_2sigma),
                        int(iterations * g_normal_3sigma) };

  for (size_t i = 0; i < all_sim_par.size(); ++i)
  {
    MinuitParameter orig_par = minParams.Parameter(i);
    if (orig_par.IsFixed()) continue;

    vector<MinuitParameter> vpar = all_sim_par[i];

    vector<MinuitParameter> min_par;
    vector<MinuitParameter> max_par;
    double upper[3];
    double lower[3];

    vector<Error> verr;

    for (size_t j = 0; j < 3; ++j)
    {
      min_par.push_back(*min_element(vpar.begin(), vpar.begin() + limit[j]));
      max_par.push_back(*max_element(vpar.begin(), vpar.begin() + limit[j]));

      upper[j] = max_par[j].Value() - orig_par.Value();
      lower[j] = orig_par.Value() - min_par[j].Value();

      verr.push_back(
        Error(orig_par.Number(), orig_par.GetName(), upper[j], lower[j]));
    }

    retval.push_back(verr);
  }

  chisq_func->setDataPoints(orig_dps);
  chisq_func->setParameters(orig_params);
  chisq_func->chiSq(minParams);

  chisq_func->enableProcessing();
  chisq_func->processBootstrap(&retval, iterations);
  return retval;
}

} // namespace Kaimini

// vim: sw=2 tw=78
