#include <cctype>
#include <string>
#include "slha.h"

using namespace std;

void Slha::read(istream& is)
{
  string line;

  while (getline(is, line)) {
    // Iterate over line up to the first non-whitespace character.
    string::const_iterator p = line.begin();
    for (; p < line.end() && isspace(*p); p++) {}

    // Ignore empty lines.
    if (p == line.end()) {
      continue;
    }

    // Ignore lines whose first non-whitespace character matches (#|[^BbDd\d]).
    char c = tolower(*p);
    if (c == '#' || (c != 'b' && c != 'd' && !isdigit(c))) {
      continue;
    }

    cout << line << endl;
  }
/*
    for (p = line.begin(); p < line.end(); p++) {
      // Ignore leading white space.
      if (isspace(*p)) {
        continue;
      }

      // Don't parse comment-only lines.
      if (*p == '#') {
        parse_line = false;
        break;
      }

      char c = tolower(*p);
      if (c != 'b' && c != 'd' && !isdigit(c)) {
        cout << c;
        parse_line = false;
      }
        break;
    }
    //cout << line;
    //cout << parse_line;

  }
*/


/*
  char c;
  string line;
  bool ignore_line = false;

  while(is.get(c)) {
    line += c;

    // ignoriere whitespace
    //   beginnt zeile mit # oder irgendwas das nicht zahl und auch nicht b
    //   und d ist, ignoriere zeile ... ansonsten lese zeile

    if (!ignore_line && !isspace(c)) {
      if (c == '#')
        ignore_line = true;
    }

    if (c == '\n') {
      cout << line;
      line.clear();
    }
  }
*/
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
