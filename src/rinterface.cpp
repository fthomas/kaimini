// FISP - Fitting Interface for SPheno
// Copyright © 2009 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#include <string>
#include <vector>
#include <Rcpp.h>
#include "fisp.h"
#include "rpvfit.h"
#include "slha.h"
#include "spheno.h"

using namespace std;
using namespace FISP;

extern "C" {

RpvFit gRpvFit;

SEXP rpvfit_init(SEXP args)
{
  RcppParams par(args);
  string input_file = par.getStringValue("input");
  string output_file = par.getStringValue("output");

  set_filenames(input_file, output_file);
  Slha slha_input(input_file);

  gRpvFit.setParameters(slha_input);
  gRpvFit.setObservables(slha_input);

  SPhenoDouble_RunTill_Model_bilinear_Rparity();
  return R_NilValue;
}


SEXP rpvfit_get_params()
{
  vector<double> params = gRpvFit.getParameters()->getParams();
  vector<double> var_params = gRpvFit.getParameters()->getVarParams();

  RcppResultSet rs;
  rs.add("params", params);
  rs.add("varParams", var_params);
  return rs.getReturnList();
}


SEXP rpvfit_chi_square(SEXP params)
{
  RcppVector<double> var_params(params);
  gRpvFit.getParameters()->setVarParams(var_params.stlVector());
  double chisq = gRpvFit.chiSquare();

  RcppResultSet rs;
  rs.add("chisq", chisq);
  return rs.getReturnList();
}


SEXP rpvfit_finish()
{
  SPhenoDouble_RunTill_End();
  return R_NilValue;
}

} // extern "C"

// vim: sw=2 tw=78
