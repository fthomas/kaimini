// Kaimini, a general purpose fitting and analysis front end
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

#include <algorithm>
#include <cstddef>
#include <ctime>
#include <string>
#include <boost/bind.hpp>
#include <gsl/gsl_rng.h>
#include "RandomGenerator.h"

namespace Kaimini {

const std::string RandomGenerator::alnum_ =
  "0123456789"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz";


RandomGenerator::RandomGenerator()
  : seed_(static_cast<unsigned int>(std::time(0)) +
          static_cast<unsigned int>(std::clock())),
    engine(seed_),
    alnum_gen_(engine, uniform_int_distribution(0, alnum_.length() - 1))
{
  gsl_rng_default_seed = seed_;
  gsl_engine = gsl_rng_alloc(gsl_rng_mt19937);
}


RandomGenerator::~RandomGenerator()
{
  gsl_rng_free(gsl_engine);
}


void
RandomGenerator::seed(const unsigned int new_seed)
{
  seed_ = new_seed;
  engine.seed(seed_);

  gsl_rng_default_seed = seed_;
  gsl_rng_set(gsl_engine, seed_);
}


RandomGenerator::uniform_int_generator
RandomGenerator::randUniformInt(const int min, const int max)
{
  return uniform_int_generator(engine, uniform_int_distribution(min, max));
}


RandomGenerator::uniform_real_generator
RandomGenerator::randUniformReal(const double min, const double max)
{
  return uniform_real_generator(engine, uniform_real_distribution(min, max));
}


RandomGenerator::normal_generator
RandomGenerator::randNormal(const double mean, const double stddev)
{
  return normal_generator(engine, normal_distribution(mean, stddev));
}


char
RandomGenerator::randAlnumChar()
{
  return alnum_[alnum_gen_()];
}


std::string
RandomGenerator::randAlnumString(const std::size_t length)
{
  std::string retval(length, char());
  std::generate(retval.begin(), retval.end(),
    boost::bind(&RandomGenerator::randAlnumChar, this));
  return retval;
}

} // namespace Kaimini
