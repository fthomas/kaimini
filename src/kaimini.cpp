// Kaimini, a general purpose fitting and analysis front end
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

#include <cstddef>
#include <cstdlib>
#include <ostream>
#include <stdexcept>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include "kaimini.h"
#include "random.h"

using namespace std;
using namespace boost;
namespace fs = boost::filesystem;
namespace po = boost::program_options;

namespace Kaimini {

bool g_verbose_output = true;


void exit_block_not_found(const string& block)
{
  cerr << format(
    "# Error: Could not find block ‘%1%’ in SLHA structure.\n") % block;
  exit(EXIT_FAILURE);
}


void exit_field_not_found(const string& key)
{
  cerr << format(
    "# Error: Could not find field referenced by ‘%1%’ in SLHA structure.\n")
    % key;
  exit(EXIT_FAILURE);
}


void exit_file_nonexistent(const string& filename)
{
  cerr << format(
    "# Error: File ‘%1%’ does not exist.\n") % filename;
  exit(EXIT_FAILURE);
}


void exit_file_open_failed(const string& filename)
{
  cerr << format(
    "# Error: Failed to open file ‘%1%’.\n") % filename;
  exit(EXIT_FAILURE);
}


void exit_line_not_parsed(const string& block, const string& line)
{
  cerr << format(
    "# Error: Could not parse line in block ‘%1%’: %2%\n") % block % line;
  exit(EXIT_FAILURE);
}


void exit_value_not_parsed(const string& key, const string& value)
{
  cerr << format(
    "# Error: Could not parse field referenced by ‘%1%’: %2%\n") % key
    % value;
  exit(EXIT_FAILURE);
}


void info_ignore_absent_field(const string& key)
{
  if (!g_verbose_output) return;

  cout << format(
    "# Info: Ignoring absent field referenced by ‘%1%’.\n") % key;
}


void info_include_absent_field(const string& key)
{
  if (!g_verbose_output) return;

  cout << format(
    "# Info: Treating absent field referenced by ‘%1%’ as zero.\n") % key;
}


void info_ignore_nan(const string& key)
{
  if (!g_verbose_output) return;

  cout << format(
    "# Info: Ignoring NaN in field referenced by ‘%1%’.\n") % key;
}


void info_include_nan(const string& key)
{
  if (!g_verbose_output) return;

  cout << format(
    "# Info: Treating NaN in field referenced by ‘%1%’ as zero.\n") % key;
}


void warn_line_ignored(const string& block, const string& line)
{
  cerr << format(
    "# Warning: Ignoring line in block ‘%1%’: %2%\n") % block % line;
}


void warn_line_not_parsed(const string& block, const string& line)
{
  cerr << format(
    "# Warning: Could not parse line in block ‘%1%’: %2%\n") % block % line;
}


void warn_unrecognized_switch(const string& line, const string& sw)
{
  cerr << format(
    "# Warning: Unrecognized switch ‘%1%’ (should be either ‘on’ or ‘off’) "
    "in line: %2%\n") % sw % line;
}


void parse_command_line(int argc, char** argv,
                        string* ifile, string* ofile)
{
  po::options_description cmdline_options("Options");
  cmdline_options.add_options()
    ("help,h",    "show this help message and exit")
    ("version,V", "show version number and exit")
    ("input-file,i",  po::value<string>(ifile)->
      default_value(*ifile), "read input from file <arg>")
    ("output-file,o", po::value<string>(ofile)->
      default_value(*ofile), "write result to file <arg>")
    ("quiet,q", "be quiet (no output to stdout)");

  po::positional_options_description pos_options;
  pos_options.add("input-file", 1);
  pos_options.add("output-file", 1);

  po::variables_map vm;

  try
  {
    po::parsed_options parsed = po::command_line_parser(argc, argv)
      .options(cmdline_options).positional(pos_options)
      .allow_unregistered().run();
    po::store(parsed, vm);
    po::notify(vm);
  }
  catch (po::invalid_command_line_syntax& ex)
  {
    cerr << format(
      "# Error: Invalid command line syntax: %1%\n") % ex.what();
    exit(EXIT_FAILURE);
  }
  catch (po::multiple_occurrences&)
  {
    cerr << format(
      "# Error: Several occurrences of an option that can be specified only "
      "once.\n");
    exit(EXIT_FAILURE);
  }
  catch (po::too_many_positional_options_error&)
  {
    cerr << format(
      "# Error: Too many command line arguments.\n");
    exit(EXIT_FAILURE);
  }

  if (vm.count("help"))
  {
    cout << "Usage: kaimini [options] [input-file] [output-file]\n\n";
    cout << cmdline_options;
    exit(EXIT_SUCCESS);
  }

  if (vm.count("version"))
  {
    cout << "kaimini " << g_kaimini_version << endl;
    exit(EXIT_SUCCESS);
  }

  if (vm.count("quiet")) g_verbose_output = false;
}


double parse_error_string(const double value, string errorStr)
{
  if (errorStr.empty())
  {
    throw invalid_argument("parse_error_string(): errorStr is empty");
  }

  bool normal  = false;
  bool uniform = false;

  if (errorStr.compare(0, 7, "normal:") == 0)
  {
    errorStr = errorStr.substr(7);
    normal = true;
  }
  else if (errorStr.compare(0, 8, "uniform:") == 0)
  {
    errorStr = errorStr.substr(8);
    uniform = true;
  }

  bool percentage = false;
  string::iterator last_char = errorStr.end()-1;

  if ('%' == *last_char)
  {
    errorStr.erase(last_char);
    percentage = true;
  }

  double error = lexical_cast<double>(errorStr);

  if (percentage) error *= 0.01 * std::abs(value);

  if (normal)
  {
    error = std::abs(g_rnd.randNormal(error));
  }
  else if (uniform)
  {
    error = g_rnd.randUniformReal(error);
  }

  return error;
}


fs::path temp_path(const fs::path& pathTemplate)
{
  string name = pathTemplate.leaf();
  size_t len = 0;

  string::reverse_iterator it = name.rbegin();
  while (it != name.rend() && 'X' == *it++) ++len;

  size_t pos = name.length() - len;
  if (len < 6)
  {
    name.resize(pos + 6, 'X');
    len = 6;
  }
  name.replace(pos, len, g_rnd.randString(len));

  fs::path temp_p = pathTemplate.branch_path() / name;
  return temp_p;
}


fs::path create_temp_directory(const fs::path& dirTemplate)
{
  fs::path temp_dir;

  do temp_dir = temp_path(dirTemplate);
  while (fs::exists(temp_dir));

  fs::create_directory(temp_dir);
  return temp_dir;
}

} // namespace Kaimini

// vim: sw=2 tw=78
