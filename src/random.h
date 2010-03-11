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

#ifndef KAIMINI_RANDOM_H
#define KAIMINI_RANDOM_H

#include <string>
#include <boost/random.hpp>
#include <gsl/gsl_rng.h>

namespace Kaimini {

class Random
{
public:
  Random();
  ~Random();

  int randUniformInt(int min, int max);
  int randUniformInt(int max = 9)
  {
    return randUniformInt(0, max);
  }

  double randUniformReal(double min, double max);
  double randUniformReal(double max = 1.)
  {
    return randUniformReal(0., max);
  }

  double randNormal(double mean, double stddev);
  double randNormal(double stddev = 1.)
  {
    return randNormal(0., stddev);
  }

  std::string randString(std::size_t length = 6);

private:
  Random(const Random&);
  Random& operator=(const Random&);

public:
  typedef boost::uniform_int<> uniform_int_distribution;
  typedef boost::uniform_real<> uniform_real_distribution;
  typedef boost::normal_distribution<> normal_distribution;
  typedef boost::mt19937 engine_type;

  typedef boost::variate_generator<engine_type&, uniform_int_distribution>
          uniform_int_gen;
  typedef boost::variate_generator<engine_type&, uniform_real_distribution>
          uniform_real_gen;
  typedef boost::variate_generator<engine_type&, normal_distribution>
          normal_gen;

  engine_type engine;
  gsl_rng* gsl_engine;

private:
  static const std::string mAlnum;
  uniform_int_gen mAlnumGen;
};


extern Random g_rnd;

} // namespace Kaimini

#endif // KAIMINI_RANDOM_H

// vim: sw=2 tw=78
