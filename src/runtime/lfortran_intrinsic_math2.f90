! Temporary module, a subset of lfortran_intrinsic_math that works
module lfortran_intrinsic_math2
use, intrinsic :: iso_fortran_env, only: sp => real32, dp => real64
implicit none

interface abs
    module procedure sabs, dabs
end interface

!interface sqrt
!    module procedure ssqrt, dsqrt
!end interface

interface sin ! TODO: implement complex (single, double)
    module procedure ssin !, dsin
end interface

contains

! abs --------------------------------------------------------------------------

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

! sqrt -------------------------------------------------------------------------

!elemental real(sp) function ssqrt(x) result(r)
!real(sp), intent(in) :: x
!if (x >= 0) then
!    r = x**(1._sp/2)
!else
!    error stop "sqrt(x) for x < 0 is not allowed"
!end if
!end function

!elemental real(dp) function dsqrt(x) result(r)
!real(dp), intent(in) :: x
!if (x >= 0) then
!    r = x**(1._dp/2)
!else
!    error stop "sqrt(x) for x < 0 is not allowed"
!end if
!end function

! sin --------------------------------------------------------------------------
! Our implementation here is based off of the C files from the Sun compiler
!
! Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
!
! Developed at SunSoft, a Sun Microsystems, Inc. business.
! Permission to use, copy, modify, and distribute this
! software is freely granted, provided that this notice
! is preserved.


!  __kernel_sin( x, y, iy)
!  kernel sin function on [-pi/4, pi/4], pi/4 ~ 0.7854
!  Input x is assumed to be bounded by ~pi/4 in magnitude.
!  Input y is the tail of x.
!  Input iy indicates whether y is 0. (if iy=0, y assume to be 0).
!
!  Algorithm
! 	1. Since sin(-x) = -sin(x), we need only to consider positive x.
! 	2. if x < 2^-27 (hx<0x3e400000 0), return x with inexact if x!=0.
! 	3. sin(x) is approximated by a polynomial of degree 13 on
! 	   [0,pi/4]
! 		  	         3            13
! 	   	sin(x) ~ x + S1*x + ... + S6*x
! 	   where
!
!  	|sin(x)         2     4     6     8     10     12  |     -58
!  	|----- - (1+S1*x +S2*x +S3*x +S4*x +S5*x  +S6*x   )| <= 2
!  	|  x 					           |
!
! 	4. sin(x+y) = sin(x) + sin'(x')*y
! 		    ~ sin(x) + (1-x*x/2)*y
! 	   For better accuracy, let
! 		     3      2      2      2      2
! 		r = x! (S2+x! (S3+x! (S4+x! (S5+x! S6))))
! 	   then                   3    2
! 		sin(x) = x + (S1*x + (x! (r-y/2)+y))
!

elemental real(sp) function ssin(x) result(r)
real(sp), intent(in) :: x, y
integer(sp), intent(in) :: ix, iy
! Constants
real(sp), parameter :: half = 5.00000000000000000000e-01 ! 0x3FE00000, 0x00000000 */
real(sp), parameter :: S1 = -1.66666666666666324348e-01  ! 0xBFC55555, 0x55555549 */
real(sp), parameter :: S2  =  8.33333333332248946124e-03  ! 0x3F811111, 0x1110F8A6 */
real(sp), parameter :: S3  = -1.98412698298579493134e-04  ! 0xBF2A01A0, 0x19C161D5 */
real(sp), parameter :: S4  =  2.75573137070700676789e-06  ! 0x3EC71DE3, 0x57B1FE7D */
real(sp), parameter :: S5  = -2.50507602534068634195e-08  ! 0xBE5AE5E6, 0x8A2B9CEB */
real(sp), parameter :: S6  =  1.58969099521155010221e-10  ! 0x3DE5D93A, 0x5ACFD57C */
real(sp) :: tmp, z, v

!  double z,r,v;
!  int ix;
!  ix = __HI(x)&0x7fffffff;	/* high word of x */
!  if(ix<0x3e400000)			/* |x| < 2**-27 */
!     {if((int)x==0) return x;}		/* generate inexact */
!  z	=  x*x;
!  v	=  z*x;
!  r	=  S2+z*(S3+z*(S4+z*(S5+z*S6)));
!  if(iy==0) return x+v*(S1+z*r);
!  else      return x-((z*(half*y-v*r)-y)-v*S1);

if (mod(x) < tiny(0.0)) then
   if (int(iy) == 0) then
    r = x
    return
   end if
end if

if (y == 0 ) then
   iy = 0
end if

z =  x*x;
v =  z*x;
tmp =  S2+z*(S3+z*(S4+z*(S5+z*S6)));

if ( iy == 0 ) then
 r = x + v*(S1+z*tmp)
 return
else
 r = x-((z*(half*y-v*tmp)-y)-v*S1)
 return
end if

end function

! elemental real(dp) function dsin(x) result(r)
! real(dp), intent(in) :: x
! interface
!     pure real(c_double) function c_dsin(x) bind(c, name="_lfortran_dsin")
!     import :: c_double
!     real(c_double), intent(in), value :: x
!     end function
! end interface
! r = c_dsin(x)
! end function

end module
