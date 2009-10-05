extern "C" {

  extern bool inputoutput_mp_add_rparity_;
  extern double control_mp_delta_mass_;

  extern int    mimicspheno_mp_kont_;
  extern double mimicspheno_mp_m_gut_;

  void mimicspheno_mp_runtill_model_bilinear_rparity_(void);
  void mimicspheno_mp_runtillend_(void);

  void rptools_mp_chisquare_(bool *add_rparity, double *delta, double *M_GUT, int *kont);
}

int main(int argc, char *argv[])
{
  mimicspheno_mp_runtill_model_bilinear_rparity_();

  rptools_mp_chisquare_(&inputoutput_mp_add_rparity_, &control_mp_delta_mass_, &mimicspheno_mp_m_gut_,
                        &mimicspheno_mp_kont_);

  mimicspheno_mp_runtillend_();
  return 0;
}

// vim: sw=2 tw=78
