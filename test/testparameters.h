// Kaimini, a general purpose fitting frontend
// Copyright © 2009-2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#include <gsl/gsl_vector.h>
#include "parameters.h"

using namespace std;
using namespace Kaimini;
using namespace ROOT::Minuit2;

BOOST_AUTO_TEST_SUITE(TestParameters)

BOOST_AUTO_TEST_CASE(testVarParams)
{
  Parameters par;
  par.Add("p1", 1.1, 0.1);
  par.Add("p2", 2.2, 0.1);
  par.Add("p3", 3.3, 0.);
  par.Fix("p2");
  par.Fix("p3");

  BOOST_CHECK(par.getParams().size() == 3);
  BOOST_CHECK(par.getStepSizes().size() == 3);

  BOOST_CHECK(par.getVarParams().size() == 1);
  BOOST_CHECK(par.getVarStepSizes().size() == 1);

  gsl_vector* vpar_gsl = par.getVarParamsGSLVec();
  gsl_vector* vss_gsl = par.getVarStepSizesGSLVec();

  BOOST_CHECK(vpar_gsl->size == 1);
  BOOST_CHECK(vss_gsl->size == 1);

  gsl_vector_free(vpar_gsl);
  gsl_vector_free(vss_gsl);

  par.Release("p3");
  BOOST_CHECK(par.getVarParams().size() == 2);
  BOOST_CHECK(par.getVarStepSizes().size() == 2);
}

BOOST_AUTO_TEST_CASE(testTranspose)
{
  vector<Parameters> vecPar;

  Parameters par;
  par.Add("p00", 1.1, 0.);
  par.Add("p01", 1.2, 0.);
  par.Add("p02", 1.3, 0.);
  vecPar.push_back(par);

  par = Parameters();
  par.Add("p10", 2.1, 0.);
  par.Add("p11", 2.2, 0.);
  par.Add("p12", 2.3, 0.);
  vecPar.push_back(par);

  par = Parameters();
  par.Add("p20", 3.1, 0.);
  par.Add("p21", 3.2, 0.);
  par.Add("p22", 3.3, 0.);
  vecPar.push_back(par);

  vector<vector<MinuitParameter> > transPar = transpose(vecPar);
  BOOST_CHECK(transPar.size() == 3);
  BOOST_CHECK(transPar[0].size() == 3);
  BOOST_CHECK(transPar[1].size() == 3);
  BOOST_CHECK(transPar[2].size() == 3);

  BOOST_CHECK(transPar[0][0].GetName() == "p00");
  BOOST_CHECK(transPar[0][1].GetName() == "p10");
  BOOST_CHECK(transPar[0][2].GetName() == "p20");

  BOOST_CHECK(transPar[1][0].GetName() == "p01");
  BOOST_CHECK(transPar[1][1].GetName() == "p11");
  BOOST_CHECK(transPar[1][2].GetName() == "p21");

  BOOST_CHECK(transPar[2][0].GetName() == "p02");
  BOOST_CHECK(transPar[2][1].GetName() == "p12");
  BOOST_CHECK(transPar[2][2].GetName() == "p22");
}

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
