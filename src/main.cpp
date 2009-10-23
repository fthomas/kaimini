// RPVFit
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

#include <cstdlib>
#include <getopt.h>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MnMinimize.h>
#include <Minuit2/MnPrint.h>
#include "fcn.h"
#include "slha.h"
#include "spheno.h"

using namespace std;
using namespace ROOT::Minuit2;

void parse_command_line(int argc, char** argv, string* inputFileName,
                        string* outputFileName)
{
  int c;
  int option_index = 0;
  struct option long_options[] =
    {
      {"input",  required_argument, 0, 'i'},
      {"output", required_argument, 0, 'o'},
      {0, 0, 0, 0}
    };

  while (1)
  {
    c = getopt_long(argc, argv, "i:o:", long_options, &option_index);
    if (-1 == c) break;

    switch (c)
    {
      case 'i':
        inputFileName->assign(optarg);
        break;

      case 'o':
        outputFileName->assign(optarg);
        break;

      default:
        abort();
    }
  }
}


int main(int argc, char* argv[])
{
  // Use SPheno's defaul input/output filenames as our own default filenames.
  string input_file  = "LesHouches.in";
  string output_file = "SPheno.spc";
  parse_command_line(argc, argv, &input_file, &output_file);

  if (input_file.size() < sizeof(inputoutput_mp_leshouches_file_))
    strcpy(inputoutput_mp_leshouches_file_, input_file.c_str());
  else cerr << "Warning: input filename too long. SPheno will use its "
            << "default input filename." << endl;

  Slha slha_input;
  slha_input.readFile(input_file);

  Fcn fcn;
  fcn.setUserParameters(slha_input);
  fcn.setFixedParameters(slha_input);

  sphenodouble_mp_runtill_model_bilinear_rparity_();

  MnMinimize minimizer(fcn, fcn.upar);
  FunctionMinimum m = minimizer();
  fcn(m.UserParameters().Params());

  sphenodouble_mp_runtill_end_();

  cout << m << endl;

  return 0;
}

// vim: sw=2 tw=78
