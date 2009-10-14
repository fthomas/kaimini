#ifndef RPVFIT_SLHA_H
#define RPVFIT_SLHA_H

#include <iostream>
#include <limits>
#include <list>
#include <string>
#include <vector>

const int no_index = std::numeric_limits<int>::max();

class SlhaLine;
class SlhaBlock;


class Slha : public std::list<SlhaBlock>
{
public:
  SlhaBlock& operator()(const std::string& blockName);
  Slha& read(std::istream& is);
  Slha& readFile(const std::string& fileName);
};


class SlhaBlock : public std::list<SlhaLine>
{
public:
  std::string name;

  SlhaBlock() { }
  SlhaBlock(const std::string& blockName) : name(blockName) { }

  SlhaLine& operator()(const std::string& si = "",
                       const std::string& sj = "",
                       const std::string& sk = "",
                       const std::string& sl = "");
  SlhaLine& operator()(const int i,            const int j = no_index,
                       const int k = no_index, const int l = no_index);
};


class SlhaLine : public std::vector<std::string>
{
public:
  SlhaLine() { }
  SlhaLine(const std::string& line) { fromString(line); }

  SlhaLine& operator=(const std::string& line) { return fromString(line); }
  SlhaLine& fromString(const std::string& line);
};


std::ostream& operator<<(std::ostream& os, const Slha& slha);
std::ostream& operator<<(std::ostream& os, const SlhaBlock& block);
std::ostream& operator<<(std::ostream& os, const SlhaLine& line);

#endif // RPVFIT_SLHA_H

// vim: sw=2 tw=78
