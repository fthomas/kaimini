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

#ifndef KAIMINI_KERNEL_CONTROLLER_H
#define KAIMINI_KERNEL_CONTROLLER_H

#include <boost/program_options.hpp>
#include "kernel/RandomGenerator.h"

namespace Kaimini {

class Controller
{
public:
  Controller();

  void
  initializeKaimini(int argc, char* argv[]);

private:
  void
  initializeOptions();

  boost::program_options::variables_map
  parseOptions(int argc, char* argv[]) const;

  void
  processOptions(const boost::program_options::variables_map& var_map);

private:
  RandomGenerator rg_;
  boost::program_options::options_description cmdline_options_;
};

} // namespace Kaimini

#endif // KAIMINI_KERNEL_CONTROLLER_H
