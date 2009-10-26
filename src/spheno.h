#ifndef RPVFIT_SPHENO_H
#define RPVFIT_SPHENO_H

#ifdef __cplusplus
extern "C" {
#endif

// Control module
extern double control_mp_delta_mass_;

// InputOutput module
extern char inputoutput_mp_leshouches_inputfile_[256];
extern char inputoutput_mp_leshouches_outputfile_[256];
extern bool inputoutput_mp_add_rparity_;

// Model_Data module
extern struct { double re; double im; } model_data_mp_eps_[3];
extern double model_data_mp_vevl_[3];

// RPtools module
void rptools_mp_chisquare_(int* observInclude, double* observExpected,
       double* observSigma, double* chiSq, bool* Add_Rparity, double* delta,
       double* M_GUT, int* kont);

// SPhenoDouble module
extern int    sphenodouble_mp_kont_;
extern double sphenodouble_mp_m_gut_;

void sphenodouble_mp_runtill_model_bilinear_rparity_(void);
void sphenodouble_mp_runtill_end_(void);

#ifdef __cplusplus
}
#endif

#endif // RPVFIT_SPHENO_H

// vim: sw=2 tw=78
