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

#include <vector>
#include <gsl/gsl_vector.h>
#include "parameters.h"

using namespace std;
using namespace ROOT::Minuit2;

namespace FISP {

Parameters& Parameters::operator=(const MnUserParameters& par)
{
  MnUserParameters::operator=(par);
  return *this;
}


vector<double> Parameters::getVarParams() const
{
  vector<double> par(VariableParameters());
  const size_t size = Params().size();
  for (size_t i = 0, j = 0; i < size; ++i)
  {
    if (!Parameter(i).IsFixed()) par.at(j++) = Value(i);
  }
  return par;
}


vector<double> Parameters::getVarStepSizes() const
{
  vector<double> step_sizes(VariableParameters());
  const size_t size = Params().size();
  for (size_t i = 0, j = 0; i < size; ++i)
  {
    if (!Parameter(i).IsFixed()) step_sizes.at(j++) = Error(i);
  }
  return step_sizes;
}


gsl_vector* Parameters::getVarParamsGslVec() const
{
  gsl_vector* par = gsl_vector_alloc(VariableParameters());
  const size_t size = Params().size();
  for (size_t i = 0, j = 0; i < size; ++i)
  {
    if (!Parameter(i).IsFixed()) gsl_vector_set(par, j++, Value(i));
  }
  return par;
}


gsl_vector* Parameters::getVarStepSizesGslVec() const
{
  gsl_vector* step_sizes = gsl_vector_alloc(VariableParameters());
  const size_t size = Params().size();
  for (size_t i = 0, j = 0; i < size; ++i)
  {
    if (!Parameter(i).IsFixed()) gsl_vector_set(step_sizes, j++, Error(i));
  }
  return step_sizes;
}


Parameters& Parameters::setVarParams(const vector<double>& v)
{
  assert(v.size() >= VariableParameters());
  const size_t size = Params().size();
  for (size_t i = 0, j = 0; i < size; ++i)
  {
    if (!Parameter(i).IsFixed()) SetValue(i, v.at(j++));
  }
  return *this;
}


Parameters& Parameters::setVarParams(const gsl_vector* v)
{
  assert(v->size >= VariableParameters());
  const size_t size = Params().size();
  for (size_t i = 0, j = 0; i < size; ++i)
  {
    if (!Parameter(i).IsFixed()) SetValue(i, gsl_vector_get(v, j++));
  }
  return *this;
}

} // namespace FISP

// vim: sw=2 tw=78
