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

#ifndef KAIMINI_TESTSPHENOFIT_H
#define KAIMINI_TESTSPHENOFIT_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <sphenofit.h>

using namespace std;

namespace Kaimini {

class TestSPhenoFit : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(TestSPhenoFit);
  CPPUNIT_TEST(testConstructor);
  CPPUNIT_TEST(testChiSquare);
  CPPUNIT_TEST(testTearDown);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}

  void tearDown() {}

  void testConstructor()
  {
    SPhenoFit fit("slha1.txt");
  }

  void testChiSquare()
  {
    SPhenoFit fit("slha1.txt");
    vector<double> v;
    v.push_back(7.1E+01);
    fit.chiSquare(v);
  }

  void testTearDown()
  {
    SPhenoFit fit("slha1.txt");
    vector<double> v;
    v.push_back(7.1E+01);
    fit.chiSquare(v);
    fit.tearDown("out.txt");
  }

};

} // namespace Kaimini

#endif // KAIMINI_TESTSPHENOFIT_H

// vim: sw=2 tw=78
