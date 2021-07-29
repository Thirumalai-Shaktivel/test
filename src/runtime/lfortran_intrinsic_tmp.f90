module lfortran_intrinsic_tmp
implicit none

contains

  real function tiny(x) result(r)
    real, intent(in) :: x
    ! r = 1.17549435E-38
    ! Doesn't matter right now, let the backend figure it out
    r = 0.0
  end function tiny

end module
