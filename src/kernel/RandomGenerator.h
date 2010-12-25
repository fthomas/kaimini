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

#ifndef KAIMINI_KERNEL_RANDOMGENERATOR_H
#define KAIMINI_KERNEL_RANDOMGENERATOR_H

#include <algorithm>
#include <cstddef>
#include <string>
#include <boost/random.hpp>
#include <boost/utility.hpp>
#include <gsl/gsl_rng.h>

namespace Kaimini {

class RandomGenerator : private boost::noncopyable
{
public:
  typedef boost::uniform_int<>          uniform_int_distribution;
  typedef boost::uniform_real<>         uniform_real_distribution;
  typedef boost::normal_distribution<>  normal_distribution;
  typedef boost::mt19937                engine_type;

  typedef boost::variate_generator<engine_type&, uniform_int_distribution>
                                        uniform_int_generator;
  typedef boost::variate_generator<engine_type&, uniform_real_distribution>
                                        uniform_real_generator;
  typedef boost::variate_generator<engine_type&, normal_distribution>
                                        normal_generator;

public:
  RandomGenerator();
  ~RandomGenerator();

  void
  seed(unsigned int new_seed);

  uniform_int_generator
  randUniformInt(int min, int max);

  uniform_int_generator
  randUniformInt(int max = 9)
  { return randUniformInt(0, max); }

  uniform_real_generator
  randUniformReal(double min, double max);

  uniform_real_generator
  randUniformReal(double max = 1.)
  { return randUniformReal(0., max); }

  normal_generator
  randNormal(double mean, double stddev);

  normal_generator
  randNormal(double stddev = 1.)
  { return randNormal(0., stddev); }

  char
  randDigitChar();

  char
  randAlphaChar();

  char
  randAlnumChar();

  char
  randGraphChar();

  template<class CharGenerator> std::string
  randString(std::size_t length, CharGenerator generator)
  {
    std::string retval(length, char());
    std::generate(retval.begin(), retval.end(), generator);
    return retval;
  }

  std::string
  randDigitString(std::size_t length = 6);

  std::string
  randAlphaString(std::size_t length = 6);

  std::string
  randAlnumString(std::size_t length = 6);

  std::string
  randGraphString(std::size_t length = 6);

private:
  unsigned int seed_;

public:
  engine_type engine;
  gsl_rng* gsl_engine;
};

} // namespace Kaimini

#endif // KAIMINI_KERNEL_RANDOMGENERATOR_H
