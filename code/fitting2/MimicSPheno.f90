module mimic_spheno

  use BranchingRatios
  use Chargino3Decays
  use Control
  use EplusEminusProduction
  use InputOutput
  use LoopFunctions
  use LoopMasses
  use LowEnergy
  use Mathematics
  use Model_Data
  use NMSSM_tools
  use RPtools
  use RGEs
  use StandardModel
  use SugraRuns

  implicit none

  real(dp) :: epsI, deltaM

  integer, parameter :: p_max = 100
  complex(dp) :: Ylp(3,3)
  real(dp) :: Ecms(p_max), Pm(p_max), Pp(p_max), SigSup(p_max,6,6),     &
            & SigSdown(p_max,6,6), SigSle(p_max,6,6), SigSn(p_max,3,3), &
            & SigC(p_max,5,5), SigChi0(p_max,7,7), SigS0(p_max,5),      &
            & SigSP(p_max,5,4), SigHp(p_max,7,7)
  logical :: ISR(p_max) = .false., Beam(p_max) = .false.

  real(dp) :: BRbtosgamma, Bs_mumu, BrBToSLL, BR_Bu_TauNu, a_e, a_mu, a_tau, &
            & d_e, d_mu, d_tau, BrMutoEGamma, BrTautoEGamma, BrTautoMuGamma, &
            & BrMu3e, BrTau3e, BrTau3Mu, BR_Z_e_mu, BR_Z_e_tau, BR_Z_mu_tau, &
            & BtoSNuNu
  complex(dp) :: DeltaMBd, DeltaMBs

  logical :: CalcTBD
  integer :: kont, i1, i_min(3)
  real(dp) :: m_Gut, ratioWoM

  contains

  subroutine run_till_Model_bilinear_Rparity
    call Set_All_Parameters_0()

    kont = 0
    call ReadingData(kont)
  end subroutine run_till_Model_bilinear_Rparity

  subroutine ReadingData(kont)
    implicit none
    integer, intent(out) :: kont

    logical :: file_exists

    kont = -123456
    inquire(file="LesHouches.in", exist=file_exists)

    if (file_exists) then
      kont = 1
      call LesHouches_Input(kont, HighScaleModel, Ecms, Pm, Pp, ISR, F_GMSB)
      LesHouches_Format = .true.
    else
      write(*,*) &
        & "The file 'LesHouches.in' has not been found. ", &
        & "Please provide an input file."
      call TerminateProgram
    endif
  end subroutine ReadingData

end module mimic_spheno

! vim: sw=2 tw=78
