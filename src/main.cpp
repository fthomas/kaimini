// Kaimini
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

#include <string>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MnMinimize.h>
#include <Minuit2/MnPrint.h>
#include "kaimini.h"
#include "sphenofit.h"

using namespace std;
using namespace Kaimini;
using namespace ROOT::Minuit2;

int main(int argc, char* argv[])
{
  // Use SPheno's defaul input/output filenames as our own default filenames.
  string input_file  = "LesHouches.in";
  string output_file = "SPheno.spc";

  // Set our (user-supplied) i/o filenames as SPheno's i/o filenames.
  parse_command_line(argc, argv, input_file, output_file);

  SPhenoFit fit(input_file);

  MnMinimize minimizer(fit, fit.getIntParameters());
  FunctionMinimum minimum = minimizer();

  // Call chiSquare() again with the parameter values of the minimal
  // chi^2 to assure that the same values are set in SPheno.
  fit.setIntParameters( minimum.UserParameters());
  //fit.chiSquare();

  cout << minimum << endl;


  fit.writeResult(output_file);

  return 0;
}

// vim: sw=2 tw=78
