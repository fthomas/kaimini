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
#include <sstream>
#include <string>
#include <vector>

namespace FISP {

class SlhaLine;
class SlhaBlock;
class Slha;

std::ostream& operator<<(std::ostream& os, const SlhaLine& line);
std::ostream& operator<<(std::ostream& os, const SlhaBlock& block);
std::ostream& operator<<(std::ostream& os, const Slha& slha);


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
  { return impl_[n]; }

  const std::string& operator[](std::size_t n) const
  { return impl_[n]; }

  std::string& at(std::size_t n)
  { return impl_.at(n); }

  const std::string& at(std::size_t n) const
  { return impl_.at(n); }

  SlhaLine& append(const std::string& rhs)
  { return str(str() + rhs); }

  std::string& back()
  { return impl_.back(); }

  const std::string& back() const
  { return impl_.back(); }

  iterator begin()
  { return impl_.begin(); }

  const_iterator begin() const
  { return impl_.begin(); }

  bool empty() const
  { return size() == 1 && impl_[0] == ""; }

  iterator end()
  { return impl_.end(); }

  const_iterator end() const
  { return impl_.end(); }

  std::string& front()
  { return impl_.front(); }

  const std::string& front() const
  { return impl_.front(); }

  reverse_iterator rbegin()
  { return impl_.rbegin(); }

  const_reverse_iterator rbegin() const
  { return impl_.rbegin(); }

  reverse_iterator rend()
  { return impl_.rend(); }

  const_reverse_iterator rend() const
  { return impl_.rend(); }

  std::size_t size() const
  { return impl_.size(); }

  SlhaLine& str(const std::string& line);

  std::string str() const;

  std::string strPlain() const;

private:
  std::vector<std::string> impl_;
  std::string mLineFormat;
};


class SlhaBlock
{
public:
  typedef std::vector<SlhaLine>::iterator iterator;
  typedef std::vector<SlhaLine>::const_iterator const_iterator;
  typedef std::vector<SlhaLine>::reverse_iterator reverse_iterator;
  typedef std::vector<SlhaLine>::const_reverse_iterator
          const_reverse_iterator;

  SlhaBlock(const std::string& name = "") : mName(name) {}

  SlhaLine& operator[](const std::vector<std::string>& keys);
  const SlhaLine& operator[](const std::vector<std::string>& keys) const;

  SlhaLine& operator[](const std::vector<int>& intKeys);
  const SlhaLine& operator[](const std::vector<int>& intKeys) const;

  SlhaLine& operator[](const std::string& keysStr);
  const SlhaLine& operator[](const std::string& keysStr) const;

  SlhaLine& at(const std::string& s0 = "", const std::string& s1 = "",
               const std::string& s2 = "", const std::string& s3 = "");

  const
  SlhaLine& at(const std::string& s0 = "", const std::string& s1 = "",
               const std::string& s2 = "", const std::string& s3 = "") const;

  SlhaLine& at(int i0, int i1 = nind, int i2 = nind, int i3 = nind);

  const
  SlhaLine& at(int i0, int i1 = nind, int i2 = nind, int i3 = nind) const;

  SlhaLine& back()
  { return impl_.back(); }

  const SlhaLine& back() const
  { return impl_.back(); }

  iterator begin()
  { return impl_.begin(); }

  const_iterator begin() const
  { return impl_.begin(); }

  SlhaBlock& clear()
  { impl_.clear(); return *this; }

  bool empty() const
  { return impl_.empty(); }

  iterator end()
  { return impl_.end(); }

  const_iterator end() const
  { return impl_.end(); }

  iterator erase(iterator position)
  { return impl_.erase(position); }

  iterator erase(iterator first, iterator last)
  { return impl_.erase(first, last); }

  iterator find(const std::vector<std::string>& keys);
  const_iterator find(const std::vector<std::string>& keys) const;

  SlhaLine& front()
  { return impl_.front(); }

  const SlhaLine& front() const
  { return impl_.front(); }

  SlhaBlock& name(const std::string& newName)
  { mName = newName; return *this; }

  const std::string& name() const
  { return mName; }

  SlhaBlock& pop_back()
  { impl_.pop_back(); return *this; }

  SlhaBlock& push_back(const SlhaLine& line)
  { impl_.push_back(line); return *this; }

  SlhaBlock& push_back(const std::string& line)
  { impl_.push_back(SlhaLine(line)); return *this; }

  reverse_iterator rbegin()
  { return impl_.rbegin(); }

  const_reverse_iterator rbegin() const
  { return impl_.rbegin(); }

  reverse_iterator rend()
  { return impl_.rend(); }

  const_reverse_iterator rend() const
  { return impl_.rend(); }

  std::size_t size() const
  { return impl_.size(); }

  SlhaBlock& str(const std::string& block);

  std::string str() const
  { std::stringstream ss(""); ss << *this; return ss.str(); }

private:
  std::string mName;
  std::vector<SlhaLine> impl_;
  static const int nind;
};


class Slha
{
public:
  typedef std::vector<SlhaBlock>::iterator iterator;
  typedef std::vector<SlhaBlock>::const_iterator const_iterator;
  typedef std::vector<SlhaBlock>::reverse_iterator reverse_iterator;
  typedef std::vector<SlhaBlock>::const_reverse_iterator
          const_reverse_iterator;

  Slha() {}

  Slha(const std::string filename)
  { readFile(filename); }

  SlhaBlock& operator[](const std::string& blockName);
  const SlhaBlock& operator[](const std::string& blockName) const;

  SlhaBlock& back()
  { return impl_.back(); }

  const SlhaBlock& back() const
  { return impl_.back(); }

  iterator begin()
  { return impl_.begin(); }

  const_iterator begin() const
  { return impl_.begin(); }

  Slha& clear()
  { impl_.clear(); return *this; }

  bool empty() const
  { return impl_.empty(); }

  iterator end()
  { return impl_.end(); }

  const_iterator end() const
  { return impl_.end(); }

  iterator erase(iterator position)
  { return impl_.erase(position); }

  iterator erase(iterator first, iterator last)
  { return impl_.erase(first, last); }

  SlhaBlock& front()
  { return impl_.front(); }

  const SlhaBlock& front() const
  { return impl_.front(); }

  Slha& pop_back()
  { impl_.pop_back(); return *this; }

  Slha& push_back(const SlhaBlock& block)
  { impl_.push_back(block); return *this; }

  Slha& read(std::istream& is);
  Slha& readFile(const std::string& filename);

  reverse_iterator rbegin()
  { return impl_.rbegin(); }

  const_reverse_iterator rbegin() const
  { return impl_.rbegin(); }

  reverse_iterator rend()
  { return impl_.rend(); }

  const_reverse_iterator rend() const
  { return impl_.rend(); }

  std::size_t size() const
  { return impl_.size(); }

  Slha& str(const std::string& slhaStr)
  { std::stringstream ss(""); ss << slhaStr; return read(ss); }

  std::string str() const
  { std::stringstream ss(""); ss << *this; return ss.str(); }

  Slha& writeFile(const std::string& filename);

private:
  std::vector<SlhaBlock>  impl_;
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
