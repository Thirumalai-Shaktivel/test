module a
implicit none

contains

integer function b(x)
integer, intent(in) :: x
integer y
y = x
print *, "b()"
b = c(6)
contains
    integer function c(z)
    integer, intent(in) :: z
    print *, z
    print *, y
    c = z
    end function c
end function b

end module

program nested_04
use a, only: b
implicit none
integer test
test = b(5)

end
