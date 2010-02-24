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
#include <vector>
#include <utility>
#include "datapoint.h"
#include "erroranalysis.h"
#include "genericfit.h"
#include "gsldriver.h"
#include "minuitdriver.h"
#include "parameters.h"
#include "random.h"

using namespace std;

namespace Kaimini {

template<typename Driver> void
bootstrapping(GenericFit* fit, const Parameters& minPar,
              Driver* driver, Parameters (Driver::*minimize)())
{
  fit->chiSquare(minPar);

  vector<DataPoint> min_dps = fit->getDataPoints();
  for (vector<DataPoint>::iterator dp = min_dps.begin();
       dp != min_dps.end(); ++dp)
  { if (dp->use) dp->value = dp->calcValue; }

  vector<DataPoint> rnd_dps = min_dps;
  vector<pair<double, Parameters> > rnd_params;

  for (int i= 0; i < 10; ++i)
  {
    rnd_dps = min_dps;
    for (vector<DataPoint>::iterator dp = rnd_dps.begin();
         dp != rnd_dps.end(); ++dp)
    { if (dp->use) dp->value += g_rnd.randNormal(dp->error); }

    fit->setDataPoints(rnd_dps);
    Parameters par = (driver->*minimize)();

    fit->setDataPoints(min_dps);
    rnd_params.push_back(make_pair(fit->chiSquare(par), par));
    // check if chi^2 is zero!
    cout << rnd_params[i].first << "  " << rnd_params[i].second.Value(0)<< endl;
  }

  cout << endl;
  sort(rnd_params.begin(), rnd_params.end());

  //cout << endl;
  for (int i= 0; i < 10; ++i)
    cout << rnd_params[i].first << "  " << rnd_params[i].second.Value(0)<< endl;
  // run chi^2 again
}


template void
bootstrapping<GSLDriver>(GenericFit*, const Parameters&,
  GSLDriver*, Parameters (GSLDriver::*)());


template void
bootstrapping<MinuitDriver>(GenericFit*, const Parameters&,
  MinuitDriver*, Parameters (MinuitDriver::*)());

} // namespace Kaimini

// vim: sw=2 tw=78
