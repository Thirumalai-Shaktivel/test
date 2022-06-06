program bindc2
use iso_c_binding, only: c_loc, c_ptr, c_f_pointer
type(c_ptr) :: queries
integer :: idx = 1
integer(2), target :: xv(3, 4)
integer :: newshape(2)
integer(2), pointer :: x(:, :)

newshape(1) = 2
newshape(2) = 3

x => xv

call c_f_pointer(queries, x, newshape)

print *, c_loc(x), queries
end program
