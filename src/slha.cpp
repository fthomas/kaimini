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

#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "slha.h"

using namespace std;
using namespace boost;

namespace FISP {

/* static */
const int SlhaBlock::msNoIndex = std::numeric_limits<int>::max();


SlhaBlock& Slha::operator()(const string& blockName)
{
  const string block_name = to_upper_copy(blockName);

  for (Slha::iterator it = begin(); it != end(); ++it)
  {
    if (block_name == to_upper_copy(it->name)) return *it;
  }
  push_back(SlhaBlock(block_name));
  return back();
}


SlhaBlock Slha::operator()(const string& blockName) const
{
  const string block_name = to_upper_copy(blockName);

  for (Slha::const_iterator it = begin(); it != end(); ++it)
  {
    if (block_name == to_upper_copy(it->name)) return *it;
  }
  throw out_of_range("Slha::operator()(\""+ blockName +"\")");
}


Slha& Slha::read(istream& is)
{
  string line = "";
  string curr_block_name = "";

  while (getline(is, line))
  {
    if (trim_left_copy(line).empty()) continue;

    const SlhaLine slha_line(line);
    const unsigned char first_char = slha_line[1][0];

    if (isdigit(first_char))
    {
      if (curr_block_name.empty())
      {
        cerr << "Warning (SLHA): data line outside a (valid) block: "
             << line << endl;
        continue;
      }

      (*this)(curr_block_name)() = slha_line;
    }
    else if ("BLOCK" == to_upper_copy(slha_line[1]))
    {
      curr_block_name = slha_line.at(2);
      (*this)(curr_block_name)() = slha_line;
    }
    else if ('#' == first_char) continue;
    else cerr << "Warning (SLHA): unparsed line: " << line << endl;
  } // end while getline

  return *this;
}


Slha& Slha::readFile(const string& fileName)
{
  ifstream fs(fileName.c_str());
  if (!fs)
  {
    cerr << "Error (SLHA): readFile(\"" << fileName << "\") failed" << endl;
    return *this;
  }
  return read(fs);
}


SlhaLine& SlhaBlock::operator()(const string& si, const string& sj,
                                const string& sk, const string& sl)
{
  SlhaBlock::iterator it = begin();

  if ("" == si)
  {
    push_back(SlhaLine());
    return back();
  }
  else if ("" == sj)
  {
    for (; it != end(); ++it)
    {
      if (si == it->at(1)) return *it;
    }
  }
  else if ("" == sk)
  {
    for (; it != end(); ++it)
    {
      if (si == it->at(1) && sj == it->at(2)) return *it;
    }
  }
  else if ("" == sl)
  {
    for (; it != end(); ++it)
    {
      if (si == it->at(1) && sj == it->at(2) &&
          sk == it->at(3)) return *it;
    }
  }
  else
  {
    for (; it != end(); ++it)
    {
      if (si == it->at(1) && sj == it->at(2) &&
          sk == it->at(3) && sl == it->at(4)) return *it;
    }
  }

  throw out_of_range("SlhaBlock::operator()(" + si + ", " + sj + ", " + sk +
                     ", " + sl + "); Block " + name);
}


SlhaLine& SlhaBlock::operator()(const int i, const int j, const int k,
                                const int l)
{
  const string si = lexical_cast<string>(i);
  const string sj = lexical_cast<string>(j);
  const string sk = lexical_cast<string>(k);
  const string sl = lexical_cast<string>(l);

  if (msNoIndex == i)
  {
    push_back(SlhaLine());
    return back();
  }
  else if (msNoIndex == j) return (*this)(si);
  else if (msNoIndex == k) return (*this)(si, sj);
  else if (msNoIndex == l) return (*this)(si, sj, sk);
  else                     return (*this)(si, sj, sk, sl);
}


SlhaLine& SlhaLine::fromString(const string& line)
{
  size_t comment_begin = line.find("#");
  if (string::npos == comment_begin) comment_begin = line.length();

  const string data    = trim_copy(line.substr(0, comment_begin));
  const string comment = trim_copy(line.substr(comment_begin));

  clear();
  if (!data.empty())
  {
    split(*dynamic_cast<vector<string>*>(this), data, is_space(),
          token_compress_on);
  }
  insert(begin(), line);
  push_back(comment);

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
  return os << line.at(0);
}

int to_int(const string& str) { return lexical_cast<int>(str); }

double to_double(const string& str) { return lexical_cast<double>(str); }

} // namespace FISP

// vim: sw=2 tw=78
