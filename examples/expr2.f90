program expr2
use iso_fortran_env, only: dp=>real32
implicit none

complex(dp) :: x

x = (2.5_dp, 3.5_dp)
x = tan(x)

end program
