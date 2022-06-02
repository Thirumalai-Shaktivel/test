program bindc2
use iso_c_binding, only: c_loc, c_ptr, c_f_pointer
type(c_ptr) :: queries
integer(2), pointer :: x(:)
call c_f_pointer(queries, x, [2])
print *, c_loc(x(idx))
end program
