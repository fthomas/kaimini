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

Parameters GenericDriver::runRandomWalk(const Parameters& startParams)
{
  Parameters curr_params = startParams;
  Parameters best_params = curr_params;
  Parameters new_params  = curr_params;
  double curr_chisq = mpFunc->chiSq(curr_params);
  double best_chisq = curr_chisq;
  double new_chisq  = curr_chisq;

  const int iterations = 200;
  const double limit = 10.;

  int penalty = 0;
  double step_factor = 1.;

  for (int i = 0; i < iterations; ++i)
  {
    new_params.stepRandom(step_factor);
    new_chisq = mpFunc->chiSq(new_params);

    if (new_chisq <= best_chisq)
    {
      best_params = curr_params = new_params;
      best_chisq  = curr_chisq  = new_chisq;
    }
    else if (new_chisq <= limit)
    {
      curr_params = new_params;
      curr_chisq  = new_chisq;
    }
    else
    {
      new_params = best_params;
      if (++penalty > 10)
      {
        penalty = 0;
        step_factor *= 0.7;
      }
    }
  }

  mpFunc->chiSq(best_params);
  mpFunc->clearResults();
  mpFunc->processParameters(&best_params);
  mpFunc->processDataPoints();
  return best_params;
}


Parameters GenericDriver::runRandomHillClimbing(const Parameters& startParams)
{
  Parameters curr_params = startParams;
  Parameters best_params = curr_params;
  Parameters new_params  = curr_params;
  double curr_chisq = mpFunc->chiSq(curr_params);
  double best_chisq = curr_chisq;
  double new_chisq  = curr_chisq;

  const int max_steps = 10000;
  int penalty_count   = 0;
  double step_factor  = 1.;

  for (int i = 0; i < max_steps; ++i)
  {
    new_params.stepRandom(step_factor);
    new_chisq = mpFunc->chiSq(new_params);

    if (new_chisq < best_chisq)
    {
      best_params = new_params;
      best_chisq  = new_chisq;
    }

    if (new_chisq < curr_chisq)
    {
      curr_params = new_params;
      curr_chisq  = new_chisq;
      --penalty_count;
    }
    else
    {
      new_params = curr_params;
      ++penalty_count;

      if (penalty_count > 200)
      {
        penalty_count = 0;
        step_factor   = 1.;

        curr_params.stepRandom(10.);
        curr_chisq = mpFunc->chiSq(curr_params);
        new_params = curr_params;
      }
      else if (penalty_count > 50)
      {
        step_factor *= 0.9;
      }
    }
  }

  mpFunc->chiSq(best_params);
  mpFunc->clearResults();
  mpFunc->processParameters(&best_params);
  mpFunc->processDataPoints();
  return best_params;
}


Parameters GenericDriver::runMetropolis(const Parameters& startParams)
{
  Random::uniform_real_gen
    rand01(g_rnd.engine, Random::uniform_real_distribution(0., 1.));

  Parameters curr_params = startParams;
  Parameters best_params = curr_params;
  Parameters new_params  = curr_params;
  double curr_chisq = mpFunc->chiSq(curr_params);
  double best_chisq = curr_chisq;
  double new_chisq  = curr_chisq;

  const int    max_calls = 100000;
  const double min_chisq = 1.e-3;

  const double temp  = 0.1;
  double probability = 0.;

  for (int i = 0; i < max_calls && best_chisq > min_chisq; ++i)
  {
    const double step_factor =
        (max_calls - i) / static_cast<double>(max_calls);

    new_params.stepRandom(step_factor);
    new_chisq = mpFunc->chiSq(new_params);

    if (new_chisq <= best_chisq)
    {
      best_params = curr_params = new_params;
      best_chisq  = curr_chisq  = new_chisq;
      continue;
    }

    probability = exp(-(new_chisq - curr_chisq) / temp);
    if (probability > rand01())
    {
      curr_params = new_params;
      curr_chisq  = new_chisq;
    }
    else new_params = curr_params;
  }

  mpFunc->chiSq(best_params);
  mpFunc->clearResults();
  mpFunc->processParameters(&best_params);
  mpFunc->processDataPoints();
  return best_params;
}


Parameters GenericDriver::runSimulatedAnnealing(const Parameters& startParams)
{
  Random::uniform_real_gen
    rand01(g_rnd.engine, Random::uniform_real_distribution(0., 1.));

  Parameters curr_params = startParams;
  Parameters best_params = curr_params;
  Parameters new_params  = curr_params;
  double curr_chisq = mpFunc->chiSq(curr_params);
  double best_chisq = curr_chisq;
  double new_chisq  = curr_chisq;

  const double min_chisq = 1.e-3;
  const double t_initial = 10.0;
  const double t_minimal = 0.001;
  const double mu_t      = 1.001;

  double t_curr      = t_initial;
  double probability = 0.;

  while (t_minimal < (t_curr /= mu_t) && best_chisq > min_chisq)
  {
    const double step_factor = t_curr / t_initial;

    new_params.stepRandom(step_factor);
    new_chisq = mpFunc->chiSq(new_params);

    if (new_chisq <= best_chisq)
    {
      best_params = curr_params = new_params;
      best_chisq  = curr_chisq  = new_chisq;
      continue;
    }

    probability = exp(-(new_chisq - curr_chisq) / t_curr);
    if (probability > rand01())
    {
      curr_params = new_params;
      curr_chisq  = new_chisq;
    }
    else new_params = curr_params;
  }

  mpFunc->chiSq(best_params);
  mpFunc->clearResults();
  mpFunc->processParameters(&best_params);
  mpFunc->processDataPoints();
  return best_params;
}

} //namespace Kaimini

// vim: sw=2 tw=78
