program Fitting
  call OpenErrorFile("Messages.out")
  call ReadData("LesHouches.in")
end program Fitting


subroutine OpenErrorFile(error_file)
  use Control

  implicit none
  character(len=*), intent(in) :: error_file

  Iname = Iname + 1
  NameOfUnit(Iname) = "OpenErrorFile"

  open(unit=ErrCan, file=error_file, status="replace")
end subroutine OpenErrorFile


! The filename "LesHouches.in" is hard coded into the LesHouches_Input
! subroutine, so specifying the input_file parameter of this subroutine
! is currently redundant.
subroutine ReadData(input_file)
  use Control
  use InputOutput

  implicit none
  character(len=*), intent(in) :: input_file

  Iname = Iname + 1
  NameOfUnit(Iname) = "ReadData"

  ! Calling the LesHouches_Input subroutine will open "Messages.out" as unit
  ! number ErrCan unless l_open is false. Since we use ErrCan in our own
  ! OpenErrorFile subroutine, we set l_open to false.
  l_open = .false.

  !call LesHouches_Input
end subroutine ReadData






!subroutine PrepareIO(input_file, error_file)
!  implicit none
!  character(len=128), intent(inout) :: input_file
!
!
!end subroutine PrepareIO
!
!
!subroutine ReadData
!  ! Load required SPheno modules.
!  use Control
!  use InputOutput
!
!  implicit none
!  integer :: kont = -123456
!  character(len=128) :: file_name
!  logical :: file_exists = .false.
!
!  Iname = Iname + 1
!  NameOfUnit(Iname) = "ReadData"
!
!  ! Set file_name to the first command line argument or to "LesHouches.in"
!  ! if no command line argument was given.
!  call get_command_argument(1, file_name)
!  if (len_trim(file_name) == 0) then
!    file_name = "LesHouches.in"
!  end if
!
!  ! Now check if file file_name exists and abort if not.
!  inquire(file=trim(file_name), exist=file_exists)
!  if (file_exists == .false.) then
!    write(*,*) "File '", trim(file_name), "' does not exist. Please ", &
!      & "provide an input file."
!    call TerminateProgram
!  end if
!
!  ! SLHA file exists so set kont to 1.
!  kont = 1
!
!end subroutine ReadData

! vim: sw=2 tw=78
