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

#ifndef KAIMINI_KAIMINI_H
#define KAIMINI_KAIMINI_H

#include <stdexcept>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/random.hpp>

namespace Kaimini {

const std::string kaimini_version = "0.0.3-46-g2fa6c2c";

// Random number generator that is used in Kaimini.
typedef boost::mt19937 random_generator_type;
extern random_generator_type random_generator;

// Auxiliary functions/classes for error handling:
void exit_field_not_found(const std::string& key);

void exit_file_open_failed(const std::string& filename);

void exit_line_not_parsed(const std::string& block, const std::string& line);

void exit_value_not_parsed(const std::string& key, const std::string& value);

void warn_line_ignored(const std::string& block, const std::string& line);

class ExBlockNotFound : public std::runtime_error
{
public:
  explicit ExBlockNotFound(const std::string& msg)
    : std::runtime_error(msg) {}
};

// Miscellaneous auxiliary functions:
void parse_command_line(int argc, char** argv,
                        std::string* ifile, std::string* ofile);

std::string random_string(size_t length = 6);

boost::filesystem::path
temp_path(const boost::filesystem::path& p_template);

boost::filesystem::path
create_temp_directory(const boost::filesystem::path& dp_template);

} // namespace Kaimini

#endif // KAIMINI_KAIMINI_H

// vim: sw=2 tw=78
