#include <cctype>
#include <string>
#include "slha.h"

using namespace std;

void Slha::read(istream& is)
{
  char c;
  string line;

  while(is.get(c)) {
    line += c;

    if (isspace(c) && c != '\n')
      continue;
    
    if (c == '\n') {
      cout << line;
      line.clear();
    }
  }
/*
  string line;

  while (!is.eof()) {
    getline(is, line);

    if (line.empty()) continue;

    bool parse_line = true;

    string::const_iterator p;
    for (p = line.begin(); p < line.end(); p++) {
      if (isspace(*p)) continue;

      // Ignore comment-only lines.
      if (*p == '#') {
        parse_line = false;
        break;
      }

      char c = tolower(*p);
      if (!isalnum(c) || c != 'b' || c != 'd') {
        parse_line = false;
        //break;
      }
      cout << *p;
    }

    if (!parse_line) continue;

    //char fc = tolower(line.at(0));
    //if (fc == '#' || fc != 'b')
    //  continue;
    //std::cout << line << std::endl;
  }
*/
}

// vim: sw=2 tw=78
