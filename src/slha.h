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

#ifndef RPVFIT_SLHA_H
#define RPVFIT_SLHA_H

#include <iostream>
#include <limits>
#include <list>
#include <string>
#include <vector>

class SlhaLine;
class SlhaBlock;


class Slha : public std::list<SlhaBlock>
{
public:
  SlhaBlock& operator()(const std::string& blockName);
  SlhaBlock operator()(const std::string& blockName) const;
  Slha& read(std::istream& is);
  Slha& readFile(const std::string& fileName);
};


class SlhaBlock : public std::list<SlhaLine>
{
public:
  std::string name;

  SlhaBlock() { }
  SlhaBlock(const std::string& blockName) : name(blockName) { }

  SlhaLine& operator()(const std::string& si = "",
                       const std::string& sj = "",
                       const std::string& sk = "",
                       const std::string& sl = "");
  SlhaLine& operator()(const int i,             const int j = m_noIndex,
                       const int k = m_noIndex, const int l = m_noIndex);

private:
  static const int m_noIndex;
};


class SlhaLine : public std::vector<std::string>
{
public:
  SlhaLine() { }
  SlhaLine(const std::string& line) { fromString(line); }

  SlhaLine& operator=(const std::string& line) { return fromString(line); }
  SlhaLine& fromString(const std::string& line);
};


std::ostream& operator<<(std::ostream& os, const Slha& slha);
std::ostream& operator<<(std::ostream& os, const SlhaBlock& block);
std::ostream& operator<<(std::ostream& os, const SlhaLine& line);

double to_double(const std::string& str);

#endif // RPVFIT_SLHA_H

// vim: sw=2 tw=78
