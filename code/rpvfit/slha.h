#ifndef RPVFIT_SLHA_H
#define RPVFIT_SLHA_H

#include <iostream>

class Slha
{
public:
  void read(std::istream& is);

private:
  void parseLine(std::string& s);
};

#endif // RPVFIT_SLHA_H

// vim: sw=2 tw=78
