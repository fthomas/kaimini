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
class SlhaBlock2
{
public:
  SlhaBlock(const std::string& name = "") : mName(name)
  {}

  SlhaLine& operator[](const std::string& indexStr = "");
  const SlhaLine& operator[](const std::string& indexStr) const;

  SlhaLine& at(int i0 = nind, int i1 = nind, int i2 = nind,
               int i3 = nind, int i4 = nind, int i5 = nind);

  const SlhaLine& at(int i0,        int i1 = nind, int i2 = nind,
                     int i3 = nind, int i4 = nind, int i5 = nind) const;

  SlhaBlock& clear()
  { mVecLine.clear(); return *this; }

  SlhaBlock& erase(const std::string& indexStr);
  SlhaBlock& erase(int i0,        int i1 = nind, int i2 = nind,
                   int i3 = nind, int i4 = nind, int i5 = nind);

  SlhaBlock& name(const std::string& newName)
  { mName = newName; return *this; }

  const std::string& name() const
  { return mName; }

  std::size_t size() const
  { return mVecLine.size(); }

  std::string str() const
  { std::stringstream ss(""); ss << *this; return ss.str(); }

private:
  getPos()
  getPos()
  getPos()

  std::string mName;
  std::vector<SlhaLine> mVecLine;

  static const int nind;
};
*/

class SlhaLine
{
public:
  typedef std::vector<std::string>::iterator iterator;
  typedef std::vector<std::string>::const_iterator const_iterator;
  typedef std::vector<std::string>::reverse_iterator reverse_iterator;
  typedef std::vector<std::string>::const_reverse_iterator
          const_reverse_iterator;

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

  std::string& back()
  { return mVecStr.back(); }

  const std::string& back() const
  { return mVecStr.back(); }

  iterator begin()
  { return mVecStr.begin(); }

  const_iterator begin() const
  { return mVecStr.begin(); }

  bool empty() const
  { return size() == 1 && mVecStr[0] == ""; }

  iterator end()
  { return mVecStr.end(); }

  const_iterator end() const
  { return mVecStr.end(); }

  std::string& front()
  { return mVecStr.front(); }

  const std::string& front() const
  { return mVecStr.front(); }

  reverse_iterator rbegin()
  { return mVecStr.rbegin(); }

  const_reverse_iterator rbegin() const
  { return mVecStr.rbegin(); }

  reverse_iterator rend()
  { return mVecStr.rend(); }

  const_reverse_iterator rend() const
  { return mVecStr.rend(); }

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
