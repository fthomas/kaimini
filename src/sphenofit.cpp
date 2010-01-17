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

#include <fstream>
#include <string>
#include <vector>
#include "kaimini.h"
#include "slhaea.h"
#include "sphenofit.h"

using namespace std;
using namespace SLHAea;

namespace Kaimini {

void SPhenoFit::init(const string& inputFile)
{
  ifile = "LesHouches.in";
  ofile = "SPheno.spc";

  // cp filename ifile
  ifstream source(inputFile.c_str(), ios::binary);
  ofstream dest(ifile.c_str(), ios::binary);

  if (!source) exit_file_open_failed(inputFile);
  if (!dest) exit_file_open_failed(ifile);

  dest << source.rdbuf();
  dest.close();

  // Set data points and parameters from input file.
  source.seekg(0, ios::beg);
  const SLHA input(source);
  source.close();

  setDataPoints(input);
  setParameters(input);
}


void SPhenoFit::writeResult(const string& outputFile) const
{
  // Write the fit result into outputFile.
  ofstream dest(outputFile.c_str());
  if (!dest) exit_file_open_failed(outputFile);
  dest << result();

  // Append SPheno's output to outputFile.
  ifstream source(ofile.c_str());
  if (!source) exit_file_open_failed(ofile);
  dest << source.rdbuf();
}


double SPhenoFit::chiSquare(const vector<double>& v) const
{
  fstream fs;
  SLHA input, output;

  // Read the content of SPheno's input file.
  fs.open(ifile.c_str(), ios::in);
  if (!fs) exit_file_open_failed(ifile);
  fs >> input;
  fs.close();

  // Write the parameters ‘v’ into SPheno's input file.
  writeParameters(paramTransformIntToExt(v), input);
  fs.open(ifile.c_str(), ios::out | ios::trunc);
  if (!fs) exit_file_open_failed(ifile);
  fs << input;
  fs.close();

  system("SPheno");

  // Read the data points from SPheno's output file.
  fs.open(ofile.c_str(), ios::in);
  if (!fs) exit_file_open_failed(ofile);
  fs >> output;
  fs.close();
  readDataPoints(output);

  mChiSq = sumWtSqResiduals(mDataPoints.begin(), mDataPoints.end());
  return mChiSq;
}

} // namespace Kaimini

// vim: sw=2 tw=78
