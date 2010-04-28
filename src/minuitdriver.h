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

#include <map>
#include <ostream>
#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/utility.hpp>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MinosError.h>
#include <Minuit2/MnApplication.h>
#include "chisqfunction.h"
#include "driver.h"
#include "kaimini.h"
#include "parameters.h"

namespace Kaimini {

class MinuitDriver : public Driver, private boost::noncopyable
{
public:
  explicit MinuitDriver(ChiSqFunction* func) : mpFunc(func)
  {
    minimizer0Map["MinuitMigrad"]   = &MinuitDriver::runMigrad;
    minimizer1Map["MinuitMigrad"]   = &MinuitDriver::runMigrad;
    minimizer0Map["MinuitMinimize"] = &MinuitDriver::runMinimize;
    minimizer1Map["MinuitMinimize"] = &MinuitDriver::runMinimize;
    minimizer0Map["MinuitScan"]     = &MinuitDriver::runScan;
    minimizer1Map["MinuitScan"]     = &MinuitDriver::runScan;
    minimizer0Map["MinuitSimplex"]  = &MinuitDriver::runSimplex;
    minimizer1Map["MinuitSimplex"]  = &MinuitDriver::runSimplex;
  }

  ChiSqFunction* getFunction() const
  {
    return mpFunc;
  }

  Parameters runMigrad(const Parameters& startParams, unsigned int stra);
  Parameters runMigrad(unsigned int stra)
  {
    return runMigrad(mpFunc->getParameters(), stra);
  }

  Parameters runMigrad()
  {
    return runMigrad(1);
  }

  Parameters runMinimize(const Parameters& startParams, unsigned int stra);
  Parameters runMinimize(unsigned int stra)
  {
    return runMinimize(mpFunc->getParameters(), stra);
  }

  Parameters runMinimize()
  {
    return runMinimize(1);
  }

  Parameters runScan(const Parameters& startParams, unsigned int stra);
  Parameters runScan(unsigned int stra)
  {
    return runScan(mpFunc->getParameters(), stra);
  }

  Parameters runScan()
  {
    return runScan(1);
  }

  Parameters runSimplex(const Parameters& startParams, unsigned int stra);
  Parameters runSimplex(unsigned int stra)
  {
    return runSimplex(mpFunc->getParameters(), stra);
  }

  Parameters runSimplex()
  {
    return runSimplex(1);
  }

  ROOT::Minuit2::FunctionMinimum getFunctionMinimum();

  std::vector<ROOT::Minuit2::MinosError>
  runMinos(const ROOT::Minuit2::FunctionMinimum& minimum,
           unsigned int stra = 1);

  std::vector<ROOT::Minuit2::MinosError>
  runMinos(unsigned int stra = 1);

public:
  typedef Parameters (MinuitDriver::*minimizer0_t)();
  typedef Parameters (MinuitDriver::*minimizer1_t)(unsigned int);

  std::map<std::string, minimizer0_t, iless_than> minimizer0Map;
  std::map<std::string, minimizer1_t, iless_than> minimizer1Map;

private:
  void runHesse(unsigned int stra);
  void sanitize();
  void processResults(const std::string& number, const std::string& name,
                      const ROOT::Minuit2::MnApplication& app);

private:
  ChiSqFunction* mpFunc;
  boost::scoped_ptr<ROOT::Minuit2::FunctionMinimum> mpMinimum;
};


std::ostream&
operator<<(std::ostream& os,
           const std::vector<ROOT::Minuit2::MinosError>& errors);

} // namespace Kaimini

#endif // KAIMINI_MINUITDRIVER_H

// vim: sw=2 tw=78
