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

#include <string>
#include <boost/program_options.hpp>
#include "Controller.h"

namespace po = boost::program_options;

namespace Kaimini {

Controller::Controller() : rg_(), cmdline_options_("Options")
{
  initializeCmdlineOptions();
}


void
Controller::initializeCmdlineOptions()
{
  cmdline_options_.add_options()
    ("help,h",    "Show this help message and exit.")
    ("version,V", "Show version number and exit.")

    ("input-file,i",  po::value<std::string>()->default_value("kaimini.in"),
      "Read input from file <arg>.")
    ("output-file,o", po::value<std::string>()->default_value("kaimini.out"),
      "Write results to file <arg>.")

    ("seed,s", po::value<unsigned int>(),
      "Use <arg> as seed for the random number generator.");
}

} // namespace Kaimini
