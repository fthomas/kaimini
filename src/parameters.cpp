// Kaimini, a general purpose fitting frontend
// Copyright © 2009-2010 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#include <cassert>
#include <cstddef>
#include <ostream>
#include <sstream>
#include <vector>
#include <gsl/gsl_vector.h>
#include <Minuit2/MinuitParameter.h>
#include "parameters.h"

using namespace std;
using namespace ROOT::Minuit2;

namespace Kaimini {

vector<double> Parameters::getVarParams() const
{
  vector<double> par;
  const vector<MinuitParameter>& mps = getMinuitParameters();

  for (vector<MinuitParameter>::const_iterator mp = mps.begin();
       mp != mps.end(); ++mp)
  {
    if (!mp->IsFixed() && !mp->IsConst()) par.push_back(mp->Value());
  }
  return par;
}


vector<double> Parameters::getVarStepSizes() const
{
  vector<double> step_sizes;
  const vector<MinuitParameter>& mps = getMinuitParameters();

  for (vector<MinuitParameter>::const_iterator mp = mps.begin();
       mp != mps.end(); ++mp)
  {
    if (!mp->IsFixed() && !mp->IsConst()) step_sizes.push_back(mp->Error());
  }
  return step_sizes;
}


gsl_vector* Parameters::getVarParamsGSLVec() const
{
  gsl_vector* par = gsl_vector_alloc(VariableParameters());
  const vector<MinuitParameter>& mps = getMinuitParameters();

  size_t i = 0;
  for (vector<MinuitParameter>::const_iterator mp = mps.begin();
       mp != mps.end(); ++mp)
  {
    if (!mp->IsFixed() && !mp->IsConst())
    { gsl_vector_set(par, i++, mp->Value()); }
  }
  return par;
}


gsl_vector* Parameters::getVarStepSizesGSLVec() const
{
  gsl_vector* step_sizes = gsl_vector_alloc(VariableParameters());
  const vector<MinuitParameter>& mps = getMinuitParameters();

  size_t i = 0;
  for (vector<MinuitParameter>::const_iterator mp = mps.begin();
       mp != mps.end(); ++mp)
  {
    if (!mp->IsFixed() && !mp->IsConst())
    { gsl_vector_set(step_sizes, i++, mp->Error()); }
  }
  return step_sizes;
}


Parameters& Parameters::setVarParams(const vector<double>& v)
{
  assert(v.size() >= VariableParameters());
  for (size_t i = 0, j = 0; i < Params().size(); ++i)
  {
    if (!Parameter(i).IsFixed() && !Parameter(i).IsConst())
    { SetValue(i, v.at(j++)); }
  }
  return *this;
}


Parameters& Parameters::setVarParams(const gsl_vector* v)
{
  assert(v->size >= VariableParameters());
  for (size_t i = 0, j = 0; i < Params().size(); ++i)
  {
    if (!Parameter(i).IsFixed() && !Parameter(i).IsConst())
    { SetValue(i, gsl_vector_get(v, j++)); }
  }
  return *this;
}


vector<vector<MinuitParameter> > transpose(const vector<Parameters>& vecPar)
{
  vector<vector<MinuitParameter> > retval;
  if (vecPar.empty()) return retval;

  const unsigned int inner_size = vecPar[0].getMinuitParameters().size();
  const unsigned int outer_size = vecPar.size();

  // Assert that every element of vecPar is of the same size.
  for (vector<Parameters>::const_iterator it = vecPar.begin(); it !=
       vecPar.end(); ++it)
  { assert(it->getMinuitParameters().size() == inner_size); }

  for (unsigned int i = 0; i < inner_size; ++i)
  {
    vector<MinuitParameter> vec_mp;
    for (unsigned int j = 0; j < outer_size; ++j)
    {
      vec_mp.push_back(vecPar[j].Parameter(i));
    }
    retval.push_back(vec_mp);
  }
  return retval;
}


ostream& operator<<(ostream& os, const MinuitParameter& mp)
{
  stringstream limits; limits << "[ ";
  if (mp.HasLowerLimit()) limits << mp.LowerLimit();
  limits << ", ";
  if (mp.HasUpperLimit()) limits << mp.UpperLimit() << " ";
  limits << "]";

  os << "MinuitParameter:"                  << endl
     << "    number    : " << mp.Number()   << endl
     << "    name      : " << mp.GetName()  << endl
     << "    value     : " << mp.Value()    << endl
     << "    step_size : " << mp.Error()    << endl
     << "    limits    : " << limits.str()  << endl
     << "    fixed     : " << mp.IsFixed()  << endl
     << "    constant  : " << mp.IsConst()  << endl;
  return os;
}


ostream& operator<<(ostream& os, const Parameters& par)
{
  const vector<MinuitParameter>& mps = par.getMinuitParameters();

  os << "Parameters:" << endl;
  for (vector<MinuitParameter>::const_iterator mp = mps.begin();
       mp != mps.end(); ++mp)
  {
    stringstream limits; limits << "[ ";
    if (mp->HasLowerLimit()) limits << mp->LowerLimit();
    limits << ", ";
    if (mp->HasUpperLimit()) limits << mp->UpperLimit() << " ";
    limits << "]";

    os << "    - number    : " << mp->Number()  << endl
       << "      name      : " << mp->GetName() << endl
       << "      value     : " << mp->Value()   << endl
       << "      step_size : " << mp->Error()   << endl
       << "      limits    : " << limits.str()  << endl
       << "      fixed     : " << mp->IsFixed() << endl
       << "      constant  : " << mp->IsConst() << endl
       << endl;
  }
  return os;
}

} // namespace Kaimini

// vim: sw=2 tw=78
