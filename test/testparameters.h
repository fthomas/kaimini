// Kaimini
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

#include <gsl/gsl_vector.h>
#include "parameters.h"

using namespace std;
using namespace Kaimini;

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

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
