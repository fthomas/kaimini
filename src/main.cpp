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

#include <string>
#include "fisp.h"
#include "rpvfit.h"
#include "slhaea.h"
#include "spheno.h"

using namespace std;
using namespace ROOT::Minuit2;
using namespace FISP;
using namespace SLHAea;

int main(int argc, char* argv[])
{
  // Use SPheno's defaul input/output filenames as our own default filenames.
  string input_file  = "LesHouches.in";
  string output_file = "SPheno.spc";

  // Set our (user-supplied) i/o filenames as SPheno's i/o filenames.
  parse_command_line(argc, argv, &input_file, &output_file);
  set_filenames(input_file, output_file);

  SLHA slha_input(input_file);

  RpvFit rpvfit;
  rpvfit.setParameters(slha_input);
  rpvfit.setObservables(slha_input);

  SPhenoDouble_RunTill_Model_bilinear_Rparity();

  //rpvfit.fitMinuitMinimize();
  //rpvfit.fitMinuitSimplex();
  rpvfit.fitGslSimplex();
  cout << rpvfit.result();

  SPhenoDouble_RunTill_End();

  return 0;
}

// vim: sw=2 tw=78
