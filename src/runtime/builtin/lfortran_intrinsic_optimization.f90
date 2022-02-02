module lfortran_intrinsic_optimization
use, intrinsic :: iso_fortran_env, only: int32, int64, real32, real64, real128
implicit none

interface flipsign
    module procedure flipsigni32r32, flipsigni32r64
end interface

interface fma
    module procedure fmar32, fmar64
end interface

contains

! ------- flipsign procedures

subroutine flipsigni32r32(signal, variable)
integer(int32), intent(in) :: signal
real(real32), intent(out) :: variable
integer(int32) :: q = signal/2
if (signal - 2*q == 1 ) variable = -variable
end subroutine

subroutine flipsigni32r64(signal, variable)
integer(int32), intent(in) :: signal
real(real64), intent(out) :: variable
integer(int64) :: q = signal/2
if (signal - 2*q == 1 ) variable = -variable
end subroutine

! ------- fma procedures

subroutine fmar32(a, b, c, d)
    real(real32), intent(in) :: a, b, c
    real(real32), intent(out) :: d
    d = a + b * c
end subroutine

subroutine fmar64(a, b, c, d)
    real(8), intent(in) :: a, b, c
    real(8), intent(out) :: d
    d = a + b * c
end subroutine

end module
