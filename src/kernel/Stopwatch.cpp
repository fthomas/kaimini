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

#include "Stopwatch.h"
#include <ctime>
#include <sys/times.h>
#include <unistd.h>

namespace Kaimini {

const double Stopwatch::kClockTicksPerSec_ =
  static_cast<double>(sysconf(_SC_CLK_TCK));


Stopwatch::Stopwatch()
  : running_(false),
    user_(0.),
    sys_(0.),
    real_(0.),
    wall_(0.),
    tms_r1_(),
    tms_r2_(),
    real_r1_(),
    real_r2_(),
    wall_r1_(),
    wall_r2_() {}


void
Stopwatch::start()
{
  if (running()) return;

  running_ = true;
  recordRef1();
}


void
Stopwatch::stop()
{
  if (!running()) return;

  recordRef2();
  running_ = false;
  updateTimes();
}


void
Stopwatch::reset()
{
  resetTimes();
  if (running()) recordRef1();
}


double
Stopwatch::user()
{
  updateTimesIfRunning();
  return user_;
}


double
Stopwatch::sys()
{
  updateTimesIfRunning();
  return sys_;
}


double
Stopwatch::real()
{
  updateTimesIfRunning();
  return real_;
}


double
Stopwatch::wall()
{
  updateTimesIfRunning();
  return wall_;
}


void
Stopwatch::recordRef1()
{
  real_r1_ = times(&tms_r1_);
  wall_r1_ = std::time(0);
}


void
Stopwatch::recordRef2()
{
  real_r2_ = times(&tms_r2_);
  wall_r2_ = std::time(0);
}


void
Stopwatch::updateTimes()
{
  user_ += static_cast<double>(tms_r2_.tms_utime - tms_r1_.tms_utime) /
    kClockTicksPerSec_;

  sys_  += static_cast<double>(tms_r2_.tms_stime - tms_r1_.tms_stime) /
    kClockTicksPerSec_;

  real_ += static_cast<double>(real_r2_ - real_r1_) /
    kClockTicksPerSec_;

  wall_ += std::difftime(wall_r2_, wall_r1_);
}


void
Stopwatch::updateTimesIfRunning()
{
  if (!running()) return;

  recordRef2();
  updateTimes();
  recordRef1();
}

} // namespace Kaimini
