#include <fstream>

#include "slha.h"
#include "spheno.h"

int main(int argc, char *argv[])
{
  std::ifstream fs("test/LesHouches.in");
  Slha s;
  s.read( fs );

  //sphenodouble_mp_runtill_model_bilinear_rparity_();

  //rptools_mp_chisquare_(&inputoutput_mp_add_rparity_, &control_mp_delta_mass_,
  //                      &sphenodouble_mp_m_gut_, &sphenodouble_mp_kont_);

  //sphenodouble_mp_runtill_end_();

  return 0;
}

// vim: sw=2 tw=78
