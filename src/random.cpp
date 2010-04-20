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

#include <ctime>
#include <string>
#include <gsl/gsl_rng.h>
#include "random.h"

using namespace std;

namespace Kaimini {

// static
const string Random::mAlnum = "0123456789"
                              "abcdefghijklmnopqrstuvwxyz"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


Random::Random()
  : mSeed(static_cast<unsigned int>(time(0)) +
          static_cast<unsigned int>(clock())),
    engine(mSeed),
    mAlnumGen(engine, uniform_int_distribution(0, mAlnum.length()-1))
{
  gsl_rng_default_seed = mSeed;
  gsl_engine = gsl_rng_alloc(gsl_rng_mt19937);
}


Random::~Random()
{
  gsl_rng_free(gsl_engine);
}


int Random::randUniformInt(const int min, const int max)
{
  uniform_int_gen rnd(engine, uniform_int_distribution(min, max));
  return rnd();
}


double Random::randUniformReal(const double min, const double max)
{
  uniform_real_gen rnd(engine, uniform_real_distribution(min, max));
  return rnd();
}


double Random::randNormal(const double mean, const double stddev)
{
  normal_gen rnd(engine, normal_distribution(mean, stddev));
  return rnd();
}


string Random::randString(const size_t length)
{
  string retval(length, char());
  for (string::iterator ch = retval.begin(); ch != retval.end(); ++ch)
  {
      *ch = mAlnum[mAlnumGen()];
  }
  return retval;
}


Random g_rnd;

} // namespace Kaimini

// vim: sw=2 tw=78
