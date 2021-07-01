program modules_14
use modules_14_a, only: a
implicit none

integer :: i

i = 5
call a(i)
if (i /= 6) error stop

end program
