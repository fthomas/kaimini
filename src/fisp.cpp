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
#include <cstring>
#include <iostream>
#include <string>
#include <getopt.h>
#include "fisp.h"
#include "spheno.h"

using namespace std;

namespace FISP {

void parse_command_line(int argc, char** argv,
                        string* inputFilename,
                        string* outputFilename)
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
        inputFilename->assign(optarg);
        break;

      case 'o':
        outputFilename->assign(optarg);
        break;

      default:
        abort();
    }
  }
}


void set_filenames(const string& inputFilename, const string& outputFilename)
{
  if (inputFilename.size() < sizeof(InputOutput_LesHouches_InputFile))
  {
    strcpy(InputOutput_LesHouches_InputFile, inputFilename.c_str());
  }
  else
  {
    cerr << "Warning: input filename too long. SPheno will use its "
         << "default input filename." << endl;
  }

  if (outputFilename.size() < sizeof(InputOutput_LesHouches_OutputFile))
  {
    strcpy(InputOutput_LesHouches_OutputFile, outputFilename.c_str());
  }
  else
  {
    cerr << "Warning: output filename too long. SPheno will use its "
         << "default output filename." << endl;
  }
}

} // namespace FISP

// vim: sw=2 tw=78
