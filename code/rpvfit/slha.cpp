// RPVFit
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

const int SlhaBlock::no_index = std::numeric_limits<int>::max();


SlhaBlock& Slha::operator()(const string& blockName)
{
  string block_name = to_upper_copy(blockName);

  for (Slha::iterator it = begin(); it != end(); it++)
    if (block_name == to_upper_copy(it->name)) return *it;

  push_back(SlhaBlock(block_name));
  return back();
}


Slha& Slha::read(istream& is)
{
  string line;
  string curr_block_name;

  while (getline(is, line))
  {
    if (trim_left_copy(line).empty()) continue;

    SlhaLine slha_line(line);
    unsigned char first_char = slha_line[1][0];

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
  else return read(fs);
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
    for (; it != end(); it++)
      if (si == it->at(1)) return *it;
  }
  else if ("" == sk)
  {
    for (; it != end(); it++)
      if (si == it->at(1) && sj == it->at(2)) return *it;
  }
  else if ("" == sl)
  {
    for (; it != end(); it++)
      if (si == it->at(1) && sj == it->at(2) &&
          sk == it->at(3)) return *it;
  }
  else
  {
    for (; it != end(); it++)
      if (si == it->at(1) && sj == it->at(2) &&
          sk == it->at(3) && sl == it->at(4)) return *it;
  }

  throw out_of_range(
    "SlhaBlock::operator()("+ si +", "+ sj +", "+ sk +", "+ sl +")");
}


SlhaLine& SlhaBlock::operator()(const int i, const int j, const int k,
                                const int l)
{
  string si = lexical_cast<string>(i);
  string sj = lexical_cast<string>(j);
  string sk = lexical_cast<string>(k);
  string sl = lexical_cast<string>(l);

  if (no_index == i)
  {
    push_back(SlhaLine());
    return back();
  }
  else if (no_index == j) return (*this)(si);
  else if (no_index == k) return (*this)(si, sj);
  else if (no_index == l) return (*this)(si, sj, sk);
  else                    return (*this)(si, sj, sk, sl);
}


SlhaLine& SlhaLine::fromString(const string& line)
{
  size_t comment_begin = line.find("#");
  if (string::npos == comment_begin) comment_begin = line.length();

  string data    = trim_copy(line.substr(0, comment_begin));
  string comment = trim_copy(line.substr(comment_begin));

  clear();
  if (!data.empty()) split(*dynamic_cast<vector<string>*>(this), data,
                           is_space(), token_compress_on);
  insert(begin(), line);
  push_back(comment);

  return *this;
}


ostream& operator<<(ostream& os, const Slha& slha)
{
  for (Slha::const_iterator it = slha.begin(); it != slha.end(); it++)
    os << *it;
  return os;
}

ostream& operator<<(ostream& os, const SlhaBlock& block)
{
  for (SlhaBlock::const_iterator it = block.begin(); it != block.end(); it++)
    os << *it << endl;
  return os;
}

ostream& operator<<(ostream& os, const SlhaLine& line)
{
  return os << line.at(0);
}

// vim: sw=2 tw=78
