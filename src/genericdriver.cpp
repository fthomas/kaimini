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

#include <cmath>
#include "genericdriver.h"
#include "kaimini.h"
#include "parameters.h"
#include "random.h"

using namespace std;

namespace Kaimini {

Parameters GenericDriver::runSimulatedAnnealing(const Parameters& startParams)
{
  Random::uniform_real_gen
    rnd(g_rnd.engine, Random::uniform_real_distribution(0., 1.));

  const double t_initial = 100.0;
  const double t_minimal = 1.0;
  const double mu_t      = 1.001;

  Parameters curr_params = startParams;
  Parameters best_params = curr_params;
  Parameters new_params  = curr_params;
  double curr_chisq = mpFunc->chiSq(curr_params);
  double best_chisq = curr_chisq;
  double new_chisq  = curr_chisq;

  double t_curr      = t_initial;
  double probability = 0.;
  double rand01      = 0.;
  bool   move        = false;

  while (t_minimal < (t_curr /= mu_t))
  {
    new_params.stepRandom();
    new_chisq = mpFunc->chiSq(new_params);

    if (new_chisq < best_chisq)
    {
      best_params = new_params;
      best_chisq  = new_chisq;
    }

    probability = exp(-(new_chisq - curr_chisq) / t_curr);
    rand01 = rnd();
    move = probability > rand01;

    if (probability > rand01)
    {
      curr_params = new_params;
      curr_chisq  = new_chisq;
    }

    if (g_verbose_output)
    {
    }
  }

  mpFunc->chiSq(best_params);
  mpFunc->processParameters(&best_params);
  mpFunc->processDataPoints();
  return best_params;
}

} //namespace Kaimini

// vim: sw=2 tw=78
