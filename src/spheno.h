#ifndef FISP_SPHENO_H
#define FISP_SPHENO_H

namespace SPheno {

#ifdef __cplusplus
extern "C" {
#endif

/* Module Control */
extern double Control_delta_mass;

/* Module InputOutput */
extern char InputOutput_LesHouches_InputFile[256];
extern char InputOutput_LesHouches_OutputFile[256];
extern bool InputOutput_Add_Rparity;

/* Module Model_Data */
extern struct { double re; double im; } Model_Data_eps[3];
extern double Model_Data_vevL[3];

/* Module RPtools */
void RPtools_Calculate_RP_Observables(double* observables, bool* Add_Rparity,
       double* delta, double* M_GUT, int* kont);

/* Module SPhenoDouble */
extern int    SPhenoDouble_kont;
extern double SPhenoDouble_m_Gut;

void SPhenoDouble_RunTill_Model_bilinear_Rparity(void);
void SPhenoDouble_RunTill_End(void);

#ifdef __cplusplus
}
#endif

} // namespace SPheno

#endif // FISP_SPHENO_H

// vim: sw=2 tw=78
