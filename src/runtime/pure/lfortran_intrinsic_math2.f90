! Temporary module, a subset of lfortran_intrinsic_math that works
module lfortran_intrinsic_math2
use, intrinsic :: iso_fortran_env, only: i8 => int8, i16 => int16, i32 => int32, i64 => int64, sp => real32, dp => real64
implicit none

interface abs
    module procedure iabs, sabs, dabs, cabs, zabs
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

interface ceiling
    module procedure sceiling, dceiling
end interface

interface nint
    module procedure snint, dnint
end interface

interface modulo
    module procedure imodulo, smodulo, dmodulo
end interface

interface mod
    module procedure imod, smod, dmod
end interface

interface merge
    module procedure mergei8, mergei16, mergei32, mergei64, merger32, merger64, mergechar, mergelogical
end interface

interface min
    module procedure imin, imin8, imin16, imin64, smin, dmin, imin_6args
end interface

interface max
    module procedure imax, imax8, imax16, imax64, smax, dmax, imax_6args
end interface

interface huge
    module procedure i32huge, sphuge, dphuge
end interface

interface matmul
    module procedure matmulr32r32, matmulr64r64, matmulz32z32, matmulz64z64
end interface

interface transpose
    module procedure transposer32, transposer64, transposez32, transposez64
end interface

interface conjg
    module procedure conjgz32, conjgz64
end interface

interface dot_product
    module procedure dotproductr32r32, dotproductr64r64
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

elemental real(sp) function cabs(x) result(r)
complex(sp), intent(in) :: x
r = 1.0
end function

elemental real(dp) function zabs(x) result(r)
complex(dp), intent(in) :: x
r = 1.0
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
r = 3
! Uncomment once it is implemented
!r = x%im
!error stop "aimag not implemented yet"
end function

elemental real(dp) function daimag(x) result(r)
complex(dp), intent(in) :: x
r = 3
! Uncomment once it is implemented
!r = x%im
!error stop "aimag not implemented yet"
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

! ceiling ------------------------------------------------------------------------

elemental integer function sceiling(x) result(r)
real(sp), intent(in) :: x
r = sfloor(x)
if (r - x /= 0.0) then
    r = r + 1
end if
end function

elemental integer function dceiling(x) result(r)
real(dp), intent(in) :: x
r = dfloor(x)
if (r - x /= 0.0) then
    r = r + 1
end if
end function

! nint ------------------------------------------------------------------------

elemental integer function snint(x) result(r)
real(sp), intent(in) :: x
if (x >= 0) then
    r = x+0.5_sp
else
    r = x-1+0.5_sp
end if
end function

elemental integer function dnint(x) result(r)
real(dp), intent(in) :: x
if (x >= 0) then
    r = x+0.5_dp
else
    r = x-1+0.5_dp
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

! mod --------------------------------------------------------------------------

elemental integer function imod(x, y) result(r)
integer, intent(in) :: x, y
r = x-floor(real(x)/y)*y
if (x < 0 .and. y < 0) return
if (x < 0) r = r - y
if (y < 0) r = r - y
end function

elemental real(sp) function smod(x, y) result(r)
real(sp), intent(in) :: x, y
r = x-floor(x/y)*y
if (x < 0 .and. y < 0) return
if (x < 0) r = r - y
if (y < 0) r = r - y
end function

elemental real(dp) function dmod(x, y) result(r)
real(dp), intent(in) :: x, y
r = x-floor(x/y)*y
if (x < 0 .and. y < 0) return
if (x < 0) r = r - y
if (y < 0) r = r - y
end function

! min --------------------------------------------------------------------------

elemental integer function imin(x, y) result(r)
integer, intent(in) :: x, y
if (x < y) then
    r = x
else
    r = y
end if
end function

elemental integer(i8) function imin8(x, y) result(r)
integer(i8), intent(in) :: x, y
if (x < y) then
    r = x
else
    r = y
end if
end function

elemental integer(i16) function imin16(x, y) result(r)
integer(i16), intent(in) :: x, y
if (x < y) then
    r = x
else
    r = y
end if
end function

elemental integer(i64) function imin64(x, y) result(r)
integer(i64), intent(in) :: x, y
if (x < y) then
    r = x
else
    r = y
end if
end function

elemental real(sp) function smin(x, y) result(r)
real(sp), intent(in) :: x, y
if (x < y) then
    r = x
else
    r = y
end if
end function

elemental real(dp) function dmin(x, y) result(r)
real(dp), intent(in) :: x, y
if (x < y) then
    r = x
else
    r = y
end if
end function

elemental integer function imin_6args(a, b, c, d, e, f) result(r)
integer, intent(in) :: a, b, c, d, e, f
integer :: args(6)
integer :: itr, curr_value
args(1) = a
args(2) = b
args(3) = c
args(4) = d
args(5) = e
args(6) = f
r = a
do itr = 1, 6
    curr_value = args(itr)
    r = min(curr_value, r)
end do
end function

! max --------------------------------------------------------------------------

elemental integer function imax(x, y) result(r)
integer, intent(in) :: x, y
if (x > y) then
    r = x
else
    r = y
end if
end function

elemental integer(i8) function imax8(x, y) result(r)
integer(i8), intent(in) :: x, y
if (x > y) then
    r = x
else
    r = y
end if
end function

elemental integer(i16) function imax16(x, y) result(r)
integer(i16), intent(in) :: x, y
if (x > y) then
    r = x
else
    r = y
end if
end function

elemental integer(i64) function imax64(x, y) result(r)
integer(i64), intent(in) :: x, y
if (x > y) then
    r = x
else
    r = y
end if
end function

elemental integer function imax_6args(a, b, c, d, e, f) result(r)
integer, intent(in) :: a, b, c, d, e, f
integer :: args(6)
integer :: itr, curr_value
args(1) = a
args(2) = b
args(3) = c
args(4) = d
args(5) = e
args(6) = f
r = a
do itr = 1, 6
    curr_value = args(itr)
    r = imax(curr_value, r)
end do
end function

elemental real(sp) function smax(x, y) result(r)
real(sp), intent(in) :: x, y
if (x > y) then
    r = x
else
    r = y
end if
end function

elemental real(dp) function dmax(x, y) result(r)
real(dp), intent(in) :: x, y
if (x > y) then
    r = x
else
    r = y
end if
end function

! huge -------------------------------------------------------------------------

elemental integer(i32) function i32huge(x) result(r)
integer(i32), intent(in) :: x
r = 2147483647
! r = 2**31 - 1
end function

elemental real(sp) function sphuge(x) result(r)
real(sp), intent(in) :: x
r = 3.40282347e38
! r = 2**128 * (1 - 2**-24)
end function

elemental real(dp) function dphuge(x) result(r)
real(dp), intent(in) :: x
r = 1.7976931348623157d308
! r = 2**1024 * (1 - 2**-53)
end function

function mergei8(tsource, fsource, mask) result(r)
    integer(i8), intent(in) :: tsource(:)
    integer(i8), intent(in) :: fsource(:)
    logical, intent(in) :: mask(:)
    integer(i8) :: r
end function

function mergei16(tsource, fsource, mask) result(r)
    integer(i16), intent(in) :: tsource(:)
    integer(i16), intent(in) :: fsource(:)
    logical, intent(in) :: mask(:)
    integer(i16) :: r
end function

function mergei32(tsource, fsource, mask) result(r)
    integer(i32), intent(in) :: tsource(:)
    integer(i32), intent(in) :: fsource(:)
    logical, intent(in) :: mask(:)
    integer(i32) :: r
end function

function mergei64(tsource, fsource, mask) result(r)
    integer(i64), intent(in) :: tsource(:)
    integer(i64), intent(in) :: fsource(:)
    logical, intent(in) :: mask(:)
    integer(i64) :: r
end function

function merger32(tsource, fsource, mask) result(r)
    real(sp), intent(in) :: tsource(:)
    real(sp), intent(in) :: fsource(:)
    logical, intent(in) :: mask(:)
    real(sp) :: r
end function

function merger64(tsource, fsource, mask) result(r)
    real(dp), intent(in) :: tsource(:)
    real(dp), intent(in) :: fsource(:)
    logical, intent(in) :: mask(:)
    real(dp) :: r
end function

function mergechar(tsource, fsource, mask) result(r)
    character(len=*), intent(in) :: tsource(:)
    character(len=*), intent(in) :: fsource(:)
    logical, intent(in) :: mask(:)
    character(len=len(tsource)) :: r
end function

function mergelogical(tsource, fsource, mask) result(r)
    logical, intent(in) :: tsource(:)
    logical, intent(in) :: fsource(:)
    logical, intent(in) :: mask(:)
    logical :: r
end function

function matmulr32r32(x, y) result(r)
real(sp) :: x(:), y(:)
real(sp) :: r(size(x))
end function

function matmulr64r64(x, y) result(r)
real(dp) :: x(:), y(:)
real(dp) :: r(size(x))
end function

function matmulz32z32(x, y) result(r)
complex(sp) :: x(:), y(:)
complex(sp) :: r(size(x))
end function

function matmulz64z64(x, y) result(r)
complex(dp) :: x(:), y(:)
complex(dp) :: r(size(x))
end function

function dotproductr32r32(x, y) result(r)
real(sp) :: x(:), y(:)
real(sp) :: r
end function

function dotproductr64r64(x, y) result(r)
real(dp) :: x(:), y(:)
real(dp) :: r
end function

function transposer32(x) result(r)
real(sp) :: x(:, :)
real(sp) :: r(size(x) : size(x))
end function

function transposer64(x) result(r)
real(dp) :: x(:, :)
real(dp) :: r(size(x) : size(x))
end function

function transposez32(x) result(r)
complex(sp) :: x(:, :)
complex(sp) :: r(size(x) : size(x))
end function

function transposez64(x) result(r)
complex(dp) :: x(:, :)
complex(dp) :: r(size(x) : size(x))
end function

function conjgz32(x) result(r)
complex(sp) :: x
complex(sp) :: r
end function

function conjgz64(x) result(r)
complex(dp) :: x
complex(dp) :: r
end function

end module
