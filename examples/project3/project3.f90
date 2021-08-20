program project1
use energies, only: esample
use types, only: dp
implicit none

real(dp), parameter :: c=137.036
integer, parameter :: n=3, l=2, Z=2
print *, esample(c, n, l, Z)

end
