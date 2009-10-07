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
    for (; p < line.end() && isspace((unsigned char) *p); p++) {}

    // Ignore empty lines.
    if (p == line.end()) {
      continue;
    }

    // Ignore comment-only lines.
    if (*p == '#') {
      continue;
    }

    // Ignore lines whose first non-whitespace character matches [^BbDd\d].
    unsigned char c = tolower((unsigned char) *p);
    if (c != 'b' && c != 'd' && !isdigit(c)) {
      // This is an error in the SLHA file, should we throw an exception?
      continue;
    }

    this->parseLine(line);
  }
}

void Slha::parseLine(string& line)
{
  // Create a copy of line with all characters converted to uppercase.
  string line_uc = line;
  for (string::iterator p = line_uc.begin(); p < line_uc.end(); p++) {
    *p = toupper((unsigned char) *p);
  }

  // Remove comment from line_uc.
  size_t comment_begin = line_uc.find("#");
  cout << line_uc.substr(0, comment_begin) << endl;

  size_t pos = line_uc.find("BLOCK");
  if (pos != string::npos) {
    //pos = line_uc.find
    cout <<  line.substr(pos) << endl;
  }

  /*
    sind wir im block statement? oder nicht
     - ja: setze block namen

    ansonsten
  */
 // cout << line_uc << endl;
}

// vim: sw=2 tw=78
