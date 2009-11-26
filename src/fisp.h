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

#ifndef FISP_FISP_H
#define FISP_FISP_H

#include <string>

namespace FISP {

const std::string fisp_version = "0.0.1-26-ge5c620d";

void parse_command_line(int argc, char** argv,
                        std::string* inputFilename,
                        std::string* outputFilename);

void set_filenames(const std::string& inputFilename,
                   const std::string& outputFilename);

} // namespace FISP

#endif // FISP_FISP_H

// vim: sw=2 tw=78
