#ifndef RPVFIT_SLHA_H
#define RPVFIT_SLHA_H

#include <iostream>
#include <string>
#include <vector>

class Slha
{
public:
  void read(std::istream& is);

private:
  std::string mCurrentBlockName;

  void parseLine(std::string& line);
};

class SlhaBlock // list<SlhaDataLine>
{
};

class SlhaLine : public std::vector<std::string>
{
public:
  SlhaLine& fromString(const std::string& line);
};

#endif // RPVFIT_SLHA_H

// vim: sw=2 tw=78
