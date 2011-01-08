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

#ifndef KAIMINI_KERNEL_CONTROLLER_H
#define KAIMINI_KERNEL_CONTROLLER_H

#include <cstdlib>
#include <string>
#include <boost/program_options.hpp>
#include "kernel/Logger.h"
#include "kernel/RandomGenerator.h"

namespace Kaimini {

class Controller
{
public:
  Controller();

  void
  initializeKaimini();

  void
  initializeKaimini(int argc, char* argv[]);

  void
  terminateKaimini(int status = EXIT_SUCCESS);

  Logger&
  logger()
  { return logger_; }

  RandomGenerator&
  randomGenerator()
  { return rand_gen_; }

private:
  void
  initializeOptions();

  boost::program_options::variables_map
  parseOptions(int argc, char* argv[]);

  void
  processOptions(const boost::program_options::variables_map& var_map);

  std::string
  createHelpMessage() const;

  std::string
  createVersionMessage() const;

private:
  Logger logger_;
  RandomGenerator rand_gen_;

  boost::program_options::options_description cmdline_options_;
};

} // namespace Kaimini

#endif // KAIMINI_KERNEL_CONTROLLER_H
