#include <fstream>
#include <map>
#include <string>
#include "slha.h"
#include "spheno.h"
#include <tr1/unordered_map>

using namespace std;
using namespace std::tr1;

int main(int argc, char *argv[])
{

  SlhaLine l;
  string s = string(" 1  300  # irgendwas");

cout<<  l.fromString(s)[1];

  SlhaLine::iterator p = l.begin();
  for(; p < l.end(); p++) {
    cout << *p << ", ";
  }

/*
  std::ifstream fs("test/LesHouches.in");
  Slha s;
  s.read( fs );
*/

  //sphenodouble_mp_runtill_model_bilinear_rparity_();

  //rptools_mp_chisquare_(&inputoutput_mp_add_rparity_, &control_mp_delta_mass_,
  //                      &sphenodouble_mp_m_gut_, &sphenodouble_mp_kont_);

  //sphenodouble_mp_runtill_end_();

  return 0;
}

// vim: sw=2 tw=78
