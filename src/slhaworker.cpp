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
#include <ctime>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
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

  initialize(mSLHAInput);
}


void SLHAWorker::initialize(const Coll& input)
{
  mWallTimeStart = time(0);
  mProcTimeStart = clock();

  // Avoid self-assignment if we are called by initialize(const string&).
  if (&mSLHAInput != &input) mSLHAInput = input;

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
    ofstream dest(mTempInputFile.file_string().c_str());
    dest << mSLHAInput;
    dest.close();
  }
  catch (fs::basic_filesystem_error<fs::path>& ex)
  {
    cerr << ex.what() << endl;
    exit(EXIT_FAILURE);
  }

  chdir(mWorkingDir.file_string().c_str());
}


void SLHAWorker::shutdown(const string& outputFile)
{
  chdir(mInitialDir.file_string().c_str());

  mWallTimeStop = time(0);
  mProcTimeStop = clock();

  const double wall_time = difftime(mWallTimeStop, mWallTimeStart);
  const double proc_time =
    static_cast<double>(mProcTimeStop - mProcTimeStart) / CLOCKS_PER_SEC;

  processRuntime(wall_time, proc_time);

  ofstream dest(outputFile.c_str());
  if (!dest) exit_file_open_failed(outputFile);
  dest << result();

  if (fs::exists(mTempOutputFile))
  {
    fs::ifstream src(mTempOutputFile);
    if (!src) exit_file_open_failed(mTempOutputFile.file_string());
    dest << src.rdbuf();
    src.close();
  }

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
  size_t block_count = 0;

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
    const Coll slha_output(src);
    src.close();

    block_count = slha_output.size();
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

  if (mSaveAllPoints && block_count > 3)
  {
    const_cast<SLHAWorker*>(this)->saveIntermediatePoint(params, chisq);
  }

  return chisq;
}


void SLHAWorker::saveIntermediatePoint(const std::vector<double>& params,
                                       const double& chiSquare)
{
  static int counter = 0;

  Parameters curr_params = getParameters();
  curr_params.setVarParams(params);

  const string output_file =
      boost::str(boost::format("../%1$014.8f_%2$08d")
                 % chiSquare % ++counter);

  if (fs::exists(output_file)) return;

  ofstream dest(output_file.c_str());
  if (!dest) exit_file_open_failed(output_file);

  clearResults();
  processDataPoints();
  processParameters(&curr_params);

  dest << result().at("KaiminiChiSquare");
  dest << result().at("KaiminiDataPointsOut");
  dest << result().at("KaiminiParametersOut");
  dest << result().at("KaiminiParameterDifferences");
  dest.close();
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


void SLHAWorker::selectCalculator(const Block& block)
{
  mCalcInfo.workingDir = ".kaimini.";
  mCalcInfo.inputFile  = "slha.in";
  mCalcInfo.outputFile = "slha.out";
  mCalcInfo.path       = "";
  mCalcInfo.cmdline    = "";
  mCalcInfo.command    = "";

  for(Block::const_iterator line = block.begin();
      line != block.end(); ++line)
  {
    if (!line->is_data_line()) continue;
    if (line->data_size() < 3)
    {
      warn_line_ignored(block.name(), line->str());
      continue;
    }

    const string key = (*line)[1];

    if (boost::iequals(key, "calculator"))
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
    else if (boost::iequals(key, "path"))
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
    else if (boost::iequals(key, "inputfile"))
    {
      mCalcInfo.inputFile = (*line)[2];
    }
    else if (boost::iequals(key, "outputfile"))
    {
      mCalcInfo.outputFile = (*line)[2];
    }
    else if (boost::iequals(key, "cmdline"))
    {
      string cmdline = line->str();
      cmdline = cmdline.substr(cmdline.find((*line)[2]));
      cmdline = boost::trim_copy(cmdline.substr(0, cmdline.find('#')));

      mCalcInfo.cmdline = cmdline;
      mCalcInfo.command = mCalcInfo.path + " " + mCalcInfo.cmdline;
    }
    else warn_line_ignored(block.name(), line->str());
  }
}

} // namespace Kaimini

// vim: sw=2 tw=78
