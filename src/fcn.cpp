// RPVFit
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
#include <gsl/gsl_vector.h>
#include <Minuit2/MnUserParameters.h>
#include "fcn.h"
#include "slha.h"
#include "spheno.h"

using namespace std;
using namespace ROOT::Minuit2;

/* static */    int Fcn::observInclude[4];
/* static */ double Fcn::observExpected[4];
/* static */ double Fcn::observSigma[4];


/* static */
double Fcn::chiSquare(const vector<double>& par)
{
  size_t size = par.size();

  for (size_t i = 0; i < 3 && i < size; i++)
  {
    model_data_mp_eps_[i].re = par.at(i);
  }

  for (size_t i = 3; i < 6 && i < size; i++)
  {
    model_data_mp_vevl_[i] = par.at(i);
  }

  double chiSq = 0;

  rptools_mp_chisquare_(observInclude, observExpected, observSigma, &chiSq,
    &inputoutput_mp_add_rparity_, &control_mp_delta_mass_,
    &sphenodouble_mp_m_gut_, &sphenodouble_mp_kont_);

#ifdef DEBUG
  cout.setf(ios::scientific);
  cout.precision(8);
  cout << chiSq << endl;
#endif

  return chiSq;
}


/* static */
double Fcn::chiSquare(const gsl_vector* v, void* params)
{
  vector<double> par(v->size);
  for (size_t i = 0; i < v->size; i++)
  {
    par[i] = gsl_vector_get(v, i);
  }
  return chiSquare(par);
}


double Fcn::operator()(const vector<double>& par) const
{
  return chiSquare(par);
}


double Fcn::Up() const { return 1.; }


void Fcn::setParameters(const Slha& input)
{
  upar.Add("epsilon_1", to_double(input("RVKAPPAIN")(1)[2]),
    to_double(input("RPVFitUpar")(1)[3]));
  upar.Add("epsilon_2", to_double(input("RVKAPPAIN")(2)[2]),
    to_double(input("RPVFitUpar")(2)[3]));
  upar.Add("epsilon_3", to_double(input("RVKAPPAIN")(3)[2]),
    to_double(input("RPVFitUpar")(3)[3]));
  upar.Add("v_L1", to_double(input("RVSNVEVIN")(1)[2]),
    to_double(input("RPVFitUpar")(4)[3]));
  upar.Add("v_L2", to_double(input("RVSNVEVIN")(2)[2]),
    to_double(input("RPVFitUpar")(5)[3]));
  upar.Add("v_L3", to_double(input("RVSNVEVIN")(3)[2]),
    to_double(input("RPVFitUpar")(6)[3]));

  if ("0" == input("RPVFitUpar")(1)[2]) upar.Fix("epsilon_1");
  if ("0" == input("RPVFitUpar")(2)[2]) upar.Fix("epsilon_2");
  if ("0" == input("RPVFitUpar")(3)[2]) upar.Fix("epsilon_3");
  if ("0" == input("RPVFitUpar")(4)[2]) upar.Fix("v_L1");
  if ("0" == input("RPVFitUpar")(5)[2]) upar.Fix("v_L2");
  if ("0" == input("RPVFitUpar")(6)[2]) upar.Fix("v_L3");
}


/* static */
void Fcn::setObservables(const Slha& input)
{
  for (int i = 0; i < 4; i++)
  {
    try
    {
      observInclude[i]  =    to_int(input("RPVFitObserv")(i+7)[2]);
      observExpected[i] = to_double(input("RPVFitObserv")(i+7)[3]);
      observSigma[i]    = to_double(input("RPVFitObserv")(i+7)[4]);
    }
    catch (out_of_range)
    {
      cerr << "Note (Fcn::setObservables): observable with index "
           << (i+7) << " not found" << endl;
      observInclude[i]  = 0;
      observExpected[i] = 0.;
      observSigma[i]    = 0.;
    }
  }
}

// vim: sw=2 tw=78
