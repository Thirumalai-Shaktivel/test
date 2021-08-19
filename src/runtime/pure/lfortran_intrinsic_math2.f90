! Temporary module, a subset of lfortran_intrinsic_math that works
module lfortran_intrinsic_math2
use, intrinsic :: iso_fortran_env, only: sp => real32, dp => real64
implicit none

interface abs
    module procedure sabs, dabs, isabs, idabs
end interface

interface sqrt
    module procedure ssqrt, dsqrt
end interface

interface aimag
    module procedure saimag, daimag
end interface

interface floor
    module procedure sfloor, dfloor
end interface

interface modulo
    module procedure imodulo, smodulo, dmodulo
end interface

contains

! abs --------------------------------------------------------------------------
! J3/18-007r1 16.9.2 (F2018)
! TODO: Handle complex

elemental integer function iabs(x) result(r)
integer, intent(in) :: x
if (x >= 0) then
    r = x
else
    r = -x
end if
end function

elemental real(sp) function sabs(x) result(r)
real(sp), intent(in) :: x
if (x >= 0) then
    r = x
else
    r = -x
end if
end function

elemental real(dp) function dabs(x) result(r)
real(dp), intent(in) :: x
if (x >= 0) then
    r = x
else
    r = -x
end if
end function

elemental integer(sp) function isabs(x) result(r)
integer(sp), intent(in) :: x
if (x >= 0) then
    r = x
else
    r = -x
end if
end function

elemental integer(dp) function idabs(x) result(r)
integer(dp), intent(in) :: x
if (x >= 0) then
    r = x
else
    r = -x
end if
end function

! sqrt -------------------------------------------------------------------------

elemental real(sp) function ssqrt(x) result(r)
real(sp), intent(in) :: x
if (x >= 0) then
    r = x**(1._sp/2)
else
    error stop "sqrt(x) for x < 0 is not allowed"
end if
end function

elemental real(dp) function dsqrt(x) result(r)
real(dp), intent(in) :: x
if (x >= 0) then
    r = x**(1._dp/2)
else
    error stop "sqrt(x) for x < 0 is not allowed"
end if
end function

! aimag ------------------------------------------------------------------------

elemental real(sp) function saimag(x) result(r)
complex(sp), intent(in) :: x
! Uncomment once it is implemented
!r = x%im
error stop "aimag not implemented yet"
end function

elemental real(dp) function daimag(x) result(r)
complex(dp), intent(in) :: x
! Uncomment once it is implemented
!r = x%im
error stop "aimag not implemented yet"
end function

! floor ------------------------------------------------------------------------

elemental integer function sfloor(x) result(r)
real(sp), intent(in) :: x
if (x >= 0) then
    r = x
else
    r = x-1
end if
end function

elemental integer function dfloor(x) result(r)
real(dp), intent(in) :: x
if (x >= 0) then
    r = x
else
    r = x-1
end if
end function

! modulo -----------------------------------------------------------------------

elemental integer function imodulo(x, y) result(r)
integer, intent(in) :: x, y
r = x-floor(real(x)/y)*y
end function

elemental real(sp) function smodulo(x, y) result(r)
real(sp), intent(in) :: x, y
r = x-floor(x/y)*y
end function

elemental real(dp) function dmodulo(x, y) result(r)
real(dp), intent(in) :: x, y
r = x-floor(x/y)*y
end function

end module
