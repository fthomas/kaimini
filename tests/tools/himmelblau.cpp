// http://en.wikipedia.org/wiki/Himmelblau%27s_function
// f(x, y) = (x^2 + y - 11)^2 + (x + y^2 - 7)^2

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

  double f = std::pow(x*x + y - 11, 2) + std::pow(x + y*y - 7, 2);
  std::cout << std::showpoint << f << std::endl;
}
