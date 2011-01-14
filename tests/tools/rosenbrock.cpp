// http://en.wikipedia.org/wiki/Rosenbrock_function
// f(x, y) = (1 - x)^2 + 100 (y - x^2)^2

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

  double f = std::pow(1 - x, 2) + 100. * std::pow(y - x*x, 2);
  std::cout << std::showpoint << f << std::endl;
}
