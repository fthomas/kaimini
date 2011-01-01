// Kaimini, a general purpose fitting and analysis front end
// Copyright © 2010-2011 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#include <clocale>
#include <cstdlib>
#include <libintl.h>
#include "kernel/Controller.h"
#include "utility/exit_exception.h"

int main(int argc, char* argv[])
{
  std::setlocale(LC_MESSAGES, "");
  bindtextdomain("kaimini", ".");
  textdomain("kaimini");

  try
  {
    Kaimini::Controller controller;
    controller.initializeKaimini(argc, argv);
    controller.terminateKaimini();
  }
  catch (Kaimini::utility::exit_exception& ex)
  {
    std::exit(ex.status);
  }
}
