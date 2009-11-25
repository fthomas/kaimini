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
#include "rpvfit.h"
#include "slha.h"
#include "spheno.h"

using namespace std;

namespace FISP {

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

  mChiSq = 0;
  for (size_t i = 0; i < msObsCnt; ++i)
  {
    mObs.at(i).calcValue = calc_obs[i];
    if (mObs.at(i).use)
    {
      mObs.at(i).wSqResidual = pow(mObs.at(i).value - calc_obs[i], 2) /
                               pow(mObs.at(i).error, 2);
      mChiSq += mObs.at(i).wSqResidual;
    }
  }

  cout.setf(ios::scientific);
  cout.precision(8);
  for (size_t i = 0; i < size; ++i)
  {
    cout << "p" << i << ": " << v[i] << endl;
  }
  cout << "chisq: " << mChiSq << endl << endl;

  return mChiSq;
}


void RpvFit::setParameters(const Slha& input)
{
  mPar = Parameters();
  mPar.Add("epsilon_1", to_double(input("RVKAPPAIN")(1)[2]), 0.);
  mPar.Add("epsilon_2", to_double(input("RVKAPPAIN")(2)[2]), 0.);
  mPar.Add("epsilon_3", to_double(input("RVKAPPAIN")(3)[2]), 0.);
  mPar.Add("v_L1", to_double(input("RVSNVEVIN")(1)[2]), 0.);
  mPar.Add("v_L2", to_double(input("RVSNVEVIN")(2)[2]), 0.);
  mPar.Add("v_L3", to_double(input("RVSNVEVIN")(3)[2]), 0.);

  for (size_t i = 0; i < mPar.Params().size(); ++i)
  {
    mPar.SetError(i, to_double(input("RPVFitParamsIn")(i+1)[3]));
    if ("0" == input("RPVFitParamsIn")(i+1)[2]) mPar.Fix(i);
  }
}


void RpvFit::setObservables(const Slha& input)
{
  mObs.clear();
  for (size_t i = 1; i <= msObsCnt; ++i)
  {
    try
    {
      Observable obs(
        input("RPVFitObservIn")(i)[5],
        to_bool(input("RPVFitObservIn")(i)[2]),
        to_double(input("RPVFitObservIn")(i)[3]),
        to_double(input("RPVFitObservIn")(i)[4])
      );
      mObs.push_back(obs);
    }
    catch (out_of_range)
    {
      cerr << "Note (RpvFit::setObservables): observable with index " << i
           << " not found" << endl;
      Observable obs;
      mObs.push_back(obs);
    }
  }
}

} // namespace FISP

// vim: sw=2 tw=78
