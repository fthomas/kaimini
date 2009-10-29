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

#include <cstdlib>
#include <getopt.h>
#include "fcn.h"
#include "slha.h"
#include "spheno.h"

using namespace std;
using namespace ROOT::Minuit2;
using namespace FISP;
using namespace SPheno;

void parse_command_line(int argc, char** argv, string* inputFileName,
                        string* outputFileName)
{
  int c = 0;
  int option_index = 0;
  const struct option long_options[] =
  {
    {"input",  required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {0, 0, 0, 0}
  };

  while (true)
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

  // Set our (user-supplied) i/o filenames as SPheno's i/o filenames.
  if (input_file.size() < sizeof(InputOutput_LesHouches_InputFile))
    strcpy(InputOutput_LesHouches_InputFile, input_file.c_str());
  else
  {
    cerr << "Warning: input filename too long. SPheno will use its "
         << "default input filename." << endl;
  }

  if (output_file.size() < sizeof(InputOutput_LesHouches_OutputFile))
    strcpy(InputOutput_LesHouches_OutputFile, output_file.c_str());
  else
  {
    cerr << "Warning: output filename too long. SPheno will use its "
         << "default output filename." << endl;
  }

  Slha slha_input;
  slha_input.readFile(input_file);

  Fcn fcn;
  fcn.setParameters(slha_input);
  fcn.setObservables(slha_input);

  SPhenoDouble_RunTill_Model_bilinear_Rparity();
  fcn.simpleFitMinuit();
  SPhenoDouble_RunTill_End();

  return 0;
}

// vim: sw=2 tw=78
