// Kaimini
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
#include "gsldriver.h"

using namespace std;

namespace Kaimini {

// static
GenericFit* GSLDriver::mspFit = 0;

// static
Parameters GSLDriver::msPar = Parameters();


// explicit
GSLDriver::GSLDriver(GenericFit* fit)
{
  mspFit = fit;
  msPar  = fit->getIntParameters();
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
  return mspFit->chiSquare(msPar);
}


void GSLDriver::runSimplex()
{
  gsl_multimin_function func;
  func.f = &GSLDriver::chiSquare;
  func.n = msPar.VariableParameters();
  func.params = 0;

  const gsl_multimin_fminimizer_type* mtype;
  gsl_multimin_fminimizer* minimizer;

  mtype = gsl_multimin_fminimizer_nmsimplex2rand;
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
  // cached variables in mpFit correspond to the found minimum.
  chiSquare(minimizer->x);

  cout.setf(ios::scientific);
  cout.precision(8);

  cout << "Iterations: " << iter << endl;
  cout << "Minimal chi^2: " << minimizer->fval << endl;
  for (size_t i = 0; i < func.n; ++i)
  {
    cout << "Parameter " << i << ": " << gsl_vector_get(minimizer->x, i)
         << endl;
    cout << "          " << i << ": " <<
gsl_vector_get(gsl_multimin_fminimizer_x(minimizer), i) << endl;
  }

  gsl_vector_free(v);
  gsl_vector_free(step_sizes);
  gsl_multimin_fminimizer_free(minimizer);
}


void GSLDriver::runSimulatedAnnealing()
{
  const gsl_rng_type* T;
  gsl_rng* r;

  gsl_rng_env_setup();

  T = gsl_rng_default;
  r = gsl_rng_alloc(T);

  gsl_siman_params_t params
       = {20, 10, 0.0005,
          1.0, 0.0008, 1.03, 2.0e-6};

  gsl_vector* x_initial = msPar.getVarParamsGSLVec();

  gsl_siman_solve(r, x_initial, GSLDriver::chiSquare,
    gsl_vector_step_random, gsl_vector_dist, NULL,
    NULL, NULL, NULL,
    sizeof(gsl_vector), params);

  gsl_vector_free(x_initial);
  gsl_rng_free(r);
}


double gsl_vector_minkowski_dist(void* v1, void* v2, double p)
{
  gsl_vector* vx = static_cast<gsl_vector*>(v1);
  gsl_vector* vy = static_cast<gsl_vector*>(v2);

  gsl_vector* diff = gsl_vector_alloc(vx->size);
  gsl_vector_memcpy(diff, vx);
  gsl_vector_sub(diff, vy);

  double retval = 0.;
  for (size_t i = 0; i < diff->size; ++i)
  { retval += pow(abs(gsl_vector_get(diff, i)), p); }
  retval = pow(retval, 1./p);

  gsl_vector_free(diff);
  return retval;
}


void gsl_vector_step_random(const gsl_rng* r, void* v, double step_size)
{
  gsl_vector* v_old = static_cast<gsl_vector*>(v);
  const size_t n = v_old->size;
  gsl_vector* v_new = gsl_vector_alloc(n);

  // Set normal distributed random numbers as elements of v_new and
  // compute the euclidean norm of this vector.
  double length = 0.;
  for (size_t i = 0; i < n; ++i)
  {
    double* v_new_i = gsl_vector_ptr(v_new, i);
    *v_new_i = gsl_ran_ugaussian(r);
    length += pow(*v_new_i, 2);
  }
  length = sqrt(length);

  // Scale v_new so that the elements of v_new are uniformly distributed
  // within an n-sphere of radius step_size.
  const double scale =
    pow(pow(step_size, n) * gsl_rng_uniform_pos(r), 1.0/n) / length;
  gsl_vector_scale(v_new, scale);

  gsl_vector_add(v_old, v_new);
}

} // namespace Kaimini

// vim: sw=2 tw=78
