// Kaimini, a general purpose fitting and analysis front end
// Copyright © 2010-2011 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
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

#include <cstddef>
#include <string>
#include <boost/random.hpp>
#include <gsl/gsl_rng.h>
#include "utility/uniform_in_sphere.hpp"

namespace Kaimini {

class RandomGenerator
{
public:
  typedef boost::uniform_int<>          uniform_int_distribution;
  typedef boost::uniform_real<>         uniform_real_distribution;
  typedef boost::uniform_in_sphere<>    uniform_in_sphere_distribution;
  typedef boost::normal_distribution<>  normal_distribution;
  typedef boost::mt19937                engine_type;

  typedef boost::variate_generator<engine_type&, uniform_int_distribution>
                                        uniform_int_generator;
  typedef boost::variate_generator<engine_type&, uniform_real_distribution>
                                        uniform_real_generator;
  typedef boost::variate_generator<engine_type&, uniform_in_sphere_distribution>
                                        uniform_in_sphere_generator;
  typedef boost::variate_generator<engine_type&, normal_distribution>
                                        normal_generator;

public:
  RandomGenerator();
  RandomGenerator(const RandomGenerator& rand_gen);
  ~RandomGenerator();

  RandomGenerator&
  operator=(const RandomGenerator& rand_gen);

  void
  seed(unsigned int new_seed);

  uniform_int_generator
  uniformIntGen(int min, int max);

  uniform_int_generator
  uniformIntGen(int max = 9)
  { return uniformIntGen(0, max); }

  uniform_real_generator
  uniformRealGen(double min, double max);

  uniform_real_generator
  uniformRealGen(double max = 1.)
  { return uniformRealGen(0., max); }

  uniform_in_sphere_generator
  uniformInSphereGen(int dim, double radius);

  uniform_in_sphere_generator
  uniformInSphereGen(double radius = 1.)
  { return uniformInSphereGen(3, radius); }

  normal_generator
  normalGen(double mean, double stddev);

  normal_generator
  normalGen(double stddev = 1.)
  { return normalGen(0., stddev); }

  char
  digitChar();

  char
  alphaChar();

  char
  alnumChar();

  std::string
  digitString(std::size_t length = 6);

  std::string
  alphaString(std::size_t length = 6);

  std::string
  alnumString(std::size_t length = 6);

  std::string
  digitString(const std::string& templ);

  std::string
  alphaString(const std::string& templ);

  std::string
  alnumString(const std::string& templ);

private:
  unsigned int seed_;

public:
  engine_type engine;
  gsl_rng* gsl_engine;

private:
  uniform_int_generator digit_gen_;
  uniform_int_generator alpha_gen_;
  uniform_int_generator alnum_gen_;
};

} // namespace Kaimini

#endif // KAIMINI_KERNEL_RANDOMGENERATOR_H
