! Temporary module, a subset of lfortran_intrinsic_math that works
module lfortran_intrinsic_math2
use, intrinsic :: iso_fortran_env, only: sp => real32, dp => real64
implicit none

interface abs
    module procedure iabs, sabs, dabs
end interface

interface aimag
    module procedure saimag, daimag
end interface

interface exp
    module procedure sexp, dexp !, csexp, cdexp
end interface

interface floor
    module procedure sfloor, dfloor
end interface

interface modulo
    module procedure imodulo, smodulo, dmodulo
end interface

interface sign
    module procedure ssign, dsign, isign
end interface

interface sqrt
    module procedure ssqrt, dsqrt
end interface

contains

! abs --------------------------------------------------------------------------

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

! exp --------------------------------------------------------------------------
! J3/18-007r1 16.9.74 (F2018)
! TODO: Handle complex
! TODO: Benchmark, and use a better approximation
! TODO: Fix for large positive values
! TODO: Make pure later, for elemental
real(sp) function sexp(x) result(r)
real(sp), intent(in) :: x
integer(sp) :: y=0
real(dp), parameter :: e=2.71828182845904523536028747135266249775724709369995_dp
integer(sp) :: i=1
real(sp) :: temp=1.0, rem=0.0
y=floor(x)
temp = e**y
rem = x-y
r = temp * skexp(rem)
end function sexp

real(dp) function dexp(x) result(r)
real(dp), intent(in) :: x
real(dp), parameter :: e=2.71828182845904523536028747135266249775724709369995
integer(dp) :: y=0
integer(dp) :: i=1
real(dp) :: temp=1.0, rem=0.0
y=floor(x)
temp = e**y
rem = x-y
r = temp * dkexp(rem)
end function

real(dp) function dkexp(x) result(r)
  real(dp), intent(in) :: x
  r = 1.0000000000000000000000005083394318904510148833396_dp &
+ x * (0.99999999999999999999969919958634717810033807895144_dp &
+ x * (0.50000000000000000002911346029801314716786548728254_dp &
+ x * (0.16666666666666666555784076109116721482457603908183_dp &
+ x * (4.1666666666666688844054402980928111423289510294421e-2_dp &
+ x * (8.333333333333064559181426776728513440574185957534e-3_dp &
+ x * (1.3888888888910347427359429229370074904739824124944e-3_dp &
+ x * (1.9841269840080026248840770408180621017308976014971e-4_dp &
+ x * (2.4801587349002024327103599745679667185644101313129e-5_dp &
+ x * (2.7557317836216813286078787618328205913393500544759e-6_dp &
+ x * (2.7557349416187529681115410053593103667752043931006e-7_dp &
+ x * (2.5051618471238628319842010445028003709897263525734e-8_dp &
+ x * (2.0882653409299033362960379288517836791127648059464e-9_dp &
+ x * (1.60072729696308450824970761046065826862221143627163e-10_dp &
+ x * (1.17915398470253482806191895081889250570302246272647e-11_dp &
+ x * (6.3302376876570951999166725931186385038150410171334e-13_dp &
+ x * 7.909030358916591735769808575928752895945283211931e-14_dp)))))))))))))))
end function

real(sp) function skexp(x) result(r)
  real(sp), intent(in) :: x
  r = 1.0000000000000000000000005083394318904510148833396_sp &
+ x * (0.99999999999999999999969919958634717810033807895144_sp &
+ x * (0.50000000000000000002911346029801314716786548728254_sp &
+ x * (0.16666666666666666555784076109116721482457603908183_sp &
+ x * (4.1666666666666688844054402980928111423289510294421e-2_sp &
+ x * (8.333333333333064559181426776728513440574185957534e-3_sp &
+ x * (1.3888888888910347427359429229370074904739824124944e-3_sp &
+ x * (1.9841269840080026248840770408180621017308976014971e-4_sp &
+ x * (2.4801587349002024327103599745679667185644101313129e-5_sp &
+ x * (2.7557317836216813286078787618328205913393500544759e-6_sp &
+ x * (2.7557349416187529681115410053593103667752043931006e-7_sp &
+ x * (2.5051618471238628319842010445028003709897263525734e-8_sp &
+ x * (2.0882653409299033362960379288517836791127648059464e-9_sp &
+ x * (1.60072729696308450824970761046065826862221143627163e-10_sp &
+ x * (1.17915398470253482806191895081889250570302246272647e-11_sp &
+ x * (6.3302376876570951999166725931186385038150410171334e-13_sp &
+ x * 7.909030358916591735769808575928752895945283211931e-14_sp)))))))))))))))
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

! sign --------------------------------------------------------------------------
! J3/18-007r1 16.9.176 (F2018)
! TODO: Benchmark
! TODO: Handle positive and negative zeros

! 5 Result Value
! Case (i): If B > 0, the value of the result is |A|
! Case (ii): If B < 0, the value of the result is -|A|
! Case (iii): If B is of type integer and B=0, the value of the result is |A|
! Case (iv): If B is of type real and is zero, then:
! • if the processor does not distinguish between positive and negative real zero, or if B is positive
! real zero, the value of the result is |A|
! • if the processor distinguishes between positive and negative real zero, and B is negative real
! zero, the value of the result is -|A|
! 6 Example. SIGN (−3.0, 2.0) has the value 3.0

elemental integer function isign(a, b) result(r)
integer, intent(in) :: a, b
if (b > 0) then
    r = abs(a)
else if (b < 0) then
    r = -1*abs(a)
else
   ! TODO: Can be +ve zero or -ve zero
   r = abs(a)
end if
end function

elemental real(sp) function ssign(a, b) result(r)
real(sp), intent(in) :: a, b
if (b > 0) then
    r = abs(a)
else if (b < 0) then
    r = -1*abs(a)
else
   ! TODO: Can be +ve zero or -ve zero
   r = abs(a)
end if
end function

elemental real(dp) function dsign(a, b) result(r)
real(dp), intent(in) :: a, b
if (b > 0) then
    r = abs(a)
else if (b < 0) then
    r = -1*abs(a)
else
   ! TODO: Can be +ve zero or -ve zero
   r = abs(a)
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

end module
