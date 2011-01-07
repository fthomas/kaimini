// Kaimini, a general purpose fitting and analysis front end
// Copyright © 2009-2011 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#include "Controller.h"
#include <cstdlib>
#include <sstream>
#include <string>
#include <boost/program_options.hpp>
#include "kernel/Kaimini.h"
#include "utility/exit_exception.h"

namespace po = boost::program_options;

namespace Kaimini {

Controller::Controller() : logger_(), rg_(), cmdline_options_()
{
  initializeOptions();
}


void
Controller::initializeKaimini()
{
  const int argc = 1;
  char arg1[] = "kaimini";
  char* argv[] = { arg1 };

  initializeKaimini(argc, argv);
}


void
Controller::initializeKaimini(int argc, char* argv[])
{
  processOptions(parseOptions(argc, argv));
}


void
Controller::terminateKaimini(int status)
{
  throw utility::exit_exception(status);
}


void
Controller::initializeOptions()
{
  po::options_description general_options(_("General options"));
  general_options.add_options()
    ("help,h",    _("Show this help message and exit."))
    ("version,V", _("Show version number and exit."))

    ("input-file,i",  po::value<std::string>()->default_value("kaimini.in"),
      _("Read input from file <arg>."))
    ("output-file,o", po::value<std::string>()->default_value("kaimini.out"),
      _("Write results to file <arg>."))
    ("seed,s", po::value<unsigned int>(),
      _("Use <arg> as seed for the random number generator."));

  po::options_description logging_options(_("Message and logging options"));
  logging_options.add_options()
    ("quiet,q",
      _("Suppress all messages to standard output."))
    ("verbose,v",   po::value<int>()->implicit_value(2),
      _("Show additional informational/debug messages."))
    ("log-file,L",  po::value<std::string>(),
      _("Log messages to file <arg>."))
    ("log-level,l", po::value<int>()->implicit_value(2),
      _("Set verbosity level for file logging."));

  cmdline_options_
    .add(general_options)
    .add(logging_options);
}


po::variables_map
Controller::parseOptions(int argc, char* argv[])
{
  po::positional_options_description pos_options;
  pos_options.add("input-file",  1);
  pos_options.add("output-file", 1);

  po::variables_map var_map;

  try
  {
    po::parsed_options parsed = po::command_line_parser(argc, argv)
      .options(cmdline_options_)
      .positional(pos_options)
      .run();
    po::store(parsed, var_map);
    po::notify(var_map);
  }
  catch (po::invalid_command_line_syntax&)
  {
    logger().logMessage(_(""));
    terminateKaimini(EXIT_FAILURE);
  }
  catch (po::invalid_option_value&)
  {
    logger().logMessage(_(""));
    terminateKaimini(EXIT_FAILURE);
  }
  catch (po::multiple_occurrences&)
  {
    logger().logMessage(_(""));
    terminateKaimini(EXIT_FAILURE);
  }
  catch (po::too_many_positional_options_error&)
  {
    logger().logMessage(_(""));
    terminateKaimini(EXIT_FAILURE);
  }
  return var_map;
}


void
Controller::processOptions(const po::variables_map& var_map)
{
  if (var_map.count("help"))
  {
    logger().logMessage(createHelpMessage());
    terminateKaimini();
  }

  if (var_map.count("version"))
  {
    logger().logMessage(createVersionMessage());
    terminateKaimini();
  }

  if (var_map.count("input-file"))
  {
  }

  if (var_map.count("output-file"))
  {
  }

  if (var_map.count("seed"))
  {
    const unsigned int provided_seed = var_map["seed"].as<unsigned int>();
    rg_.seed(provided_seed);
  }

  if (var_map.count("quiet"))
  {
    logger().verbosityLevel(0);
  }

  if (var_map.count("verbose"))
  {
    const int verbosity_level = var_map["verbose"].as<int>();
    logger().verbosityLevel(verbosity_level);
  }
}


std::string
Controller::createHelpMessage() const
{
  std::ostringstream out;
  out << _("Usage: kaimini [options] [input-file] [output-file]") << "\n";
  out << cmdline_options_;
  return out.str();
}


std::string
Controller::createVersionMessage() const
{
  std::ostringstream out;
  out << "Kaimini " << g_kaimini_version << "\n";
  return out.str();
}

} // namespace Kaimini
