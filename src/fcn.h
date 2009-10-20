#ifndef RPVFIT_FCN_H
#define RPVFIT_FCN_H

#include <vector>
#include <Minuit2/FCNBase.h>
#include <Minuit2/MnUserParameters.h>
#include "slha.h"
 
class Fcn : public ROOT::Minuit2::FCNBase
{
public:
  double operator()(const std::vector<double>& par) const;
  double Up() const;

  ROOT::Minuit2::MnUserParameters upar;
  void setUserParameters(const Slha& input);
  void setFixedParameters(const Slha& input) const;
};

#endif // RPVFIT_FCN_H

// vim: sw=2 tw=78
