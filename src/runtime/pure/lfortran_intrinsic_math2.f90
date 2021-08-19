! Temporary module, a subset of lfortran_intrinsic_math that works
module lfortran_intrinsic_math2
use, intrinsic :: iso_fortran_env, only: sp => real32, dp => real64
implicit none

interface abs
    module procedure sabs, dabs, isabs, idabs !, csabs, cdabs
end interface

interface sign
    module procedure ssign, dsign, issign, idsign
end interface

interface exp
    module procedure sexp, dexp !, csexp, cdexp
end interface

interface log
    module procedure slog, dlog !, cslog, cdlog
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

elemental integer function a1floor(x) result(r)
real(dp), intent(in) :: x
if (x >= 0) then
    r = x
else
    r = x-1
end if
end function

elemental integer function a1sfloor(x) result(r)
real(sp), intent(in) :: x
if (x >= 0) then
    r = x
else
    r = x-1
end if
end function

! log --------------------------------------------------------------------------
! J3/18-007r1 16.9.118 (F2018)
! TODO: Handle complex
! TODO: Benchmark, and use a better approximation
! TODO: Fix for large positive values
! TODO: Make pure later, for elemental

real(sp) function slog(x) result(r)
real(sp), intent(in) :: x
real(sp) :: a, b
if (x<=1e-20_sp) then
   error stop "Below threshold"
end if
if (x<=1) then
   r = sk2log(x)
else if ((x>1) .and. (x<=2)) then
   r = sklog(x)
else
   ! From https://math.stackexchange.com/a/75398/506917
   a = 0.5 * (1+x)
   b = sqrt(x)
   do while (abs(a-b) <= tiny(0.0))
    a = (a + b)*0.5
    b = sqrt(a*b)
   end do
r = 2 * ((x-1)/(a+b))
end if
end function slog

real(dp) function dlog(x) result(r)
real(dp), intent(in) :: x
real(dp) :: a, b
if (x<=1e-20_dp) then
   error stop "Below threshold"
end if
if (x<=1) then
   r = dk2log(x)
else if ((x>1) .and. (x<=2)) then
   r = dklog(x)
else
   ! From https://math.stackexchange.com/a/75398/506917
   a = 0.5 * (1+x)
   b = sqrt(x)
   do while (abs(a-b) <= tiny(0.0))
    a = (a + b)*0.5
    b = sqrt(a*b)
   end do
r = 2 * ((x-1)/(a+b))
end if
end function dlog

real(dp) function dk2log(x) result(r)
! p=remez(log(x),12,[1e-20,1],default,1e-30,1e-30);
  real(dp), intent(in) :: x
  r = -44.6241805019019418886284037335713020737_dp + &
x * (3863.54067377322457882890109080339011761_dp + &
x * (-1.165700867590407357585069275982654323345e5_dp + &
x * (1.648256701134734739562680048235424185076e6_dp + &
x * (-1.30878533145293064094806074739229217909e7_dp + &
x * (6.43835208287059911154361948875487242565e7_dp + &
x * (-2.07271978815287223322518760593793108305e8_dp + &
x * (4.49060849708921868099742690777312605834e8_dp + &
x * (-6.5947855116652079353323140383123494016e8_dp + &
x * (6.47398609939110884636899766407673347824e8_dp + &
x * (-4.06766384833646667566069278407306328638e8_dp + &
x * (1.47886801367615131735410692815199504564e8_dp + &
x * (-2.36605206727602080616022383416332100042e7_dp))))))))))))
end function

real(sp) function sk2log(x) result(r)
! p=remez(log(x),12,[1e-20,1],default,1e-30,1e-30);
  real(sp), intent(in) :: x
  r = -44.635902078015333855_sp + &
x * (4294.813627967851624_sp + &
x * (-1.27295737841369601774e5_sp + &
x * (1.7649571878737459499e6_sp + &
x * (-1.382526254140659912e7_sp + &
x * (6.7372517571984996335e7_sp + &
x * (-2.153755495472285572e8_sp + &
x * (4.6401394579896055322e8_sp + &
x * (-6.782794287479116882e8_sp + &
x * (6.632358688389824827e8_sp + &
x * (-4.15314086480325569e8_sp + &
x * (1.5055838728634916506e8_sp + &
x * (-2.4028305222962831884e7_sp))))))))))))
end function

real(dp) function dklog(x) result(r)
  ! p=remez(log(x),12,[1,2],default,1e-30,1e-30);
  real(dp), intent(in) :: x
  r = -3.25289140173914202504698307666434606988_dp + &
x * (14.14509453264892141178187612834139991_dp + &
x * (-47.376013235901408059920736965787402451_dp + &
x * (133.21752028460297994968316515339923569_dp + &
x * (-297.604451589775528244303496588849520219_dp + &
x * (532.346246547842284087369343163786619816_dp + &
x * (-772.604443308758329898532352436237770824_dp + &
x * (919.89279264583773410916742072557906998_dp + &
x * (-905.715396084325276442519440762751986207_dp + &
x * (741.091317656321219625562208155466756458_dp + &
x * (-505.017288840578619268438397926571944247_dp + &
x * (286.43840983648298042117522872791996061_dp + &
x * (-134.750821763023655637064270436594793655_dp + &
x * (52.2236315953768540684814726120759228964_dp + &
x * (-16.494313139531541066479053346891786479_dp + &
x * (4.17687172429043318322938942387333344103_dp + &
x * (-0.827635076539055488141085786532566586193_dp + &
x * (0.123606771032212536293499037500301654607_dp + &
x * (-1.308435592222155657935521636304954215667e-2_dp + &
x * (8.74994929937514073811801065616538964265e-4_dp + &
x * (-2.7793270779212376012509501528009442969e-5_dp))))))))))))))))))))
end function

real(sp) function sklog(x) result(r)
  real(sp), intent(in) :: x
  r = -2.7583768557201352593_sp + x &
 * (8.478514044070050984_sp + x &
 * (-16.390332927188993577_sp + x &
 * (25.474828460628157993_sp + x &
 * (-29.915663903161725894_sp + x &
 * (26.513663516934948714_sp + x &
 * (-17.759339260607131876_sp + x &
 * (8.944803435906684667_sp + x &
 * (-3.337035420742565326_sp + x &
 * (0.8949790681237649383_sp + x &
 * (-0.16325706464506785083_sp + x &
 * (1.814451508707195218e-2_sp + x &
 * (-9.276086688770115036e-4_sp))))))))))))
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

elemental integer(sp) function issign(a, b) result(r)
integer(sp), intent(in) :: a, b
if (b > 0) then
    r = abs(a)
else if (b > 0) then
    r = -1*abs(a)
else
   ! TODO: Can be +ve zero or -ve zero
   r = abs(a)
end if
end function

elemental integer(dp) function idsign(a, b) result(r)
integer(dp), intent(in) :: a, b
if (b > 0) then
    r = abs(a)
else if (b > 0) then
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
else if (b > 0) then
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
else if (b > 0) then
    r = -1*abs(a)
else
   ! TODO: Can be +ve zero or -ve zero
   r = abs(a)
end if
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
y=a1sfloor(x)
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
y=a1floor(x)
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
