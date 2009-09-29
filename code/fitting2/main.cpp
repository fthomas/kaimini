
extern "C" {
  extern int test_mp_i_;
  void test_mp_ci_(void);
}

#include <iostream>

int main(int argc, char *argv[])
{
 
    std::cout << test_mp_i_ << std::endl;
    test_mp_ci_();
    std::cout << test_mp_i_ << std::endl;
    return 0;
}
