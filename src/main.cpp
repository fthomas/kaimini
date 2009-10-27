// RPVFit
// Copyright © 2009 Frank S. Thomas <fthomas@physik.uni-wuerzburg.de>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <cstdlib>
#include <getopt.h>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_vector.h>
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

  while (true)
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

  // Set our (user-supplied) i/o filenames as SPheno's i/o filenames.
  if (input_file.size() < sizeof(inputoutput_mp_leshouches_inputfile_))
    strcpy(inputoutput_mp_leshouches_inputfile_, input_file.c_str());
  else cerr << "Warning: input filename too long. SPheno will use its "
            << "default input filename." << endl;

  if (output_file.size() < sizeof(inputoutput_mp_leshouches_outputfile_))
    strcpy(inputoutput_mp_leshouches_outputfile_, output_file.c_str());
  else cerr << "Warning: output filename too long. SPheno will use its "
            << "default output filename." << endl;

  Slha slha_input;
  slha_input.readFile(input_file);

  Fcn fcn;
  fcn.setParameters(slha_input);
  fcn.setObservables(slha_input);

  sphenodouble_mp_runtill_model_bilinear_rparity_();

/*
  gsl_vector* v = gsl_vector_alloc(6);
  gsl_vector_set(v,1, 1.1);

  fcn.function(fcn.uparGsl, NULL);
  gsl_vector_free(v);
*/
/*
  gsl_multimin_function my;
  my.n = 1;
  my.f = &Fcn::chiSquare;
  my.params = NULL;

  const gsl_multimin_fminimizer_type* T;
  gsl_multimin_fminimizer* m2;

  //T = gsl_multimin_fminimizer_nmsimplex;
  //T = gsl_multimin_fminimizer_nmsimplex2;
  T = gsl_multimin_fminimizer_nmsimplex2rand;
  m2 = gsl_multimin_fminimizer_alloc(T,1);

  gsl_vector* x = gsl_vector_alloc(1);
  gsl_vector_set(x, 0, 1.0E-5);
  //gsl_vector_set(x, 1, 0.00001);


  gsl_vector* ss = gsl_vector_alloc(1);
  gsl_vector_set(ss, 0, 1.0E-6);
  //gsl_vector_set(ss, 1, 0.000001);

  gsl_multimin_fminimizer_set(m2, &my, x, ss);
size_t iter = 0;
int status = 0;
gsl_multimin_fminimizer_iterate (m2);

       do
         {
           iter++;
           status = gsl_multimin_fminimizer_iterate (m2);
     
           //if (status)
           //  break;
     
           //status = gsl_multimin_test_gradient (s->gradient, 1e-3);
     
          // if (status == GSL_SUCCESS)
          //   printf ("Minimum found at:\n");
     
           printf ("%5d %.5f  %10.5f\n", int(iter),
                   gsl_vector_get (m2->x, 0), 
                   m2->fval);
     
         }
       //while (status == GSL_CONTINUE && iter < 100 );
       while ( iter < 100 );
*/

  MnMinimize minimizer(fcn, fcn.upar);
  FunctionMinimum m = minimizer();
  
  fcn(m.UserParameters().Params());
  sphenodouble_mp_runtill_end_();

  cout << m << endl;

  return 0;
}

// vim: sw=2 tw=78
