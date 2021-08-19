! Temporary module, a subset of lfortran_intrinsic_math that works
module lfortran_intrinsic_math2
use, intrinsic :: iso_fortran_env, only: sp => real32, dp => real64
use lfortran_intrinsic_trig, only: floor
implicit none

interface abs
    module procedure sabs, dabs, isabs, idabs !, csabs, cdabs
end interface

interface exp
    module procedure sexp, dexp !, csexp, cdexp
end interface

interface sqrt
    module procedure ssqrt, dsqrt
end interface

contains

! exp --------------------------------------------------------------------------
! J3/18-007r1 16.9.74 (F2018)
! TODO: Handle complex
! TODO: Benchmark, and use a better approximation
! TODO: Make pure later, for elemental
real(sp) function sexp(x) result(r)
real(sp), intent(in) :: x
integer(sp) :: y=0
real(sp), parameter :: e=2.71828182845904523536028747135266249775724709369995
integer(sp) :: i=1
real(sp) :: temp=1.0, rem=0.0
y=floor(x)
do while (i<=y)
   temp=temp*e
end do
rem = x-y
r = temp + skexp(rem)
end function sexp

real(dp) function dexp(x) result(r)
real(dp), intent(in) :: x
real(dp), parameter :: e=2.71828182845904523536028747135266249775724709369995
integer(dp) :: y=0
integer(dp) :: i=1
real(dp) :: temp=1.0, rem=0.0
y=floor(x)
do while (i<=y)
   temp=temp*e
end do
rem = x-y
r = temp + dkexp(rem)
end function

real(dp) function dkexp(x) result(r)
  real(dp), intent(in) :: x
  r = 1.0000000000000000000034582432054056273353296135574_dp &
 + x * (0.99999999999999999924559786654160294200623694570698_dp &
 + x * (0.49999999999999999983420154396103734278674748395407_dp &
 + x * (0.16666666666666670302045786484552719313692347717579_dp &
 + x * (4.1666666666666671786860731304039555632799294507799e-2_dp &
 + x * (8.333333333332827751026615296950556555874361818821e-3_dp &
 + x * (1.38888888888883131894141507187899956557780595222563e-3_dp &
 + x * (1.9841269841585751656814667579836871493294981607125e-4_dp &
 + x * (2.48015873018922338526332134715462231032066978345857e-5_dp &
 + x * (2.7557319119172411016434869111479000103174458500596e-6_dp &
 + x * (2.7557319136118985564265647722482011897465316598629e-7_dp &
 + x * (2.505212812896778092196024189539079344388688742221e-8_dp &
 + x * (2.0876771623565916543065882994922397414857094086398e-9_dp &
 + x * (1.6056925188340229912525575841878966502481747836397e-10_dp &
 + x * (1.14693360758440893282841272319525387771928638883733e-11_dp &
 + x * (7.7677121552139548935793797109218678270257783287917e-13_dp &
 + x * 4.8523678299667709878959513660452780479315189934426e-14_dp)))))))))))))))
end function

real(sp) function skexp(x) result(r)
  real(sp), intent(in) :: x
  r = 1.0000000000000000000034582432054056273353296135574_sp &
 + x * (0.99999999999999999924559786654160294200623694570698_sp &
 + x * (0.49999999999999999983420154396103734278674748395407_sp &
 + x * (0.16666666666666670302045786484552719313692347717579_sp &
 + x * (4.1666666666666671786860731304039555632799294507799e-2_sp &
 + x * (8.333333333332827751026615296950556555874361818821e-3_sp &
 + x * (1.38888888888883131894141507187899956557780595222563e-3_sp &
 + x * (1.9841269841585751656814667579836871493294981607125e-4_sp &
 + x * (2.48015873018922338526332134715462231032066978345857e-5_sp &
 + x * (2.7557319119172411016434869111479000103174458500596e-6_sp &
 + x * (2.7557319136118985564265647722482011897465316598629e-7_sp &
 + x * (2.505212812896778092196024189539079344388688742221e-8_sp &
 + x * (2.0876771623565916543065882994922397414857094086398e-9_sp &
 + x * (1.6056925188340229912525575841878966502481747836397e-10_sp &
 + x * (1.14693360758440893282841272319525387771928638883733e-11_sp &
 + x * (7.7677121552139548935793797109218678270257783287917e-13_sp &
 + x * 4.8523678299667709878959513660452780479315189934426e-14_sp)))))))))))))))
end function
! abs --------------------------------------------------------------------------
! J3/18-007r1 16.9.2 (F2018)
! TODO: Handle complex

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


end module
