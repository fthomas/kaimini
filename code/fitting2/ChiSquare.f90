program Fitting
  !call OpenErrorFile("Messages.out")
  call ReadData("LesHouches.in")
end program Fitting


! This subroutine is currently not applicable since the file with unit number
! ErrCan will be opened in the LesHouches_Input subroutine and its name,
! "Messages.out", is also hard coded there.
subroutine OpenErrorFile(error_file)
  use Control

  implicit none
  character(len=*), intent(in) :: error_file

  call RecordUnitName("OpenErrorFile")

  open(unit=ErrCan, file=error_file, status="replace")
end subroutine OpenErrorFile


! The filename "LesHouches.in" is hard coded into the LesHouches_Input
! subroutine, so specifying the input_file parameter of this subroutine
! is currently redundant.
subroutine ReadData(input_file)
  use InputOutput

  implicit none
  character(len=*), intent(in) :: input_file

  integer :: kont = -123456
  ! The following are intent(out) parameters of LesHouches_Input:
  character(len=15) :: high_scale_model
  real(dp) :: Ecms(100), Pm(100), Pp(100), Fgmsb
  logical :: l_ISR(100) = .false.

  call RecordUnitName("ReadData")

  call LesHouches_Input(kont, high_scale_model, Ecms, Pm, Pp, l_ISR, Fgmsb)
end subroutine ReadData

! vim: sw=2 tw=78
