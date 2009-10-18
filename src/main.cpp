#include <boost/lexical_cast.hpp>
#include "slha.h"
#include "spheno.h"

using namespace std;
using namespace boost;

int main(int argc, char *argv[])
{
  Slha s;
  s.readFile("LesHouches.in");
  cout << lexical_cast<double>( s("RPVFitObserv")(7)[2] )  << endl;
  cout << to_double( s("RPVFitObserv")(7)[2] ) << endl ;

  sphenodouble_mp_runtill_model_bilinear_rparity_();

  rptools_mp_chisquare_(&inputoutput_mp_add_rparity_, &control_mp_delta_mass_,
                        &sphenodouble_mp_m_gut_, &sphenodouble_mp_kont_);

  sphenodouble_mp_runtill_end_();

  return 0;
}

// vim: sw=2 tw=78
