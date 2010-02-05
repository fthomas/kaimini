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

#include <fstream>
#include <vector>
#include "slhaea.h"
#include "slhafit.h"

using namespace std;
using namespace SLHAea;
using namespace Kaimini;

class ExposedSLHAFit : public SLHAFit
{
public:
  double chiSquare(const vector<double>&) const { return 0.; }

  using SLHAFit::setDataPoints;
  using SLHAFit::setParameters;
  using SLHAFit::readDataPoints;
  using SLHAFit::writeParameters;

  using SLHAFit::mParamsExt;
  using SLHAFit::mDataPoints;
};

BOOST_AUTO_TEST_SUITE(TestSLHAFit)

BOOST_AUTO_TEST_CASE(testSetDataPoints)
{
  ifstream fs("slha0.txt");
  SLHA input;
  fs >> input;

  ExposedSLHAFit fit;
  fit.setDataPoints(input);

  BOOST_CHECK(fit.mDataPoints.size() == 5);
  BOOST_CHECK(fit.mDataPoints[4].name == "dp5");
}

BOOST_AUTO_TEST_CASE(testSetParameters)
{
  ifstream fs("slha0.txt");
  SLHA input;
  fs >> input;

  ExposedSLHAFit fit;
  fit.setParameters(input);

  BOOST_CHECK(fit.mParamsExt.getParams().size() == 5);
  BOOST_CHECK(fit.mParamsExt.GetName(2) == "par3");
}

BOOST_AUTO_TEST_CASE(testReadDataPoints)
{
  ifstream fs("slha0.txt");
  SLHA input;
  fs >> input;

  ExposedSLHAFit fit;
  fit.setDataPoints(input);
  BOOST_CHECK(fit.mDataPoints[0].calcValue == 0.);

  fit.readDataPoints(input);
  BOOST_CHECK(fit.mDataPoints[0].calcValue != 0.);
}

BOOST_AUTO_TEST_CASE(testWriteParameters)
{
  ifstream fs("slha0.txt");
  SLHA input;
  fs >> input;

  ExposedSLHAFit fit;
  fit.setParameters(input);
  string before1 = input["RVKAPPAIN"]["1"][1];
  string before2 = input["RVSNVEVIN"]["2"][1];

  vector<double> par;
  par.push_back(66.);
  par.push_back(77.);
  par.push_back(88.);
  par.push_back(99.);
  par.push_back(10.);
  fit.writeParameters(par, input);
  string after1 = input["RVKAPPAIN"]["1"][1];
  string after2 = input["RVSNVEVIN"]["2"][1];

  BOOST_CHECK(before1 != after1);
  BOOST_CHECK(before2 != after2);
}

BOOST_AUTO_TEST_SUITE_END()

// vim: sw=2 tw=78
