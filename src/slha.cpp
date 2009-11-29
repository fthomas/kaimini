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

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <limits>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include "slha.h"

using namespace std;
using namespace boost;

namespace FISP {

/* static */
const int SlhaBlock::nind = std::numeric_limits<int>::max();


SlhaLine& SlhaLine::str(const string& line)
{
  impl_.clear();

  string line_ = trim_copy(line.substr(0, line.find("\n")));
  if (line_.empty())
  {
    impl_.push_back("");
    mLineFormat = "%|0t|%1%";
    return *this;
  }

  size_t comment_begin = line_.find("#");
  if (string::npos == comment_begin) comment_begin = line_.length();

  const string data    = trim_copy(line_.substr(0, comment_begin));
  const string comment = trim_copy(line_.substr(comment_begin));

  if (!data.empty()) split(impl_, data, is_space(), token_compress_on);
  if (!comment.empty()) impl_.push_back(comment);

  stringstream line_format("");
  for (size_t i = 0, pos = 0; i < impl_.size(); ++i, ++pos)
  {
    pos = line.find(impl_[i], pos);
    line_format << "%|" << pos << "t|%" << (i+1) << "% ";
  }
  mLineFormat = trim_copy(line_format.str());

  return *this;
}


string SlhaLine::str() const
{
  format fmter(mLineFormat);
  for (const_iterator it = begin(); it != end(); ++it) fmter % *it;
  return fmter.str();
}


string SlhaLine::strPlain() const
{
  string ret = "";
  for (const_iterator it = begin(); it != end(); ++it) ret += *it + " ";
  return trim_copy(ret);
}


SlhaLine& SlhaBlock::operator[](const vector<string>& keys)
{
  iterator it = find(keys);
  if (end() == it)
  {
    push_back(SlhaLine());
    return back();
  }
  return *it;
}


const SlhaLine& SlhaBlock::operator[](const vector<string>& keys) const
{
  const_iterator it = find(keys);
  if (end() == it)
  {
    string kstr = "";
    for (vector<string>::const_iterator it = keys.begin();
         it != keys.end(); ++it) kstr += *it + " ";

    throw out_of_range("SlhaBlock::{operator[],at}(" + trim_copy(kstr) +
                       "); Block " + name());
  }
  return *it;
}


SlhaLine& SlhaBlock::operator[](const vector<int>& intKeys)
{
  vector<string> keys;
  for (vector<int>::const_iterator it = intKeys.begin();
       it != intKeys.end(); ++it) keys.push_back(to_string(*it));
  return (*this)[keys];
}


const SlhaLine& SlhaBlock::operator[](const vector<int>& intKeys) const
{
  vector<string> keys;
  for (vector<int>::const_iterator it = intKeys.begin();
       it != intKeys.end(); ++it) keys.push_back(to_string(*it));
  return (*this)[keys];
}


SlhaLine& SlhaBlock::operator[](const string& keysStr)
{
  vector<string> keys;
  split(keys, keysStr, is_space(), token_compress_on);
  return (*this)[keys];
}


const SlhaLine& SlhaBlock::operator[](const string& keysStr) const
{
  vector<string> keys;
  split(keys, keysStr, is_space(), token_compress_on);
  return (*this)[keys];
}


SlhaLine& SlhaBlock::at(const std::string& s0, const std::string& s1,
                        const std::string& s2, const std::string& s3)
{
  vector<string> keys;

  if (s0.empty()) return (*this)[keys];
  keys.push_back(s0);

  if (s1.empty()) return (*this)[keys];
  keys.push_back(s1);

  if (s2.empty()) return (*this)[keys];
  keys.push_back(s2);

  if (s3.empty()) return (*this)[keys];
  keys.push_back(s3);

  return (*this)[keys];
}


const
SlhaLine& SlhaBlock::at(const std::string& s0, const std::string& s1,
                        const std::string& s2, const std::string& s3) const
{
  vector<string> keys;

  if (s0.empty()) return (*this)[keys];
  keys.push_back(s0);

  if (s1.empty()) return (*this)[keys];
  keys.push_back(s1);

  if (s2.empty()) return (*this)[keys];
  keys.push_back(s2);

  if (s3.empty()) return (*this)[keys];
  keys.push_back(s3);

  return (*this)[keys];
}


SlhaLine& SlhaBlock::at(const int i0, const int i1,
                        const int i2, const int i3)
{
  vector<string> keys;

  if (nind == i0) return (*this)[keys];
  keys.push_back(to_string(i0));

  if (nind == i1) return (*this)[keys];
  keys.push_back(to_string(i1));

  if (nind == i2) return (*this)[keys];
  keys.push_back(to_string(i2));

  if (nind == i3) return (*this)[keys];
  keys.push_back(to_string(i3));

  return (*this)[keys];
}


const SlhaLine& SlhaBlock::at(const int i0, const int i1,
                              const int i2, const int i3) const
{
  vector<string> keys;

  if (nind == i0) return (*this)[keys];
  keys.push_back(to_string(i0));

  if (nind == i1) return (*this)[keys];
  keys.push_back(to_string(i1));

  if (nind == i2) return (*this)[keys];
  keys.push_back(to_string(i2));

  if (nind == i3) return (*this)[keys];
  keys.push_back(to_string(i3));

  return (*this)[keys];
}


SlhaBlock::iterator SlhaBlock::find(const vector<string>& keys)
{
  if (keys.empty()) return end();
  for (iterator it = begin(); it != end(); ++it)
  {
    if (keys.size() > it->size()) continue;
    if (equal(keys.begin(), keys.end(), it->begin())) return it;
  }
  return end();
}


SlhaBlock::const_iterator SlhaBlock::find(const vector<string>& keys) const
{
  if (keys.empty()) return end();
  for (const_iterator it = begin(); it != end(); ++it)
  {
    if (keys.size() > it->size()) continue;
    if (equal(keys.begin(), keys.end(), it->begin())) return it;
  }
  return end();
}


SlhaBlock& SlhaBlock::str(const std::string& block)
{
  vector<string> lines;
  split(lines, block, is_any_of("\n"));

  clear();
  for(vector<string>::const_iterator it = lines.begin();
      it != lines.end(); ++it) push_back(*it);

  return *this;
}


SlhaBlock& Slha::operator[](const string& blockName)
{
  const string block_name = to_upper_copy(blockName);

  for (iterator it = begin(); it != end(); ++it)
  {
    if (block_name == to_upper_copy(it->name())) return *it;
  }
  push_back(SlhaBlock(blockName));
  return back();
}


const SlhaBlock& Slha::operator[](const string& blockName) const
{
  const string block_name = to_upper_copy(blockName);

  for (const_iterator it = begin(); it != end(); ++it)
  {
    if (block_name == to_upper_copy(it->name())) return *it;
  }
  throw out_of_range("Slha::operator[](\""+ blockName +"\")");
}


Slha& Slha::read(istream& is)
{
  string line = "";
  string curr_block_name = "";

  while (getline(is, line))
  {
    if (trim_left_copy(line).empty()) continue;

    const SlhaLine slha_line(line);

    if ("BLOCK" == to_upper_copy(slha_line[0]) && slha_line.size() > 1)
    {
      if ('#' != slha_line[1][0]) curr_block_name = slha_line[1];
    }

    (*this)[curr_block_name][""] = slha_line;
  }

  return *this;
}


Slha& Slha::readFile(const string& filename)
{
  ifstream fs(filename.c_str());
  if (!fs)
  {
    cerr << "Error (SLHA): readFile(\"" << filename << "\") failed" << endl;
    return *this;
  }
  return read(fs);
}


Slha& Slha::writeFile(const string& filename)
{
  ofstream fs(filename.c_str());
  if (!fs)
  {
    cerr << "Error (SLHA): writeFile(\"" << filename << "\") failed" << endl;
    return *this;
  }
  fs << *this;
  return *this;
}


ostream& operator<<(ostream& os, const Slha& slha)
{
  for (Slha::const_iterator it = slha.begin(); it != slha.end(); ++it)
  {
    os << *it;
  }
  return os;
}

ostream& operator<<(ostream& os, const SlhaBlock& block)
{
  for (SlhaBlock::const_iterator it = block.begin(); it != block.end(); ++it)
  {
    os << *it << endl;
  }
  return os;
}

ostream& operator<<(ostream& os, const SlhaLine& line)
{
  return os << line.str();
}


bool to_bool(const string& str)
{ return lexical_cast<bool>(str); }

int to_int(const string& str)
{ return lexical_cast<int>(str); }

double to_double(const string& str)
{ return lexical_cast<double>(str); }

long double to_long_double(const string& str)
{ return lexical_cast<long double>(str); }

string to_string(const bool v)
{ return lexical_cast<string>(v); }

string to_string(const int v)
{ return lexical_cast<string>(v); }

string to_string(const double v)
{ return lexical_cast<string>(v); }

string to_string(const long double v)
{ return lexical_cast<string>(v); }

} // namespace FISP

// vim: sw=2 tw=78
