module lfortran_intrinsic_optimisation
use, intrinsic :: iso_fortran_env, only: int32, int64, real32, real64, real128
implicit none

interface flipsign
    module procedure flipsigni32r32, flipsigni32r64
end interface

! ------- flipsign procedures

elemental real(real32) function flipsigni32r32(signal, variable) result(r)
integer(int32), intent(in) :: signal
real(real32), intent(in) :: variable
end function

elemental real(real64) function flipsigni32r64(signal, variable) result(r)
integer(int32), intent(in) :: signal
real(real64), intent(in) :: variable
end function

end module
