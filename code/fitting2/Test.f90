module test

  implicit none
  integer :: i = 2

  contains

    subroutine ci
      i = i + 1
    end subroutine ci
end module
