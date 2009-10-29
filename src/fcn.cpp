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

#include <stdexcept>
#include <vector>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_vector.h>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MnMinimize.h>
#include <Minuit2/MnPrint.h>
#include <Minuit2/MnUserParameters.h>
#include "fcn.h"
#include "slha.h"
#include "spheno.h"

using namespace std;
using namespace ROOT::Minuit2;
using namespace SPheno;

namespace FISP
{

/* static */
Observables<Fcn::nObs> Fcn::obs;


/* static */
double Fcn::chiSquare(const vector<double>& par)
{
  const size_t size = par.size();

  for (size_t i = 0; i < 3 && i < size; ++i)
  {
    Model_Data_eps[i].re = par.at(i);
  }

  for (size_t i = 3; i < 6 && i < size; ++i)
  {
    Model_Data_vevL[i] = par.at(i);
  }

  double chiSq = 0;

  RPtools_ChiSquare(obs.use, obs.value, obs.error, &chiSq,
                    &InputOutput_Add_Rparity, &Control_delta_mass,
                    &SPhenoDouble_m_Gut, &SPhenoDouble_kont);

#ifdef DEBUG
  cout.setf(ios::scientific);
  cout.precision(8);
  cout << chiSq << endl;
#endif

  return chiSq;
}


/* static */
double Fcn::chiSquare(const gsl_vector* v, void* /* params */ = 0)
{
  vector<double> par(v->size);
  for (size_t i = 0; i < v->size; ++i)
  {
    par[i] = gsl_vector_get(v, i);
  }
  return chiSquare(par);
}


void Fcn::setParameters(const Slha& input)
{
  upar.Add("epsilon_1", to_double(input("RVKAPPAIN")(1)[2]),
           to_double(input("RPVFitParams")(1)[3]));
  upar.Add("epsilon_2", to_double(input("RVKAPPAIN")(2)[2]),
           to_double(input("RPVFitParams")(2)[3]));
  upar.Add("epsilon_3", to_double(input("RVKAPPAIN")(3)[2]),
           to_double(input("RPVFitParams")(3)[3]));
  upar.Add("v_L1", to_double(input("RVSNVEVIN")(1)[2]),
           to_double(input("RPVFitParams")(4)[3]));
  upar.Add("v_L2", to_double(input("RVSNVEVIN")(2)[2]),
           to_double(input("RPVFitParams")(5)[3]));
  upar.Add("v_L3", to_double(input("RVSNVEVIN")(3)[2]),
           to_double(input("RPVFitParams")(6)[3]));

  if ("0" == input("RPVFitParams")(1)[2]) upar.Fix("epsilon_1");
  if ("0" == input("RPVFitParams")(2)[2]) upar.Fix("epsilon_2");
  if ("0" == input("RPVFitParams")(3)[2]) upar.Fix("epsilon_3");
  if ("0" == input("RPVFitParams")(4)[2]) upar.Fix("v_L1");
  if ("0" == input("RPVFitParams")(5)[2]) upar.Fix("v_L2");
  if ("0" == input("RPVFitParams")(6)[2]) upar.Fix("v_L3");
}


/* static */
void Fcn::setObservables(const Slha& input)
{
  for (int i = 0; i < nObs; ++i)
  {
    try
    {
      obs.use[i]   =    to_int(input("RPVFitObserv")(i+7)[2]);
      obs.value[i] = to_double(input("RPVFitObserv")(i+7)[3]);
      obs.error[i] = to_double(input("RPVFitObserv")(i+7)[4]);
    }
    catch (out_of_range)
    {
      cerr << "Note (Fcn::setObservables): observable with index "
           << (i+7) << " not found" << endl;
      obs.use[i]   = 0;
      obs.value[i] = 0.;
      obs.error[i] = 0.;
    }
  }
}


void Fcn::simpleFitMinuit()
{
  MnMinimize minimizer(*this, upar);
  FunctionMinimum minimum = minimizer();

  //
  (*this)(minimum.UserParameters().Params());

  cout << minimum << endl;
}

void Fcn::simpleFitGsl()
{
  //gsl_multimin_function function;
// function.n
  /*
    gsl_multimin_function my;
    my.n = 1;
    my.f = &Fcn::chiSquare;
    my.params = 0;

    const gsl_multimin_fminimizer_type* T;
    gsl_multimin_fminimizer* m2;

    //T = gsl_multimin_fminimizer_nmsimplex;
    //T = gsl_multimin_fminimizer_nmsimplex2;
    T = gsl_multimin_fminimizer_nmsimplex2rand;
    m2 = gsl_multimin_fminimizer_alloc(T,1);

    gsl_vector* x = gsl_vector_alloc(1);
    gsl_vector_set(x, 0, 1.0E-5);
    //gsl_vector_set(x, 1, 0.00001);


    gsl_vector* ss = gsl_vector_alloc(1);
    gsl_vector_set(ss, 0, 1.0E-6);
    //gsl_vector_set(ss, 1, 0.000001);

    gsl_multimin_fminimizer_set(m2, &my, x, ss);
  size_t iter = 0;
  int status = 0;
  gsl_multimin_fminimizer_iterate (m2);

         do
           {
             ++iter;
             status = gsl_multimin_fminimizer_iterate (m2);

             //if (status)
             //  break;

             //status = gsl_multimin_test_gradient (s->gradient, 1e-3);

            // if (status == GSL_SUCCESS)
            //   printf ("Minimum found at:\n");

             printf ("%5d %.5f  %10.5f\n", int(iter),
                     gsl_vector_get (m2->x, 0),
                     m2->fval);

           }
         //while (status == GSL_CONTINUE && iter < 100 );
         while ( iter < 100 );
  */

}

} // namespace FISP

// vim: sw=2 tw=78
