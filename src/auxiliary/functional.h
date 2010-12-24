// Kaimini, a general purpose fitting and analysis front end
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

#ifndef KAIMINI_AUXILIARY_FUNCTIONAL_H
#define KAIMINI_AUXILIARY_FUNCTIONAL_H

#include <functional>
#include <string>
#include <boost/algorithm/string.hpp>

namespace Kaimini {
namespace functional {

template<class Pair>
struct pair_select1st
  : public std::unary_function<Pair, typename Pair::first_type>
{
  typename Pair::first_type&
  operator()(Pair& x) const
  { return x.first; }

  const typename Pair::first_type&
  operator()(const Pair& x) const
  { return x.first; }
};


template<class Pair>
struct pair_select2nd
  : public std::unary_function<Pair, typename Pair::second_type>
{
  typename Pair::second_type&
  operator()(Pair& x) const
  { return x.second; }

  const typename Pair::second_type&
  operator()(const Pair& x) const
  { return x.second; }
};


template<class Sequence = std::string>
struct iequal_to : public std::binary_function<Sequence, Sequence, bool>
{
  bool
  operator()(const Sequence& a, const Sequence& b) const
  { return boost::iequals(a, b); }
};


template<class Sequence = std::string>
struct iless_than : public std::binary_function<Sequence, Sequence, bool>
{
  bool
  operator()(const Sequence& a, const Sequence& b) const
  { return boost::to_lower_copy(a) < boost::to_lower_copy(b); }
};

} // namespace functional
} // namespace Kaimini

#endif // KAIMINI_AUXILIARY_FUNCTIONAL_H
