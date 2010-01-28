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

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <getopt.h>
#include "kaimini.h"

using namespace std;

namespace Kaimini {

random_generator_type
random_generator(static_cast<unsigned int>(time(0)) +
                 static_cast<unsigned int>(clock()));


void exit_field_not_found(const string& key)
{
  cerr << "error: could not find field referenced by ‘" << key
       << "’ in SLHA structure" << endl;
  exit(EXIT_FAILURE);
}


void exit_file_open_failed(const string& filename)
{
  cerr << "error: failed to open file ‘" << filename << "’" << endl;
  exit(EXIT_FAILURE);
}


void exit_line_not_parsed(const string& block, const string& line)
{
  cerr << "error: could not parse line in block ‘" << block << "’: "
       << line << endl;
  exit(EXIT_FAILURE);
}


void exit_value_not_parsed(const string& key, const string& value)
{
  cerr << "error: could not parse field referenced by ‘" << key << "’: "
       << value << endl;
  exit(EXIT_FAILURE);
}


void throw_block_not_found(const string& function, const string& block)
{
  stringstream msg;
  msg << function << ": block ‘" << block << "’ could not be found "
      << "in SLHA structure";
  throw runtime_error(msg.str());
}


void warn_line_ignored(const string& block, const string& line)
{
  cerr << "warning: ignoring line in block ‘" << block << "’: "
       << line << endl;
}


void parse_command_line(int argc, char** argv,
                        string& inputFilename,
                        string& outputFilename)
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
        inputFilename.assign(optarg);
        break;

      case 'o':
        outputFilename.assign(optarg);
        break;

      default:
        abort();
    }
  }
}

} // namespace Kaimini

// vim: sw=2 tw=78
