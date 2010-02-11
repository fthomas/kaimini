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
#include <boost/program_options.hpp>
#include "kaimini.h"

using namespace std;
namespace po = boost::program_options;

namespace Kaimini {

random_generator_type
random_generator(static_cast<unsigned int>(time(0)) +
                 static_cast<unsigned int>(clock()));


void exit_field_not_found(const string& key)
{
  cerr << "Error: could not find field referenced by ‘" << key
       << "’ in SLHA structure" << endl;
  exit(EXIT_FAILURE);
}


void exit_file_open_failed(const string& filename)
{
  cerr << "Error: failed to open file ‘" << filename << "’" << endl;
  exit(EXIT_FAILURE);
}


void exit_line_not_parsed(const string& block, const string& line)
{
  cerr << "Error: could not parse line in block ‘" << block << "’: "
       << line << endl;
  exit(EXIT_FAILURE);
}


void exit_value_not_parsed(const string& key, const string& value)
{
  cerr << "Error: could not parse field referenced by ‘" << key << "’: "
       << value << endl;
  exit(EXIT_FAILURE);
}


void throw_block_not_found(const string& where, const string& block)
{
  stringstream msg;
  msg << where << ": block ‘" << block << "’ could not be found "
      << "in SLHA structure";
  throw runtime_error(msg.str());
}


void throw_ptr_is_null(const string& where, const string& ptr)
{
  stringstream msg;
  msg << where << ": pointer ‘" << ptr << "’ is a null pointer";
  throw runtime_error(msg.str());
}


void warn_line_ignored(const string& block, const string& line)
{
  cerr << "Warning: ignoring line in block ‘" << block << "’: "
       << line << endl;
}


void parse_command_line(int argc, char** argv,
                        string* ifile, string* ofile)
{
  po::options_description cmdline_options("Options");
  cmdline_options.add_options()
    ("help,h",    "show this help message and exit")
    ("version,V", "show Kaimini's version number and exit")

    ("input-file,i",  po::value<string>(ifile)->
      default_value(*ifile), "read input from file <arg>")

    ("output-file,o", po::value<string>(ofile)->
      default_value(*ofile), "write result to file <arg>");

  po::variables_map vm;

  try
  {
    po::parsed_options parsed = po::command_line_parser(argc, argv)
      .options(cmdline_options).allow_unregistered().run();
    po::store(parsed, vm);
    po::notify(vm);
  }
  catch(po::invalid_command_line_syntax& e)
  {
    cerr << "Invalid command line syntax: " << e.what() << endl;
    exit(EXIT_FAILURE);
  }

  if (vm.count("help"))
  {
    cout << "Usage: kaimini [options]" << endl << endl;
    cout << cmdline_options;
    exit(EXIT_SUCCESS);
  }

  if (vm.count("version"))
  {
    cout << "kaimini " << kaimini_version << endl;
    exit(EXIT_SUCCESS);
  }
}

} // namespace Kaimini

// vim: sw=2 tw=78
