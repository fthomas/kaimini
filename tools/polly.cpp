#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <map>
#include <ostream>
#include <sstream>
#include "slhaea.h"

using namespace std;
using namespace SLHAea;

double polynomial(const map<int, double>& params, const double x)
{
  double value = 0.;
  for (map<int, double>::const_iterator p = params.begin();
       p != params.end(); ++p)
  {
    value += pow(x, p->first) * p->second;
  }
  return value;
}

int main(int argc, char* argv[])
{
  if (argc < 1)
  {
    cerr << "Error: Input file required as first argument." << endl;
    exit(EXIT_FAILURE);
  }

  ifstream ifs(argv[1]);
  const SLHA input(ifs);

  SLHABlock params_block = input.at("Parameters");
  map<int, double> params;

  for (SLHABlock::const_iterator line = params_block.begin();
       line != params_block.end(); ++line)
  {
    if (!line->is_data_line()) continue;
    params[to_<int>(line->at(0))] = to_<double>(line->at(1));
  }

  SLHA output;
  output.push_back(params_block);
  output["Values"][""] = "BLOCK Values";

  stringstream ss;
  ss.precision(8);
  ss.setf(ios_base::scientific);

  for (int i = 0; i < 10; ++i)
  {
    ss.str("");
    ss << " " << i << "  " << setw(15)
       << polynomial(params, static_cast<double>(i));
    output["Values"][""] = ss.str();
  }

  cout << output;
  return 0;
}
