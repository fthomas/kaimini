// Kaimini, a general purpose fitting frontend
// Copyright © 2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#ifndef KAIMINI_MINUITDRIVER_H
#define KAIMINI_MINUITDRIVER_H

#include <memory>
#include <ostream>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MinosError.h>
#include "genericfit.h"

namespace Kaimini {

class MinuitDriver
{
public:
  explicit MinuitDriver(GenericFit* fit) : mpFit(fit) {}

  Parameters runMigrad(unsigned int stra);
  Parameters runMigrad()
  { return runMigrad(1); }

  Parameters runMinimize(unsigned int stra);
  Parameters runMinimize()
  { return runMinimize(1); }

  Parameters runScan(unsigned int stra);
  Parameters runScan()
  { return runScan(1); }

  Parameters runSimplex(unsigned int stra);
  Parameters runSimplex()
  { return runSimplex(1); }

  ROOT::Minuit2::FunctionMinimum getFunctionMinimum();

  std::vector<ROOT::Minuit2::MinosError>
  runMinos(const ROOT::Minuit2::FunctionMinimum& minimum,
           unsigned int stra = 1);

  std::vector<ROOT::Minuit2::MinosError>
  runMinos(unsigned int stra = 1);

private:
  void sanitize();

private:
  GenericFit* mpFit;
  boost::scoped_ptr<ROOT::Minuit2::FunctionMinimum> mpMinimum;
};


template<template <typename E, typename A = std::allocator<E> > class Cont>
inline std::ostream&
operator<<(std::ostream& os, const Cont<ROOT::Minuit2::MinosError>& errors)
{
  os << "MinosErrors:" << std::endl;
  for (typename Cont<ROOT::Minuit2::MinosError>::const_iterator
       err = errors.begin(); err != errors.end(); ++err)
  {
    os << "    - number : "  << err->Parameter() << std::endl
       << "      valid  : "  << err->IsValid()   << std::endl
       << "      lower  : "  << err->Lower()     << std::endl
       << "      upper  :  " << err->Upper()     << std::endl
       << std::endl;
  }
  return os;
}

} // namespace Kaimini

#endif // KAIMINI_MINUITDRIVER_H

// vim: sw=2 tw=78
