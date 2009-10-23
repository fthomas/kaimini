#include <cstdlib>
#include <getopt.h>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MnMinimize.h>
#include <Minuit2/MnPrint.h>
#include "fcn.h"
#include "slha.h"
#include "spheno.h"

using namespace std;
using namespace ROOT::Minuit2;

void parse_command_line(int argc, char** argv, string* inputFileName,
                        string* outputFileName)
{
  int c;
  int option_index = 0;
  struct option long_options[] =
    {
      {"input",  required_argument, 0, 'i'},
      {"output", required_argument, 0, 'o'},
      {0, 0, 0, 0}
    };

  while (1)
  {
    c = getopt_long(argc, argv, "i:o:", long_options, &option_index);
    if (-1 == c) break;

    switch (c)
    {
      case 'i':
        inputFileName->assign(optarg);
        break;

      case 'o':
        outputFileName->assign(optarg);
        break;

      default:
        abort();
    }
  }
}


int main(int argc, char* argv[])
{
  // Use SPheno's defaul input/output filenames as our own default filenames.
  string input_file  = "LesHouches.in";
  string output_file = "SPheno.spc";
  parse_command_line(argc, argv, &input_file, &output_file);

  if (input_file.size() < sizeof(inputoutput_mp_leshouches_file_))
    strcpy(inputoutput_mp_leshouches_file_, input_file.c_str());
  else cerr << "Warning: input filename too long. SPheno will use its "
            << "default input filename." << endl;

  Slha slha_input;
  slha_input.readFile(input_file);

  Fcn fcn;
  fcn.setUserParameters(slha_input);
  fcn.setFixedParameters(slha_input);

  sphenodouble_mp_runtill_model_bilinear_rparity_();

  MnMinimize minimizer(fcn, fcn.upar);
  FunctionMinimum m = minimizer();
  fcn(m.UserParameters().Params());

  sphenodouble_mp_runtill_end_();

  cout << m << endl;

  return 0;
}

// vim: sw=2 tw=78
