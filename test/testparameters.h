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

#ifndef KAIMINI_TESTPARAMETERS_H
#define KAIMINI_TESTPARAMETERS_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <parameters.h>

using namespace std;

namespace Kaimini {

class TestParameters : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestParameters);
  CPPUNIT_TEST(testVarParams);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}

  void tearDown() {}

  void testVarParams()
  {
    Parameters par;
    par.Add("p1", 1.1, 0.1);
    par.Add("p2", 2.2, 0.1);
    par.Add("p3", 3.3, 0.);
    par.Fix("p2");
    par.Fix("p3");

    CPPUNIT_ASSERT(par.getParams().size() == 3);
    CPPUNIT_ASSERT(par.getStepSizes().size() == 3);

    CPPUNIT_ASSERT(par.getVarParams().size() == 1);
    CPPUNIT_ASSERT(par.getVarStepSizes().size() == 1);

    gsl_vector* vpar_gsl = par.getVarParamsGslVec();
    gsl_vector* vss_gsl = par.getVarStepSizesGslVec();

    CPPUNIT_ASSERT(vpar_gsl->size == 1);
    CPPUNIT_ASSERT(vss_gsl->size == 1);

    gsl_vector_free(vpar_gsl);
    gsl_vector_free(vss_gsl);

    par.Release("p3");
    CPPUNIT_ASSERT(par.getVarParams().size() == 2);
    CPPUNIT_ASSERT(par.getVarStepSizes().size() == 2);
  }
};

} // namespace Kaimini

#endif // KAIMINI_TESTPARAMETERS_H

// vim: sw=2 tw=78
