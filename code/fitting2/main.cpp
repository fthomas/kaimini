
extern "C" {
  extern int test_mp_i_;
  void mimic_spheno_mp_run_till_model_bilinear_rparity_(void);
}

#include <iostream>

int main(int argc, char *argv[])
{

    mimic_spheno_mp_run_till_model_bilinear_rparity_();
    //std::cout << test_mp_i_ << std::endl;
    //test_mp_ci_();
    //test_mp_i_++;
    //std::cout << test_mp_i_ << std::endl;
    return 0;
}
