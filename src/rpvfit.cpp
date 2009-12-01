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
#include <stdexcept>
#include <vector>
#include "rpvfit.h"
#include "slha.h"
#include "spheno.h"

using namespace std;

namespace FISP {

double RpvFit::chiSquare(const vector<double>& v) const
{
  for (size_t i = 0; i < 3; ++i)
  {
    Model_Data_eps[i].re = v.at(i);
    Model_Data_vevL[i] = v.at(i+3);
  }

  double calc_obs[msObsCnt];
  RPtools_Calculate_RP_Observables(calc_obs, &InputOutput_Add_Rparity,
    &Control_delta_mass, &SPhenoDouble_m_Gut, &SPhenoDouble_kont);

  mChiSq = 0;
  for (size_t i = 0; i < msObsCnt; ++i)
  {
    mObs.at(i).calcValue = calc_obs[i];
    if (mObs.at(i).use) mChiSq += mObs.at(i).wSquaredResidual();
  }

  cout.setf(ios::scientific);
  cout.precision(8);
  for (size_t i = 0; i < v.size(); ++i)
  {
    cout << "p" << i << ": " << v.at(i) << endl;
  }
  cout << "chisq: " << mChiSq << endl << endl;

  return mChiSq;
}

/*
double RpvFit::chiSquare(const vector<double>& v) const
{
  Slha in("LesHouches.in");

  for (size_t i = 0; i < 3; ++i)
  {
    in["RVKAPPAIN"].at(i+1)[1] = to_string(v.at(i));
    in["RVSNVEVIN"].at(i+1)[1] = to_string(v.at(i+3));
  }
  in.writeFile("LesHouches.in");

  system("SPheno");

  Slha out("SPheno.spc");

  for (size_t i = 0; i < 4; ++i)
  {
    mObs.at(i).calcValue = to_double(out["SPhenoRP"].at(i+7)[1]);
  }

  mChiSq = 0;
  for (size_t i = 0; i < msObsCnt; ++i)
  {
    if (mObs.at(i).use) mChiSq += mObs.at(i).weightedSquaredResidual();
  }

  cout.setf(ios::scientific);
  cout.precision(8);
  for (size_t i = 0; i < v.size(); ++i)
  {
    cout << "p" << i << ": " << v.at(i) << endl;
  }
  cout << "chisq: " << mChiSq << endl << endl;

  return mChiSq;
}
*/

void RpvFit::setParameters(const Slha& input)
{
  mPar = Parameters();
  mPar.Add("epsilon_1", to_double(input["RVKAPPAIN"]["1"][1]), 0.);
  mPar.Add("epsilon_2", to_double(input["RVKAPPAIN"]["2"][1]), 0.);
  mPar.Add("epsilon_3", to_double(input["RVKAPPAIN"]["3"][1]), 0.);
  mPar.Add("v_L1", to_double(input["RVSNVEVIN"]["1"][1]), 0.);
  mPar.Add("v_L2", to_double(input["RVSNVEVIN"]["2"][1]), 0.);
  mPar.Add("v_L3", to_double(input["RVSNVEVIN"]["3"][1]), 0.);

  for (size_t i = 0; i < mPar.Params().size(); ++i)
  {
    mPar.SetError(i, to_double(input["RPVFitParamsIn"].at(i+1)[2]));
    if ("0" == input["RPVFitParamsIn"].at(i+1)[1]) mPar.Fix(i);
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
        input["RPVFitObservIn"].at(i)[4],
        to_bool(input["RPVFitObservIn"].at(i)[1]),
        to_double(input["RPVFitObservIn"].at(i)[2]),
        to_double(input["RPVFitObservIn"].at(i)[3])
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
