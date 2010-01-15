// Kaimini
// Copyright © 2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#ifndef KAIMINI_TESTSLHAFIT_H
#define KAIMINI_TESTSLHAFIT_H

#include <fstream>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <slhaea.h>
#include <slhafit.h>

using namespace std;
using namespace SLHAea;

namespace Kaimini {

class TestSLHAFit : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestSLHAFit);
  CPPUNIT_TEST(testSetDataPoints);
  CPPUNIT_TEST(testSetParameters);
  CPPUNIT_TEST(testReadDataPoints);
  CPPUNIT_TEST(testWriteParameters);
  CPPUNIT_TEST(testResult);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}

  void tearDown() {}

  void testSetDataPoints()
  {
    ifstream fs("slha0.txt");
    SLHA input;
    input.read(fs);

    SLHAFit fit;
    fit.setDataPoints(input);
  }

  void testSetParameters()
  {
    ifstream fs("slha0.txt");
    SLHA input;
    input.read(fs);

    SLHAFit fit;
    fit.setParameters(input);
  }

  void testReadDataPoints()
  {
    ifstream fs("slha0.txt");
    SLHA input;
    input.read(fs);

    SLHAFit fit;
    fit.setDataPoints(input);
    fit.readDataPoints(input);
  }

  void testWriteParameters()
  {
    ifstream fs("slha0.txt");
    SLHA input;
    input.read(fs);

    SLHAFit fit;
    fit.setParameters(input);

    vector<double> par;
    par.push_back(66.);
    par.push_back(77.);
    par.push_back(88.);
    par.push_back(99.);
    par.push_back(10.);
    fit.writeParameters(par, input);
    //cout << endl << input << endl;
  }

  void testResult()
  {
    ifstream fs("slha0.txt");
    SLHA input;
    input.read(fs);

    SLHAFit fit;
    fit.setDataPoints(input);
    fit.setParameters(input);
    //cout << endl << fit.result() << endl;
  }

};

} // namespace Kaimini

#endif // KAIMINI_TESTSLHAFIT_H

// vim: sw=2 tw=78
