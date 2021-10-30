program intrinsics_23
implicit none
integer, parameter :: x = huge(0)

if( x /= 2147483641 ) error stop

end program
