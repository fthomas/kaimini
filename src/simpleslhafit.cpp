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
#include <ostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "kaimini.h"
#include "simpleslhafit.h"
#include "slhaea.h"

using namespace std;
using namespace SLHAea;
namespace fs = boost::filesystem;

namespace Kaimini {

void SimpleSLHAFit::setUp(const string& inputFile)
{
  ifstream src(inputFile.c_str());
  if (!src) exit_file_open_failed(inputFile);

  mSLHAInput.clear();
  src >> mSLHAInput;
  src.close();

  setDataPoints(mSLHAInput);
  setParameters(mSLHAInput);

  if (mSLHAInput.count("KaiminiCalculator") > 0)
  { selectCalculator(mSLHAInput.at("KaiminiCalculator")); }
  else
  { selectSPheno(); }

  mInitialDir = fs::initial_path<fs::path>();
  mWorkingDir = mInitialDir / mWorkingDirStr;
  mTmpInFile  = mWorkingDir / mTmpInFileStr;
  mTmpOutFile = mWorkingDir / mTmpOutFileStr;

  try
  {
    //if (!fs::exists(mWorkingDir)) fs::create_directory(mWorkingDir);
    mWorkingDir = create_temp_directory(mWorkingDir);
    mTmpInFile  = mWorkingDir / mTmpInFileStr;
    mTmpOutFile = mWorkingDir / mTmpOutFileStr;

    if (fs::exists(mTmpInFile)) fs::remove(mTmpInFile);
    fs::copy_file(fs::path(inputFile), mTmpInFile);
  }
  catch (fs::basic_filesystem_error<fs::path>& ex)
  {
    cerr << ex.what() << endl;
    exit(EXIT_FAILURE);
  }

  //fs::current_path(mWorkingDir);
  chdir(mWorkingDir.file_string().c_str());
}


void SimpleSLHAFit::tearDown(const string& outputFile)
{
  //fs::current_path(mInitialDir);
  chdir(mInitialDir.file_string().c_str());

  ofstream dest(outputFile.c_str());
  if (!dest) exit_file_open_failed(outputFile);
  dest << result();

  fs::ifstream src(mTmpOutFile);
  if (!src) exit_file_open_failed(mTmpOutFile.file_string());
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


double SimpleSLHAFit::chiSquare(const vector<double>& v) const
{
  // Write the parameters ‘v’ into the calculator's input file.
  writeParameters(v, mSLHAInput);
  fs::ofstream ofs(mTmpInFile, ios_base::out | ios_base::trunc);
  if (!ofs) exit_file_open_failed(mTmpInFile.file_string());
  ofs << mSLHAInput;
  ofs.close();

  string cmd = mCommand + " " + mCmdline;
  system(cmd.c_str());

  // Read the data points from the calculator's output file.
  fs::ifstream ifs(mTmpOutFile, ios_base::in);
  if (!ifs) exit_file_open_failed(mTmpOutFile.file_string());
  SLHA output(ifs);
  ifs.close();
  readDataPoints(output);

  double chisq = dps_add_residuals(mDataPoints);

  if (g_verbose_output)
  {
    for (size_t i = 0; i < v.size(); ++i)
    { cout << "par_" << i << " :  " << v[i] << endl; }
    cout << "chi^2 :  " << chisq << endl << endl;
  }

  return chisq;
}


void SimpleSLHAFit::selectSOFTSUSY()
{
  mWorkingDirStr = ".kaimini-SOFTSUSY.";
  mTmpInFileStr  = "softsusy.in";
  mTmpOutFileStr = "softsusy.out";
  mCommand = "softpoint.x";
  mCmdline = "leshouches < softsusy.in > softsusy.out";
}


void SimpleSLHAFit::selectSPheno()
{
  mWorkingDirStr = ".kaimini-SPheno.";
  mTmpInFileStr  = "LesHouches.in";
  mTmpOutFileStr = "SPheno.spc";
  mCommand = "SPheno";
  mCmdline = "";
}


void SimpleSLHAFit::selectSuSpect()
{
  mWorkingDirStr = ".kaimini-SuSpect.";
  mTmpInFileStr  = "suspect2_lha.in";
  mTmpOutFileStr = "suspect2_lha.out";
  mCommand = "suspect2";
  mCmdline = "";
}


void SimpleSLHAFit::selectXSUSY()
{
  mWorkingDirStr = ".kaimini-XSUSY.";
  mTmpInFileStr  = "in.dat";
  mTmpOutFileStr = "out.dat";
  mCommand = "XSUSY";
  mCmdline = "< in.dat";
}


void SimpleSLHAFit::selectCalculator(const SLHABlock& block)
{
  mWorkingDirStr = ".kaimini.";
  mTmpInFileStr  = "slha.in";
  mTmpOutFileStr = "slha.out";
  mCommand = "";
  mCmdline = "";

  for(SLHABlock::const_iterator line = block.begin(); line != block.end();
      ++line)
  {
    size_t size = line->data_size();
    if (size < 2) continue;

    if ("1" == (*line)[0])
    {
      string calculator = (*line)[1];
      mWorkingDirStr = ".kaimini-" + calculator + ".";

      if (boost::iequals(calculator, "SOFTSUSY"))
      { selectSOFTSUSY(); }
      else if (boost::iequals(calculator, "SPheno"))
      { selectSPheno(); }
      else if (boost::iequals(calculator, "SuSpect"))
      { selectSuSpect(); }

      if (size > 2)
      {
        string cmd = (*line)[2];
        if (!fs::exists(cmd))
        {
          cerr << "Error: file ‘" << cmd << "’ does not exist" << endl;
          exit(EXIT_FAILURE);
        }
        mCommand = cmd;
      }
      else if (mCommand.empty())
      { mCommand = (*line)[1]; }
    }
    else if ("2" == (*line)[0])
    {
      mTmpInFileStr  = (*line)[1];
    }
    else if ("3" == (*line)[0])
    {
      mTmpOutFileStr = (*line)[1];
    }
    else if ("4" == (*line)[0])
    {
      mCmdline = (*line)[1];
    }
  }
}

} // namespace Kaimini

// vim: sw=2 tw=78
