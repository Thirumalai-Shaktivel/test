program functions_01
implicit none

    integer :: x = 5, y
    y = f(x)
    print *, y

contains

    integer function f(a) result(b)
    integer, intent(in) :: a
    b = a + 1
    end function

end program
