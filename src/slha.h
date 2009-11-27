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

#include <cstddef>
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
  Slha& writeFile(const std::string& filename);
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

/*
class SlhaBlock
{
public:
  SlhaBlock() : mName("")
  {}

  SlhaBlock(const std::string& name) : mName(name)
  {}

  SlhaLine& operator[](const std::string& indexStr = "");

  SlhaLine& at(const std::string& s0 = "", const std::string& s1 = "",
               const std::string& s2 = "", const std::string& s3 = "",
               const std::string& s4 = "", const std::string& s5 = "");

  SlhaLine& at(int i0 = msNoIndex, int i1 = msNoIndex, int i2 = msNoIndex,
               int i3 = msNoIndex, int i4 = msNoIndex, int i5 = msNoIndex);

  clear

  SlhaBlock& name(const std::string& newName)
  { mName = newName; return *this; }

  const std::string& name() const
  { return mName; }

  std::string str() const
  { std::stringstream ss(""); ss << *this; return ss.str(); }

private:
  std::string mName;
  std::vector<SlhaLine> mVecLine;

  static const int msNoIndex;
};
*/


class SlhaLine
{
public:
  SlhaLine()
  { str(""); }

  SlhaLine(const std::string& line)
  { str(line); }

  SlhaLine& operator=(const std::string& line)
  { return str(line); }

  SlhaLine& operator+=(const std::string& rhs)
  { return append(rhs); }

  std::string& operator[](std::size_t n)
  { return mVecStr[n]; }

  const std::string& operator[](std::size_t n) const
  { return mVecStr[n]; }

  std::string& at(std::size_t n)
  { return mVecStr.at(n); }

  const std::string& at(std::size_t n) const
  { return mVecStr.at(n); }

  SlhaLine& append(const std::string& rhs)
  { return str(str() + rhs); }

  bool empty() const
  { return size() == 1 && mVecStr[0] == ""; }

  std::size_t size() const
  { return mVecStr.size(); }

  SlhaLine& str(const std::string& line);

  std::string str() const;

  std::string strPlain() const;

private:
  std::vector<std::string> mVecStr;
  std::string mLineFormat;
};


bool to_bool(const std::string& str);
int to_int(const std::string& str);
double to_double(const std::string& str);
long double to_long_double(const std::string& str);

std::string to_string(bool v);
std::string to_string(int v);
std::string to_string(double v);
std::string to_string(long double v);

} // namespace FISP

#endif // FISP_SLHA_H

// vim: sw=2 tw=78
