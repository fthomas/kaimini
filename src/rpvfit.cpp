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

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_vector.h>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MnMinimize.h>
#include <Minuit2/MnPrint.h>
#include "rpvfit.h"
#include "slha.h"
#include "spheno.h"

using namespace std;
using namespace ROOT::Minuit2;

namespace FISP {

/* static */
RpvFit* RpvFit::mspObj;


double RpvFit::chiSquare(const vector<double>& v) const
{
  const size_t size = v.size();
  for (size_t i = 0; i < 3 && i < size; ++i)
  {
    Model_Data_eps[i].re = v.at(i);
  }
  for (size_t i = 3; i < 6 && i < size; ++i)
  {
    Model_Data_vevL[i] = v.at(i);
  }

  double calc_obs[msObsCnt];
  RPtools_Calculate_RP_Observables(calc_obs, &InputOutput_Add_Rparity,
    &Control_delta_mass, &SPhenoDouble_m_Gut, &SPhenoDouble_kont);

  double chisq = 0;
  for (int i = 0; i < msObsCnt; ++i)
  {
    if (mObs.at(i).use)
    {
      chisq += pow(mObs.at(i).value - calc_obs[i], 2) /
               pow(mObs.at(i).error, 2);
    }
  }

  cout.precision(8);
  for (size_t i = 0; i < size; ++i)
  {
    cout << "p" << i << ": " << v[i] << endl;
  }
  cout << "chisq: " << chisq << endl << endl;

  return chisq;
}


/* static */
double RpvFit::chiSquare(const gsl_vector* v, void* = 0)
{
  mspObj->mPar.setVarParams(v);
  return mspObj->chiSquare();
}


void RpvFit::setParameters(const Slha& input)
{
  mPar.Add("epsilon_1", to_double(input("RVKAPPAIN")(1)[2]), 0.);
  mPar.Add("epsilon_2", to_double(input("RVKAPPAIN")(2)[2]), 0.);
  mPar.Add("epsilon_3", to_double(input("RVKAPPAIN")(3)[2]), 0.);
  mPar.Add("v_L1", to_double(input("RVSNVEVIN")(1)[2]), 0.);
  mPar.Add("v_L2", to_double(input("RVSNVEVIN")(2)[2]), 0.);
  mPar.Add("v_L3", to_double(input("RVSNVEVIN")(3)[2]), 0.);

  for (int i = 0; i < 6; ++i)
  {
    mPar.SetError(i, to_double(input("RPVFitParams")(i+1)[3]));
    if ("0" == input("RPVFitParams")(i+1)[2]) mPar.Fix(i);
  }
}


void RpvFit::setObservables(const Slha& input)
{
  for (int i = 1; i <= msObsCnt; ++i)
  {
    try
    {
      Observable o =
      {
        to_int(input("RPVFitObserv")(i)[2]),
        to_double(input("RPVFitObserv")(i)[3]),
        to_double(input("RPVFitObserv")(i)[4])
      };
      mObs.push_back(o);
    }
    catch (out_of_range)
    {
      cerr << "Note (RpvFit::setObservables): observable with index " << i
           << " not found" << endl;
      Observable o = {false, 0., 0.};
      mObs.push_back(o);
    }
  }
}


void RpvFit::simpleFitMinuit()
{
  MnMinimize minimizer(*this, mPar);
  FunctionMinimum minimum = minimizer();
  cout << minimum << endl;

  // Call chiSquare() again with the parameter values of the minimal
  // chi^2 to assure that same values are set in SPheno.
  chiSquare(minimum.UserParameters().Params());
}


void RpvFit::simpleFitGsl()
{
}


void RpvFit::simpleMinimizeGsl()
{
  mspObj = this;

  gsl_multimin_function func;
  func.f = &RpvFit::chiSquare;
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
}

} // namespace FISP

// vim: sw=2 tw=78
