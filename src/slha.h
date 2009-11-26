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

#ifndef FISP_SLHA_H
#define FISP_SLHA_H

#include <iostream>
#include <limits>
#include <list>
#include <sstream>
#include <string>
#include <vector>

namespace FISP {

class Slha;
class SlhaBlock;
class SlhaLine;

std::ostream& operator<<(std::ostream& os, const Slha& slha);
std::ostream& operator<<(std::ostream& os, const SlhaBlock& block);
std::ostream& operator<<(std::ostream& os, const SlhaLine& line);


class Slha : public std::list<SlhaBlock>
{
public:
  Slha() {}
  Slha(const std::string filename)
  { readFile(filename); }

  SlhaBlock& operator()(const std::string& blockName);
  SlhaBlock operator()(const std::string& blockName) const;
  Slha& read(std::istream& is);
  Slha& readFile(const std::string& filename);
  Slha& fromString(const std::string& str)
  { std::stringstream ss(""); ss << str; return read(ss); }
  std::string toString() const
  { std::stringstream ss(""); ss << *this; return ss.str(); }
};


class SlhaBlock : public std::list<SlhaLine>
{
public:
  std::string name;

  SlhaBlock() {}
  SlhaBlock(const std::string& blockName) : name(blockName) {}

  SlhaLine& operator()(const std::string& si = "",
                       const std::string& sj = "",
                       const std::string& sk = "",
                       const std::string& sl = "");
  SlhaLine& operator()(const int i,             const int j = msNoIndex,
                       const int k = msNoIndex, const int l = msNoIndex);
  std::string toString() const
  { std::stringstream ss(""); ss << *this; return ss.str(); }

private:
  static const int msNoIndex;
};


class SlhaLine
{
public:
  SlhaLine()
  { str(""); }

  SlhaLine(const std::string& line)
  { str(line); }

  SlhaLine& operator=(const std::string& line)
  { return str(line); }

  std::string& operator[](size_t n)
  { return mVecStr[n]; }

  const std::string& operator[](size_t n) const
  { return mVecStr[n]; }

  std::string& at(size_t n)
  { return mVecStr.at(n); }

  const std::string& at(size_t n) const
  { return mVecStr.at(n); }

  SlhaLine& str(const std::string& line);
  std::string str() const;

private:
  std::vector<std::string> mVecStr;
  std::string mLineFormat;
};


bool to_bool(const std::string& str);
int to_int(const std::string& str);
double to_double(const std::string& str);
long double to_long_double(const std::string& str);

} // namespace FISP

#endif // FISP_SLHA_H

// vim: sw=2 tw=78
