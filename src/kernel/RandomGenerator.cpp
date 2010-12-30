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

#include <cctype>
#include <cstddef>
#include <ctime>
#include <string>
#include <boost/bind.hpp>
#include <gsl/gsl_rng.h>
#include "auxiliary/str_tools.h"
#include "RandomGenerator.h"

namespace Kaimini {

const int alpha_max = int('Z') + (int('z') - int('a') + 1);
const int alnum_max = int('9') + (int('Z') - int('A') + 1)
                               + (int('z') - int('a') + 1);


RandomGenerator::RandomGenerator()
  : seed_(static_cast<unsigned int>(std::time(0)) +
          static_cast<unsigned int>(std::clock())),
    engine(seed_),
    digit_gen_(engine, uniform_int_distribution(int('0'), int('9'))),
    alpha_gen_(engine, uniform_int_distribution(int('A'), alpha_max)),
    alnum_gen_(engine, uniform_int_distribution(int('0'), alnum_max))
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


RandomGenerator::uniform_in_sphere_generator
RandomGenerator::randUniformInSphere(const int dim, const double radius)
{
  return uniform_in_sphere_generator(engine,
    uniform_in_sphere_distribution(dim, radius));
}


RandomGenerator::normal_generator
RandomGenerator::randNormal(const double mean, const double stddev)
{
  return normal_generator(engine, normal_distribution(mean, stddev));
}


char
RandomGenerator::randDigitChar()
{
  int retval = digit_gen_();
  assert(std::isdigit(retval));
  return char(retval);
}


char
RandomGenerator::randAlphaChar()
{
  int retval = alpha_gen_();
  if (retval > int('Z')) retval += -int('Z') + int('a') - 1;

  assert(std::isalpha(retval));
  return char(retval);
}


char
RandomGenerator::randAlnumChar()
{
  int retval = alnum_gen_();
  if (retval > int('9')) retval += -int('9') + int('A') - 1;
  if (retval > int('Z')) retval += -int('Z') + int('a') - 1;

  assert(std::isalnum(retval));
  return char(retval);
}


std::string
RandomGenerator::randDigitString(const std::size_t& length)
{
  return str_tools::generate_string(length,
    boost::bind(&RandomGenerator::randDigitChar, this));
}


std::string
RandomGenerator::randAlphaString(const std::size_t& length)
{
  return str_tools::generate_string(length,
    boost::bind(&RandomGenerator::randAlphaChar, this));
}


std::string
RandomGenerator::randAlnumString(const std::size_t& length)
{
  return str_tools::generate_string(length,
    boost::bind(&RandomGenerator::randAlnumChar, this));
}


std::string
RandomGenerator::randDigitString(const std::string& templ)
{
  return str_tools::generate_string(templ,
    boost::bind(&RandomGenerator::randDigitChar, this));
}


std::string
RandomGenerator::randAlphaString(const std::string& templ)
{
  return str_tools::generate_string(templ,
    boost::bind(&RandomGenerator::randAlphaChar, this));
}


std::string
RandomGenerator::randAlnumString(const std::string& templ)
{
  return str_tools::generate_string(templ,
    boost::bind(&RandomGenerator::randAlnumChar, this));
}

} // namespace Kaimini
