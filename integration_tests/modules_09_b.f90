module modules_09_b
implicit none
private
public b, i

integer, parameter :: i = 5

integer, parameter :: i4 = selected_int_kind(6)

contains

integer function b()
b = i4
end function

end module
