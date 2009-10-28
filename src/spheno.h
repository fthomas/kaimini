#ifndef FISP_SPHENO_H
#define FISP_SPHENO_H

namespace SPheno
{

#ifdef __cplusplus
extern "C"
{
#endif

/* Module Control */
extern double control_mp_delta_mass_;

#define Control_delta_mass control_mp_delta_mass_

/* Module InputOutput */
extern char inputoutput_mp_leshouches_inputfile_[256];
extern char inputoutput_mp_leshouches_outputfile_[256];
extern bool inputoutput_mp_add_rparity_;

#define InputOutput_LesHouches_InputFile \
  inputoutput_mp_leshouches_inputfile_
#define InputOutput_LesHouches_OutputFile \
  inputoutput_mp_leshouches_outputfile_
#define InputOutput_Add_Rparity inputoutput_mp_add_rparity_

/* Module Model_Data */
extern struct { double re; double im; } model_data_mp_eps_[3];
extern double model_data_mp_vevl_[3];

#define Model_Data_eps model_data_mp_eps_
#define Model_Data_vevL model_data_mp_vevl_

/* Module RPtools */
void rptools_mp_chisquare_(int* observInclude, double* observExpected,
       double* observSigma, double* chiSq, bool* Add_Rparity, double* delta,
       double* M_GUT, int* kont);

inline void RPtools_ChiSquare(int* observInclude, double* observExpected,
       double* observSigma, double* chiSq, bool* Add_Rparity, double* delta,
       double* M_GUT, int* kont)
{
  rptools_mp_chisquare_(observInclude, observExpected, observSigma, chiSq,
       Add_Rparity, delta, M_GUT, kont);
};

/* Module SPhenoDouble */
extern int    sphenodouble_mp_kont_;
extern double sphenodouble_mp_m_gut_;

#define SPhenoDouble_kont sphenodouble_mp_kont_
#define SPhenoDouble_m_Gut sphenodouble_mp_m_gut_

void sphenodouble_mp_runtill_model_bilinear_rparity_(void);
void sphenodouble_mp_runtill_end_(void);

inline void SPhenoDouble_RunTill_Model_bilinear_Rparity()
{ sphenodouble_mp_runtill_model_bilinear_rparity_(); }
inline void SPhenoDouble_RunTill_End()
{ sphenodouble_mp_runtill_end_(); }

#ifdef __cplusplus
}
#endif

} // namespace SPheno

#endif // FISP_SPHENO_H

// vim: sw=2 tw=78
