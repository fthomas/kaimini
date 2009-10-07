#include <cctype>
#include <string>
#include <boost/algorithm/string.hpp>

#include "slha.h"

using namespace std;
using namespace boost;

void Slha::read(istream& is)
{
  string line;

  while (getline(is, line)) {
    trim(line);
    if (line.empty())
      continue;

    char fc = tolower(line.at(0));
    if (fc == '#' || fc != 'b')
      continue;
    std::cout << line << std::endl;
  }
}

// vim: sw=2 tw=78
