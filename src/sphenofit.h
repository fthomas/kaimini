// Kaimini
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

#ifndef KAIMINI_SPHENOFIT_H
#define KAIMINI_SPHENOFIT_H

#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include "slhaea.h"
#include "slhafit.h"

namespace Kaimini {

class SPhenoFit : public SLHAFit
{
public:
  explicit SPhenoFit(const std::string& inputFile)
  { setUp(inputFile); }

  void setUp(const std::string& inputFile);
  void tearDown(const std::string& outputFile);

  double chiSquare(const std::vector<double>& v) const;

private:
  boost::filesystem::path mInitialDir;
  boost::filesystem::path mWorkingDir;
  boost::filesystem::path mSPhenoIn;
  boost::filesystem::path mSPhenoOut;

  mutable SLHAea::SLHA mSLHAInput;
};

} // namespace Kaimini

#endif // KAIMINI_SPHENOFIT_H

// vim: sw=2 tw=78
