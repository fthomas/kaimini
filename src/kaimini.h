// Kaimini, a general purpose fitting frontend
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

#include <functional>
#include <limits>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace Kaimini {

const std::string g_kaimini_version = "0.1.1";

extern bool g_verbose_output;

// erf(1/sqrt(2)):
const long double g_normal_1sigma = 0.68268949213708589717046509126407L;
// erf(2/sqrt(2)):
const long double g_normal_2sigma = 0.95449973610364158559943472566693L;
// erf(3/sqrt(2)):
const long double g_normal_3sigma = 0.99730020393673981094669637046481L;

// Auxiliary functions/classes for error handling:
void exit_block_not_found(const std::string& block);

void exit_field_not_found(const std::string& key);

void exit_file_nonexistent(const std::string& filename);

void exit_file_open_failed(const std::string& filename);

void exit_line_not_parsed(const std::string& block, const std::string& line);

void exit_value_not_parsed(const std::string& key, const std::string& value);

void info_ignore_absent_field(const std::string& key);

void info_include_absent_field(const std::string& key);

void info_ignore_nan(const std::string& key);

void info_include_nan(const std::string& key);

void warn_line_ignored(const std::string& block, const std::string& line);

void warn_line_not_parsed(const std::string& block, const std::string& line);

void warn_unrecognized_switch(const std::string& line, const std::string& sw);

// Miscellaneous auxiliary functions:
void parse_command_line(int argc, char** argv,
                        std::string* ifile, std::string* ofile);

double parse_error_string(double value, std::string errorStr);

boost::filesystem::path
temp_path(const boost::filesystem::path& pathTemplate);

boost::filesystem::path
create_temp_directory(const boost::filesystem::path& dirTemplate);


template<typename RealType> inline bool
close_to_zero(RealType x)
{
  const RealType eps = 100. * std::numeric_limits<RealType>::epsilon();
  return -eps <= x && x <= eps;
}


/// \cond SIMPLE_FUNCTION_OBJECTS
template<typename Pair> struct pair_select1st
  : public std::unary_function<Pair, typename Pair::first_type>
{
  typename Pair::first_type&
  operator()(Pair& x) const
  { return x.first; }

  const typename Pair::first_type&
  operator()(const Pair& x) const
  { return x.first; }
};


template<typename Pair> struct pair_select2nd
  : public std::unary_function<Pair, typename Pair::second_type>
{
  typename Pair::second_type&
  operator()(Pair& x) const
  { return x.second; }

  const typename Pair::second_type&
  operator()(const Pair& x) const
  { return x.second; }
};


struct iequal_to
  : public std::binary_function<std::string, std::string, bool>
{
  bool operator()(const std::string& a, const std::string& b) const
  { return boost::iequals(a, b); }
};


struct iless_than
  : public std::binary_function<std::string, std::string, bool>
{
  bool operator()(const std::string& a, const std::string& b) const
  { return boost::to_lower_copy(a) < boost::to_lower_copy(b); }
};
/// \endcond

} // namespace Kaimini

#endif // KAIMINI_KAIMINI_H

// vim: sw=2 tw=78
