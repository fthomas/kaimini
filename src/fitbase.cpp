// FISP - Fitting Interface for SPheno
// Copyright © 2009 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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
#include <iostream>
#include <vector>
#include <boost/format.hpp>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_vector.h>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MnMinimize.h>
#include <Minuit2/MnSimplex.h>
#include <Minuit2/MnPrint.h>
#include "fitbase.h"
#include "parameters.h"
#include "slhame.h"

using namespace std;
using namespace boost;
using namespace ROOT::Minuit2;
using namespace SLHAme;

namespace FISP {

/* static */
FitBase* FitBase::mspObj = 0;


/* static */
double FitBase::chiSquare(const gsl_vector* v, void* = 0)
{
  mspObj->mPar.setVarParams(v);
  return mspObj->chiSquare();
}


Slha& FitBase::result()
{
  mResult.clear();
  if (mPar.Params().empty() || mObs.empty()) return mResult;

  string block = getName() + "Params";
  mResult[block][""] = "BLOCK " + block;

  for (size_t i = 0; i < mPar.Params().size(); ++i)
  {
    mResult[block][""] = str(
      format(" %1% %|5t|%2% %3$16.8E %4$16.8E %|43t|# %5%")
             % (i+1) % !mPar.Parameter(i).IsFixed() % mPar.Value(i)
             % mPar.Error(i) % mPar.Name(i));
  }

  block = getName() + "Observ";
  mResult[block][""] = "BLOCK " + block;

  for (size_t i = 0; i < mObs.size(); ++i)
  {
    mResult[block][""] = str(
      format(" %1% %|5t|%2% %3$16.8E %4$16.8E %5$16.8E %|60t|%6%")
             % (i+1) % mObs.at(i).use % mObs.at(i).calcValue
             % mObs.at(i).value % mObs.at(i).error % mObs.at(i).name);
  }

  block = getName() + "ChiSquare";
  mResult[block][""] = "BLOCK " + block;
  mResult[block][""] = str(
    format(" 0 %1$16.8E %|22t|%2%") % mChiSq % "# chi^2");

  for (size_t i = 0; i < mObs.size(); ++i)
  {
    mResult[block][""] = str(
      format(" %1% %2$16.8E %|22t|%3%")
             % (i+1) % mObs.at(i).wSqResidual % mObs.at(i).name);
  }

  return mResult;
}


void FitBase::fitMinuitMinimize()
{
  MnMinimize minimizer(*this, mPar);
  FunctionMinimum minimum = minimizer();

  // Call chiSquare() again with the parameter values of the minimal
  // chi^2 to assure that the same values are set in SPheno.
  mPar = minimum.UserParameters();
  chiSquare();

  cout << minimum << endl;
}


void FitBase::fitMinuitSimplex()
{
  MnSimplex minimizer(*this, mPar);
  FunctionMinimum minimum = minimizer();

  // Call chiSquare() again with the parameter values of the minimal
  // chi^2 to assure that the same values are set in SPheno.
  mPar = minimum.UserParameters();
  chiSquare();

  cout << minimum << endl;
}


void FitBase::fitGslSimplex()
{
  mspObj = this;

  gsl_multimin_function func;
  func.f = &FitBase::chiSquare;
  func.n = mPar.VariableParameters();
  func.params = 0;

  const gsl_multimin_fminimizer_type* mtype;
  gsl_multimin_fminimizer* minimizer;

  mtype = gsl_multimin_fminimizer_nmsimplex2rand;
  minimizer = gsl_multimin_fminimizer_alloc(mtype, func.n);

  gsl_vector* v = mPar.getVarParamsGslVec();
  gsl_vector* step_sizes = mPar.getVarStepSizesGslVec();

  // Use always 1% of v_i as v_i's step size.
  gsl_vector_memcpy(step_sizes, v);
  gsl_vector_scale(step_sizes, 0.01);

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
  while (GSL_CONTINUE  == status && iter < 1000);

  // Call chiSquare() again with the parameter values of the minimal
  // chi^2 to assure that the same values are set in SPheno.
  chiSquare(minimizer->x);

  cout.setf(ios::scientific);
  cout.precision(8);

  cout << "Iterations: " << iter << endl;
  cout << "Minimal chi^2: " << minimizer->fval << endl;
  for (size_t i = 0; i < func.n; ++i)
  {
    cout << "Parameter " << i << ": " << gsl_vector_get(minimizer->x, i)
         << endl;
  }

  gsl_vector_free(v);
  gsl_vector_free(step_sizes);
  gsl_multimin_fminimizer_free(minimizer);
  mspObj = 0;
}

} // namespace FISP

// vim: sw=2 tw=78
