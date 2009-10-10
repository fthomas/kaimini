#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "slha.h"

using namespace std;
using namespace boost;

void Slha::read(istream& is)
{
  string line;

  while (getline(is, line))
  {
    trim(line);

    if (line.empty()) continue;

    unsigned char c = toupper((unsigned char) line[0]);
    if ('#' == c) continue;
    if ('B' != c && 'D' != c && !isdigit(c))
    {
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

  if (isdigit((unsigned char) line.at(0)))
  {
    if (mCurrentBlockName.empty())
    {
      cerr << "Warning (SLHA): data line outside a (valid) block: "
           << line << endl;
      return;
    }

    string line_wo_comment = trim_right_copy(line.substr(0, comment_begin));

    vector<string> parts;
    split(parts, line_wo_comment, is_space(), token_compress_on);
    parts.insert(parts.begin(), line);
    parts.push_back(line.substr(comment_begin));

    // ?
  }
  else if (line_uc.substr(0,6) == "BLOCK ")
  {
    mCurrentBlockName.clear();
    string block_name = trim_left_copy(line.substr(6, line_uc.length()-6));

    if (!all(block_name, is_alnum()))
    {
      cerr << "Warning (SLHA): block name contains non-alphanumeric "
           << "characters: " << block_name << endl;
      return;
    }

    mCurrentBlockName = block_name;
  }
}

SlhaLine& SlhaLine::fromString(const string& line)
{
  size_t comment_begin = line.find("#");
  string line_wo_comment = trim_right_copy(line.substr(0, comment_begin));

  split(this&, line_wo_comment, is_space(), token_compress_on);
  insert(begin(), line);
  push_back(line.substr(comment_begin));

  return *this;
}

// vim: sw=2 tw=78
