#include <vector>
#include <Minuit2/MnUserParameters.h>
#include "fcn.h"
#include "slha.h"
#include "spheno.h"

using namespace std;
using namespace ROOT::Minuit2;

double Fcn_observExp[2];
double Fcn_sigma[2];

 
double Fcn::operator()(const vector<double>& par) const
{
    double chiSq = 0;

    for (int i = 0; i < 3; i++)
      model_data_mp_eps_[i].re = par.at(i);

    for (int i = 3; i < 6; i++)
      model_data_mp_vevl_[i] = par.at(i);

    rptools_mp_chisquare_(Fcn_observExp, Fcn_sigma, &chiSq,
      &inputoutput_mp_add_rparity_, &control_mp_delta_mass_,
      &sphenodouble_mp_m_gut_, &sphenodouble_mp_kont_);
    return chiSq;
}


double Fcn::Up() const
{
  return 1.;
}


void Fcn::setUserParameters(const Slha& input)
{
  upar.Add("epsilon_1", to_double(input("RVKAPPAIN")(1)[2]), 1.e-16);
  upar.Add("epsilon_2", to_double(input("RVKAPPAIN")(2)[2]), 1.e-16);
  upar.Add("epsilon_3", to_double(input("RVKAPPAIN")(3)[2]), 1.e-16);

  if ("0" == input("RPVFitUpar")(1)[2]) upar.Fix("epsilon_1");
  if ("0" == input("RPVFitUpar")(2)[2]) upar.Fix("epsilon_2");
  if ("0" == input("RPVFitUpar")(3)[2]) upar.Fix("epsilon_3");

  upar.Add("v_L1", to_double(input("RVSNVEVIN")(1)[2]), 1.e-16);
  upar.Add("v_L2", to_double(input("RVSNVEVIN")(2)[2]), 1.e-16);
  upar.Add("v_L3", to_double(input("RVSNVEVIN")(3)[2]), 1.e-16);

  if ("0" == input("RPVFitUpar")(4)[2]) upar.Fix("v_L1");
  if ("0" == input("RPVFitUpar")(5)[2]) upar.Fix("v_L2");
  if ("0" == input("RPVFitUpar")(6)[2]) upar.Fix("v_L3");
}


void Fcn::setFixedParameters(const Slha& input) const
{
  Fcn_observExp[0] = to_double(input("RPVFitObserv")(7)[2]);
  Fcn_observExp[1] = to_double(input("RPVFitObserv")(8)[2]);

  Fcn_sigma[0] = to_double(input("RPVFitObserv")(7)[3]);
  Fcn_sigma[1] = to_double(input("RPVFitObserv")(8)[3]);
}

// vim: sw=2 tw=78
