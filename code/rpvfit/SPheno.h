extern "C" {
  /* Control */
  extern double control_mp_delta_mass_;

  /* InputOutput */
  extern bool   inputoutput_mp_add_rparity_;

  /* RPtools */
  void  rptools_mp_chisquare_(bool *add_rparity, double *delta, double *M_GUT,
                              int *kont);

  /* SPhenoDouble */
  extern int    sphenodouble_mp_kont_;
  extern double sphenodouble_mp_m_gut_;

  void  sphenodouble_mp_runtill_model_bilinear_rparity_(void);
  void  sphenodouble_mp_runtill_end_(void);
}

// vim: sw=2 tw=78
