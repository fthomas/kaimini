
! This subroutine adds unit_name to the NameOfUnit array defined in SPheno's
! Control module. Its purpose is to enforce the principle of information
! hiding. In most units the Iname and NameOfUnit variables don't need to be
! exposed so it is hidden by this subroutine.
subroutine RecordUnitName(unit_name)
  use Control

  character(len=*), intent(in) :: unit_name

  Iname = Iname + 1
  NameOfUnit(Iname) = unit_name
end subroutine RecordUnitName

! vim: sw=2 tw=78
