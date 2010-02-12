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

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
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
  initVars();

  if (!fs::exists(mWorkingDir)) fs::create_directory(mWorkingDir);

  if (fs::exists(mTmpInFile)) fs::remove(mTmpInFile);
  fs::copy_file(fs::path(inputFile), mTmpInFile);

  ifstream src(inputFile.c_str());
  if (!src) exit_file_open_failed(inputFile);

  mSLHAInput.clear();
  src >> mSLHAInput;
  src.close();

  setDataPoints(mSLHAInput);
  setParameters(mSLHAInput);

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

  //fs::remove(mWorkingDir);
}


double SimpleSLHAFit::chiSquare(const vector<double>& v) const
{
  // Write the parameters ‘v’ into the calculator's input file.
  writeParameters(paramTransformIntToExt(v), mSLHAInput);
  fs::ofstream ofs(mTmpInFile, ios_base::out | ios_base::trunc);
  if (!ofs) exit_file_open_failed(mTmpInFile.file_string());
  ofs << mSLHAInput;
  ofs.close();

  system(mCommand.c_str());

  // Read the data points from the calculator's output file.
  fs::ifstream ifs(mTmpOutFile, ios_base::in);
  if (!ifs) exit_file_open_failed(mTmpOutFile.file_string());
  SLHA output(ifs);
  ifs.close();
  readDataPoints(output);

  mChiSq = sumWtSqResiduals(mDataPoints.begin(), mDataPoints.end());

  // Output the current parameter values and chi^2.
  cout.precision(8);
  cout.setf(ios_base::scientific);
  for (size_t i = 0; i < v.size(); ++i)
  { cout << "par_" << i << ":  " << v[i] << endl; }
  cout << "chi^2:  " << mChiSq << endl << endl;

  return mChiSq;
}

} // namespace Kaimini

// vim: sw=2 tw=78
