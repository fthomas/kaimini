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

#include <cstddef>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_siman.h>
#include <gsl/gsl_vector.h>
#include "chisqfunction.h"
#include "gslaux.h"
#include "gsldriver.h"
#include "kaimini.h"

using namespace std;

namespace Kaimini {

// static
ChiSqFunction* GSLDriver::mspFit = 0;

// static
Parameters GSLDriver::msPar = Parameters();


// explicit
GSLDriver::GSLDriver(ChiSqFunction* fit)
{
  mspFit = fit;
  msPar  = fit->getParameters();
}


GSLDriver::~GSLDriver()
{
  mspFit = 0;
  msPar  = Parameters();
}


// static
double GSLDriver::chiSquare(const gsl_vector* v, void*)
{
  msPar.setVarParams(v);
  return mspFit->chiSq(msPar);
}


Parameters GSLDriver::runSimplex()
{
  gsl_multimin_function func;
  func.f = &GSLDriver::chiSquare;
  func.n = msPar.VariableParameters();
  func.params = 0;

  const gsl_multimin_fminimizer_type* mtype;
  gsl_multimin_fminimizer* minimizer;

  mtype = gsl_multimin_fminimizer_nmsimplex;
  minimizer = gsl_multimin_fminimizer_alloc(mtype, func.n);

  gsl_vector* v = msPar.getVarParamsGSLVec();
  gsl_vector* step_sizes = msPar.getVarStepSizesGSLVec();

  gsl_multimin_fminimizer_set(minimizer, &func, v, step_sizes);

  int iter = 0;
  int status = 0;
  double size = 0.;

  do
  {
    ++iter;
    status = gsl_multimin_fminimizer_iterate(minimizer);

    if (status) break;

    size = gsl_multimin_fminimizer_size(minimizer);
    status = gsl_multimin_test_size(size, 1e-10);
  }
  while (GSL_CONTINUE == status && iter < 1000);

  // Run chiSquare with the minimal parameter values again so that
  // cached variables in mspFit correspond to the found minimum.
  chiSquare(minimizer->x);

  if (g_verbose_output)
  {
    cout << "iter   :  " << iter << endl;
    for (size_t i = 0; i < func.n; ++i)
    {
      cout << "vpar_" << i << " :  " << gsl_vector_get(minimizer->x, i)
           << endl;
    }
    cout << "chi^2  :  " << minimizer->fval << endl;
  }

  gsl_vector_free(step_sizes);
  gsl_vector_free(v);
  gsl_multimin_fminimizer_free(minimizer);

  mspFit->processParameters(&msPar);
  mspFit->processDataPoints();
  return msPar;
}


Parameters GSLDriver::runSimulatedAnnealing()
{
  int n_tries       = 200;
  int iters_fixed_t = 2000;
  double step_size  = 1.0;
  double k          = 1.0;
  double t_initial  = 0.008;
  double mu_t       = 1.003;
  double t_min      = 2.0e-6;

  gsl_siman_params_t params =
    { n_tries, iters_fixed_t, step_size, k, t_initial, mu_t, t_min };

  gsl_vector* x_initial = msPar.getVarParamsGSLVec();

  gsl_siman_solve(g_rnd.gsl_engine, x_initial,
    GSLDriver::chiSquare, gsl_vector_step_random, gsl_vector_dist,
    NULL, NULL, NULL, NULL, sizeof(*x_initial), params);

  gsl_vector_free(x_initial);

  mspFit->processParameters(&msPar);
  mspFit->processDataPoints();
  return msPar;
}

} // namespace Kaimini

// vim: sw=2 tw=78
