module lfortran_intrinsic_trig
use, intrinsic :: iso_fortran_env, only: sp => real32, dp => real64
implicit none
private
public sin

real(dp), parameter :: pi = 3.1415926535897932384626433832795_dp

interface sin
    module procedure a7ssin, a6dsin
end interface

contains


! sin --------------------

real(dp) function abs3(x) result(r)
real(dp), intent(in) :: x
if (x >= 0) then
    r = x
else
    r = -x
end if
end function

elemental integer function a1floor(x) result(r)
real(dp), intent(in) :: x
if (x >= 0) then
    r = x
else
    r = x-1
end if
end function

elemental real(dp) function a2modulo(x, y) result(r)
real(dp), intent(in) :: x, y
r = x-a1floor(x/y)*y
end function

elemental real(dp) function a3min(x, y) result(r)
real(dp), intent(in) :: x, y
if (x < y) then
    r = x
else
    r = y
end if
end function

elemental real(dp) function a4max(x, y) result(r)
real(dp), intent(in) :: x, y
if (x > y) then
    r = x
else
    r = y
end if
end function

elemental real(dp) function a6dsin(x) result(r)
real(dp), intent(in) :: x
real(dp) :: y
integer :: n
y = a2modulo(x, 2*pi)
y = a3min(y, pi - y)
y = a4max(y, -pi - y)
y = a3min(y, pi - y)
r = a5kernel_dsin(y)
end function

! Accurate on [-20,20] to about 2e-16
! Accurate on [-2E5,2E5] to about 1e-11
elemental real(dp) function a5kernel_dsin(x) result(res)
real(dp), intent(in) :: x
real(dp) :: z
! P = fpminimax(sin(x),[|1,3,5,7,9,11,13,15,17|],[|1,DD...|],[-pi/2;pi/2], fixed, relative);
z = x*x
res = x * (1_dp + &
z * (-0.166666666666666655306505478010174291974839958868813_dp + &
z * (8.333333333333191210937766414395365475534605853899e-3_dp + &
z * (-1.9841269841208314628381230132739601778520061386051e-4_dp + &
z * (2.7557319211005925166576720607184742789307105593802e-6_dp + &
z * (-2.5052106862292485979982282505972057331217670026862e-8_dp + &
z * (1.6058939234229984752935062738122048262688892328934e-10_dp + &
z * (-7.6429803568078360419013042444017287459926239780175e-13_dp + &
z * 2.7210673684877685351017286840988936723802336895982e-15_dp))))))))
end function

elemental real(sp) function a7ssin(x) result(r)
real(sp), intent(in) :: x
real(sp) :: y
real(dp) :: tmp, x2
x2 = x
tmp = a6dsin(x2)
r = tmp
end function

end module
