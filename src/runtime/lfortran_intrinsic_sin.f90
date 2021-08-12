module lfortran_intrinsic_sin
use, intrinsic :: iso_fortran_env, only: sp => real32, dp => real64
implicit none

interface sin
    module procedure dsin
end interface

contains

! sin --------------------------------------------------------------------------
! Our implementation here is designed around range reduction to [-pi/2, pi/2]
! Subsequently, we fit a 64 bit precision polynomials via Sollya (https://www.sollya.org/)
! -- Chebyshev (32 terms) --
! This has a theoretical approximation error bound of [-7.9489615048122632526e-41;7.9489615048122632526e-41]
! Due to rounding errors; we obtain a maximum error (w.r.t. gfortran) of ~ E-15 over [-10, 10]
! -- Remez (16 terms) -- [DEFAULT] (fewer terms)
! Due to rounding errors; we obtain a maximum error (w.r.t. gfortran) of ~ E-16 over [-10, 10]
! For large values, e.g. 2E10 we have an absolute error of E-7
! For huge(0) we have an absolute error of E-008
! TODO: Deal with very large numbers; the errors get worse above 2E10
! For huge(0.0) we have 3.4028234663852886E+038 -0.52187652333365853       0.99999251142364332        1.5218690347573018
!                          value                    gfortran sin             lfortran sin              absolute error

elemental real(dp) function dsin(x) result(r)
real(dp), parameter :: pi = 3.1415926535897932384626433832795_dp
real(dp), intent(in) :: x
real(dp) :: y
integer :: n
if (abs(x) < pi/2) then
    r = kernel_dsin(x)
else ! fold into [-pi/2, pi/2]
    y = modulo(x, 2*pi)
    y = min(y, pi - y)
    y = max(y, -pi - y)
    y = min(y, pi - y)
    r = kernel_dsin(y)
end if
end function

elemental real(dp) function kernel_dsin(x) result(res)
  real(dp), intent(in) :: x
  ! Remez16
  res = 2.0612613395817811826e-18_dp + x * (0.9999999999999990771_dp + x * (-1.1326311799469310948e-16_dp + x * (-0.16666666666664811048_dp + x * (4.0441204087065883493e-16_dp + x * (8.333333333226519387e-3_dp + x * (-5.1082355103624979855e-16_dp + x * (-1.9841269813888534497e-4_dp + x * (3.431131630096384069e-16_dp + x * (2.7557315514280769795e-6_dp + x * (-1.6713014856642339287e-16_dp + x * (-2.5051823583393710429e-8_dp + x * (6.6095338377356955055e-17_dp + x * (1.6046585911173017112e-10_dp + x * (-1.6627129557672300738e-17_dp + x * (-7.3572396558796051923e-13_dp + x * 1.7462917763807982697e-18)))))))))))))))
  ! Chebyshev32
  ! res = 2.1355496357691975886e-20_dp + x * (1.0000000000000000063_dp + x * (-9.004255532791894035e-18_dp + x * (-0.16666666666666708032_dp + x * (3.9332705471670443079e-16_dp + x * (8.333333333341355733e-3_dp + x * (-6.9028665221660691874e-15_dp + x * (-1.9841269848665583425e-4_dp + x * (6.1976058203375757744e-14_dp + x * (2.7557323147213076849e-6_dp + x * (-3.2138655682600360264e-13_dp + x * (-2.5053432626865828714e-8_dp + x * (1.0468987330682077985e-12_dp + x * (1.6360972193468943658e-10_dp + x * (-2.2731120913506430714e-12_dp + x * (-5.5906235858164260444e-12_dp + x * (3.425556604867707744e-12_dp + x * (5.532635689918605909e-12_dp + x * (-3.6764276518941318983e-12_dp + x * (-4.5930955707654957942e-12_dp + x * (2.8488303025191868967e-12_dp + x * (2.7673107200394155003e-12_dp + x * (-1.59803409654352135405e-12_dp + x * (-1.1963665142242100106e-12_dp + x * (6.4277391325110941334e-13_dp + x * (3.6143518894425348369e-13_dp + x * (-1.8071233122115074043e-13_dp + x * (-7.2405073651171420086e-14_dp + x * (3.3717669642454825042e-14_dp + x * (8.63748093603347239e-15_dp + x * (-3.751088768577678467e-15_dp + x * (-4.642806182921299547e-16_dp + x * 1.8832379977836398444e-16)))))))))))))))))))))))))))))))
end function

elemental real(dp) function kernel_dcos(x, y) result(res)
real(dp), intent(in) :: x, y
error stop "cos not implemented"
end function


end module
