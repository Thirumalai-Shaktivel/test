module energies

use types, only: dp
use constants, only: pi
implicit none
private
public esample

contains

real(dp) function esample(const, n, l, Z)
real(dp), intent(in) :: const
integer, intent(in) :: n, l, Z

real(dp) :: beta
beta = sqrt(l**2 - (Z/const)**2)
esample = const**2/sqrt(1 + (Z/const)**2/(n - abs(l) + beta)**2) - const**2
end function

end module
