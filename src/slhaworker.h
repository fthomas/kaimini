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

#ifndef KAIMINI_SLHAWORKER_H
#define KAIMINI_SLHAWORKER_H

#include <ctime>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include "slhaea.h"
#include "slhainterface.h"

namespace Kaimini {

class SLHAWorker : public SLHAInterface
{
public:
  explicit SLHAWorker(const std::string& inputFile)
  {
    initialize(inputFile);
  }

  explicit SLHAWorker(const SLHAea::SLHA& input)
  {
    initialize(input);
  }

  void initialize(const std::string& inputFile);
  void initialize(const SLHAea::SLHA& input);
  void shutdown(const std::string& outputFile);

  double chiSq(const std::vector<double>& params) const;

private:
  void selectSOFTSUSY();
  void selectSPheno();
  void selectSuSpect();
  void selectXSUSY();

  void selectCalculator(const SLHAea::SLHABlock& block);

private:
  boost::filesystem::path mInitialDir;
  boost::filesystem::path mWorkingDir;
  boost::filesystem::path mTempInputFile;
  boost::filesystem::path mTempOutputFile;

  struct CalculatorInfo
  {
    std::string workingDir;
    std::string inputFile;
    std::string outputFile;
    std::string path;
    std::string cmdline;
    std::string command;
  } mCalcInfo;

  mutable SLHAea::SLHA mSLHAInput;

  std::clock_t mProcTimeStart;
  std::clock_t mProcTimeStop;
  std::time_t  mWallTimeStart;
  std::time_t  mWallTimeStop;
};

} // namespace Kaimini

#endif // KAIMINI_SLHAWORKER_H

// vim: sw=2 tw=78
