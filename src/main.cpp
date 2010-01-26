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
#include <Minuit2/MnPrint.h>
#include "kaimini.h"
#include "sphenofit.h"
#include "minuitdriver.h"

using namespace std;
using namespace Kaimini;

int main(int argc, char* argv[])
{
  string input_file  = "LesHouches.in";
  string output_file = "SPheno.spc";
  parse_command_line(argc, argv, input_file, output_file);

  SPhenoFit fit(input_file);
  MinuitDriver driver(&fit);
  cout << driver.runMinimize();
  fit.tearDown(output_file);
  return 0;
}

// vim: sw=2 tw=78
