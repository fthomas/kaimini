extern "C" {
  void mimicspheno_mp_runtill_model_bilinear_rparity_(void);
  void mimicspheno_mp_runtillend_(void);

  void rptools_mp_chisquare_(void);
}

int main(int argc, char *argv[])
{
  mimicspheno_mp_runtill_model_bilinear_rparity_();
  rptools_mp_chisquare_();
  //mimicspheno_mp_runtillend_();
  return 0;
}

// vim: sw=2 tw=78
