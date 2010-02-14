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
#include <ostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "kaimini.h"

using namespace std;
namespace fs = boost::filesystem;
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
  catch(po::invalid_command_line_syntax& ex)
  {
    cerr << "Error: invalid command line syntax: " << ex.what() << endl;
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


string random_string(size_t length)
{
  static const std::string alnum = "0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  typedef boost::uniform_int<> dist_type;
  boost::variate_generator<random_generator_type&, dist_type>
  rnd(random_generator, dist_type(0, alnum.length()-1));

  string retval;
  while (0 != length--) retval += alnum[rnd()];
  return retval;
}


fs::path temp_path(const fs::path& p_template)
{
  string name = p_template.filename();
  size_t len = 0;

  string::const_reverse_iterator it = name.rbegin();
  while (it != name.rend() && 'X' == *it++) ++len;

  size_t pos = name.length() - len;
  if (len < 6)
  {
    name.resize(pos + 6, 'X');
    len = 6;
  }
  name.replace(pos, len, random_string(len));

  fs::path temp_p = p_template.parent_path() / name;
  return temp_p;
}


fs::path create_temp_directory(const fs::path& dp_template)
{
  fs::path temp_dir;

  do temp_dir = temp_path(dp_template);
  while (fs::exists(temp_dir));

  fs::create_directory(temp_dir);
  return temp_dir;
}

} // namespace Kaimini

// vim: sw=2 tw=78
