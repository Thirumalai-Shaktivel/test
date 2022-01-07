module test_float_c

use iso_c_binding, only: c_float

contains

real(c_float) function f(x) result(r) bind(c)
real(c_float), intent(in), value :: x
r = 5
end function

end module
