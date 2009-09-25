program Fitting
  ! Load required SPheno modules.
  use Control

  ! Initialize SPheno's control system.
  !Open(ErrCan, file="Messages.out")

!  call PrepareIO(input_file)
  call ReadData
end program Fitting


!subroutine PrepareIO(input_file, error_file)
!  implicit none
!  character(len=128), intent(inout) :: input_file
!
!
!end subroutine PrepareIO


subroutine ReadData
  ! Load required SPheno modules.
  use Control
  use InputOutput

  implicit none
  integer :: kont = -123456
  character(len=128) :: file_name
  logical :: file_exists = .false.

  Iname = Iname + 1
  NameOfUnit(Iname) = "ReadData"

  ! Set file_name to the first command line argument or to "LesHouches.in"
  ! if no command line argument was given.
  call get_command_argument(1, file_name)
  if (len_trim(file_name) == 0) then
    file_name = "LesHouches.in"
  end if

  ! Now check if file file_name exists and abort if not.
  inquire(file=trim(file_name), exist=file_exists)
  if (file_exists == .false.) then
    write(*,*) "File '", trim(file_name), "' does not exist. Please ", &
      & "provide an input file."
    call TerminateProgram
  end if

  ! SLHA file exists so set kont to 1.
  kont = 1

end subroutine ReadData

! vim: sw=2 tw=78
