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
#include <boost/algorithm/string.hpp>

namespace Kaimini {
  namespace Functional {

template<class PairT>
struct pair_select1st
  : public std::unary_function<PairT, typename PairT::first_type>
{
  typename PairT::first_type&
  operator()(PairT& x) const
  { return x.first; }

  const typename PairT::first_type&
  operator()(const PairT& x) const
  { return x.first; }
};


template<class PairT>
struct pair_select2nd
  : public std::unary_function<PairT, typename PairT::second_type>
{
  typename PairT::second_type&
  operator()(PairT& x) const
  { return x.second; }

  const typename PairT::second_type&
  operator()(const PairT& x) const
  { return x.second; }
};


template<class SequenceT>
struct iequal_to : public std::binary_function<SequenceT, SequenceT, bool>
{
  bool operator()(const SequenceT& a, const SequenceT& b) const
  { return boost::iequals(a, b); }
};


template<class SequenceT>
struct iless_than : public std::binary_function<SequenceT, SequenceT, bool>
{
  bool operator()(const SequenceT& a, const SequenceT& b) const
  { return boost::to_lower_copy(a) < boost::to_lower_copy(b); }
};

  } // namespace Functional
} // namespace Kaimini

#endif // KAIMINI_AUXILIARY_FUNCTIONAL_H
