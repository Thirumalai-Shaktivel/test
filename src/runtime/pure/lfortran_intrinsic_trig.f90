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
! P = fpminimax(sin(x),[|1,3,5,7,9,11,13,15,17,19,21|],[|1,D...|],[-pi/2;pi/2], fixed, relative);
res = x * (1_dp + &
x**2 * (-0.166666666666666407614627587463473901152610778808594_dp + &
x**2 * (8.3333333333313053259416847140528261661529541015625e-3_dp + &
x**2 * (-1.9841269840792374878901682677678763866424560546875e-4_dp + &
x**2 * (2.75573191999001210206188261508941650390625e-6_dp + &
x**2 * (-2.505211593728517982526682317256927490234375e-8_dp + &
x**2 * (1.606055288760899202316068112850189208984375e-10_dp + &
x**2 * (-7.7715611723760957829654216766357421875e-13_dp + &
x**2 * (8.2156503822261583991348743438720703125e-15_dp + &
x**2 * (-1.22124532708767219446599483489990234375e-15_dp + &
x**2 * 1.1102230246251565404236316680908203125e-16_dp))))))))))
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
