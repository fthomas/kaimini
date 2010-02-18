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

#include <cstdlib>
#include <ostream>
#include <string>
#include <boost/filesystem.hpp>
#include <Minuit2/MnPrint.h>
//#include "erroranalysis.h"
#include "gsldriver.h"
#include "kaimini.h"
#include "minuitdriver.h"
#include "simpleslhafit.h"

using namespace std;
using namespace Kaimini;
namespace fs = boost::filesystem;

int main(int argc, char* argv[])
{
  cout.precision(8);
  cout.setf(ios_base::scientific);

  string input_file  = "kaimini.in";
  string output_file = "kaimini.out";
  parse_command_line(argc, argv, &input_file, &output_file);

  if (!fs::exists(input_file))
  {
    cerr << "Error: input file ‘" << input_file << "’ does not exist"
         << endl;
    exit(EXIT_FAILURE);
  }

  SimpleSLHAFit fit(input_file);
  //GSLDriver gsl_dr(&fit);
  //gsl_dr.runSimplex();
  //gsl_dr.runSimulatedAnnealing();

  MinuitDriver minuit_dr(&fit);
  //cout << minuit_dr.runMigrad();
  cout << minuit_dr.runMinimize();
  cout << minuit_dr.getFunctionMinimum();
  //cout << minuit_dr.runMinos();

  //bounding_box(&fit, &minuit_dr, &MinuitDriver::runSimplex, fit.getIntParameters());

  fit.tearDown(output_file);

  return 0;
}

// vim: sw=2 tw=78
