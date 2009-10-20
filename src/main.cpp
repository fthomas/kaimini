#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MnMinimize.h>
#include <Minuit2/MnPrint.h>
#include "fcn.h"
#include "slha.h"
#include "spheno.h"

using namespace std;
using namespace ROOT::Minuit2;

int main(int argc, char *argv[])
{
  Slha s;
  s.readFile("LesHouches.in");

  Fcn fcn;
  fcn.setUserParameters(s);
  fcn.setFixedParameters(s);

  sphenodouble_mp_runtill_model_bilinear_rparity_();

  MnMinimize minimizer(fcn, fcn.upar);
  FunctionMinimum m = minimizer();

  sphenodouble_mp_runtill_end_();

  cout << m << endl;

  return 0;
}

// vim: sw=2 tw=78
