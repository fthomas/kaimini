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

    if (line.empty()) {
      continue;
    }

    unsigned char c = toupper((unsigned char) line[0]);
    if (c == '#') {
      continue;
    }
    if (c != 'B' && c != 'D' && !isdigit(c)) {
      cerr << "Warning (SLHA): invalid line in input: " << line << endl;
      continue;
    }

    parseLine(line);
  }

  mCurrentBlockName.clear();
}

void Slha::parseLine(string& line)
{
  string line_uc = to_upper_copy(line);

  size_t comment_begin = line_uc.find("#");
  line_uc = trim_right_copy(line_uc.substr(0, comment_begin));

  if (isdigit((unsigned char) line[0])) {
    if (mCurrentBlockName.empty()) {
      cerr << "Warning (SLHA): data line outside a (valid) block: "
           << line << endl;
      return;
    }

    // ?
  }
  else if (line_uc.substr(0,6) == "BLOCK ") {
    mCurrentBlockName.clear();
    string block_name = trim_left_copy(line.substr(6, line_uc.length()-6));

    if (block_name.find_first_of(" \t\n\v\f\r") != string::npos) {
      cerr << "Warning (SLHA): block name contains whitespace: "
           << block_name << endl;
      return;
    }

    mCurrentBlockName = block_name;
  }
}

// vim: sw=2 tw=78
