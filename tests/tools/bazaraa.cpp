// http://imo.rz.tu-bs.de/~degbers/konvdisk08/akd03.pdf
// f(x, y) = (x - 2)^4 + (x - 2*y)^2

#include <cmath>
#include <iostream>
#include <boost/lexical_cast.hpp>

int main(int argc, char* argv[])
{
  double x = 0., y = 0.;

  if (argc > 2)
  {
    x = boost::lexical_cast<double>(argv[1]);
    y = boost::lexical_cast<double>(argv[2]);
  }

  double f = std::pow(x - 2, 4) + std::pow(x - 2*y, 2);
  std::cout << std::showpoint << f << std::endl;
}
