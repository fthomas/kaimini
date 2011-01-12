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

#ifndef KAIMINI_KERNEL_STOPWATCH_H
#define KAIMINI_KERNEL_STOPWATCH_H

#include <ctime>
#include <sys/times.h>

namespace Kaimini {

class Stopwatch
{
public:
  Stopwatch();

  void
  start();

  void
  stop();

  void
  reset();

  bool
  running() const
  { return running_; }

  static double
  resolution()
  { return 1. / kClockTicksPerSec_; }

  double
  user();

  double
  sys();

  double
  real();

  double
  wall();

private:
  void
  recordRef1();

  void
  recordRef2();

  void
  updateTimes();

  void
  updateTimesIfRunning();

  void
  resetTimes()
  { user_ = sys_ = real_ = wall_ = 0.; }

private:
  bool running_;

  double user_;
  double sys_;
  double real_;
  double wall_;

  struct tms   tms_r1_,  tms_r2_;
  std::clock_t real_r1_, real_r2_;
  std::time_t  wall_r1_, wall_r2_;

  static const double kClockTicksPerSec_;
};

} // namespace Kaimini

#endif // KAIMINI_KERNEL_STOPWATCH_H
