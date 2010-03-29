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

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "kaimini.h"
#include "slhaea.h"
#include "slhaworker.h"

using namespace std;
using namespace SLHAea;
namespace fs = boost::filesystem;

namespace Kaimini {

void SLHAWorker::initialize(const string& inputFile)
{
  ifstream src(inputFile.c_str());
  if (!src) exit_file_open_failed(inputFile);

  mSLHAInput.clear();
  src >> mSLHAInput;
  src.close();

  setDataPoints(mSLHAInput);
  setParameters(mSLHAInput);

  if (mSLHAInput.count("KaiminiCalculator") > 0)
  {
    selectCalculator(mSLHAInput.at("KaiminiCalculator"));
  }
  else
  {
    selectSPheno();
  }

  mInitialDir     = fs::initial_path<fs::path>();
  mWorkingDir     = mInitialDir / mCalcInfo.workingDir;
  mTempInputFile  = mWorkingDir / mCalcInfo.inputFile;
  mTempOutputFile = mWorkingDir / mCalcInfo.outputFile;

  try
  {
    mWorkingDir     = create_temp_directory(mWorkingDir);
    mTempInputFile  = mWorkingDir / mCalcInfo.inputFile;
    mTempOutputFile = mWorkingDir / mCalcInfo.outputFile;

    if (fs::exists(mTempInputFile)) fs::remove(mTempInputFile);
    fs::copy_file(fs::path(inputFile), mTempInputFile);
  }
  catch (fs::basic_filesystem_error<fs::path>& ex)
  {
    cerr << ex.what() << endl;
    exit(EXIT_FAILURE);
  }

  // fs::current_path(mWorkingDir);
  chdir(mWorkingDir.file_string().c_str());
}


void SLHAWorker::shutdown(const string& outputFile)
{
  // fs::current_path(mInitialDir);
  chdir(mInitialDir.file_string().c_str());

  ofstream dest(outputFile.c_str());
  if (!dest) exit_file_open_failed(outputFile);
  dest << result();

  fs::ifstream src(mTempOutputFile);
  if (!src) exit_file_open_failed(mTempOutputFile.file_string());
  dest << src.rdbuf();

  src.close();
  dest.close();

  try
  {
    fs::remove_all(mWorkingDir);
  }
  catch (fs::basic_filesystem_error<fs::path>& ex)
  {
    cerr << ex.what() << endl;
  }
}


double SLHAWorker::chiSq(const vector<double>& params) const
{
  double chisq = 0.;

  #pragma omp critical
  {
    writeParameters(params, mSLHAInput);

    fs::ofstream dest(mTempInputFile, ios_base::out | ios_base::trunc);
    if (!dest) exit_file_open_failed(mTempInputFile.file_string());
    dest << mSLHAInput;
    dest.close();

    system(mCalcInfo.command.c_str());

    fs::ifstream src(mTempOutputFile, ios_base::in);
    if (!src) exit_file_open_failed(mTempOutputFile.file_string());
    const SLHA slha_output(src);
    src.close();

    readDataPoints(slha_output);

    chisq = dps_add_residuals(mDataPoints);
  }

  if (g_verbose_output)
  {
    for (size_t i = 0; i < params.size(); ++i)
    {
      cout << "par_" << i << " : ";
      cout << setprecision(8) << setw(15) << params[i] << endl;
    }
    cout << "chi^2 : ";
    cout << setprecision(8) << setw(15) << chisq << endl << endl;
  }

  return chisq;
}


void SLHAWorker::selectSOFTSUSY()
{
  mCalcInfo.workingDir = ".kaimini-SOFTSUSY.";
  mCalcInfo.inputFile  = "softsusy.in";
  mCalcInfo.outputFile = "softsusy.out";
  mCalcInfo.path       = "softpoint.x";
  mCalcInfo.cmdline    = "leshouches < softsusy.in > softsusy.out";
  mCalcInfo.command    = mCalcInfo.path + " " + mCalcInfo.cmdline;
}


void SLHAWorker::selectSPheno()
{
  mCalcInfo.workingDir = ".kaimini-SPheno.";
  mCalcInfo.inputFile  = "LesHouches.in";
  mCalcInfo.outputFile = "SPheno.spc";
  mCalcInfo.path       = "SPheno";
  mCalcInfo.cmdline    = "";
  mCalcInfo.command    = mCalcInfo.path;
}


void SLHAWorker::selectSuSpect()
{
  mCalcInfo.workingDir = ".kaimini-SuSpect.";
  mCalcInfo.inputFile  = "suspect2_lha.in";
  mCalcInfo.outputFile = "suspect2_lha.out";
  mCalcInfo.path       = "suspect2";
  mCalcInfo.cmdline    = "";
  mCalcInfo.command    = mCalcInfo.path;
}


void SLHAWorker::selectXSUSY()
{
  mCalcInfo.workingDir = ".kaimini-XSUSY.";
  mCalcInfo.inputFile  = "in.dat";
  mCalcInfo.outputFile = "out.dat";
  mCalcInfo.path       = "XSUSY";
  mCalcInfo.cmdline    = "< in.dat";
  mCalcInfo.command    = mCalcInfo.path + " " + mCalcInfo.cmdline;
}


void SLHAWorker::selectCalculator(const SLHABlock& block)
{
  mCalcInfo.workingDir = ".kaimini.";
  mCalcInfo.inputFile  = "slha.in";
  mCalcInfo.outputFile = "slha.out";
  mCalcInfo.path       = "";
  mCalcInfo.cmdline    = "";
  mCalcInfo.command    = "";

  for(SLHABlock::const_iterator line = block.begin();
      line != block.end(); ++line)
  {
    if (line->data_size() < 3) continue;

    if ((*line)[1] == "calculator")
    {
      const string calculator = (*line)[2];
      mCalcInfo.path          = calculator;
      mCalcInfo.workingDir    = ".kaimini-" + calculator + ".";

      if (boost::iequals(calculator, "SOFTSUSY"))
      {
        selectSOFTSUSY();
      }
      else if (boost::iequals(calculator, "SPheno"))
      {
        selectSPheno();
      }
      else if (boost::iequals(calculator, "SuSpect"))
      {
        selectSuSpect();
      }
      else if (boost::iequals(calculator, "XSUSY"))
      {
        selectXSUSY();
      }
    }
    else if ((*line)[1] == "path")
    {
      const string path = (*line)[2];
      if (!fs::exists(path))
      {
        cerr << "Error: file ‘" << path << "’ does not exist" << endl;
        exit(EXIT_FAILURE);
      }
      mCalcInfo.path    = path;
      mCalcInfo.command = path;
    }
    else if ((*line)[1] == "outputfile")
    {
      mCalcInfo.inputFile = (*line)[2];
    }
    else if ((*line)[1] == "inputfile")
    {
      mCalcInfo.outputFile = (*line)[2];
    }
    else if ((*line)[1] == "cmdline")
    {
      mCalcInfo.cmdline = (*line)[2];
      mCalcInfo.command = mCalcInfo.path + " " + mCalcInfo.cmdline;
    }
  }
}

} // namespace Kaimini

// vim: sw=2 tw=78