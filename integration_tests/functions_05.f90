program functions_01
implicit none

    integer :: x = 5, y
    real :: a, b, c
    y = f(x)
    print *, y

    a = 20.0
    b = -30.0
    c = signr32(a, b)
    print *, c


contains

    integer function f(a) result(b)
    integer, intent(in) :: a
    b = a + 1
    end function

    real(4) function signr32(x, y) result(r)
    real(4), intent(in) :: x, y
    if ((x >= 0.0 .and. y >= 0.0) .or. (x <= 0.0 .and. y <= 0.0)) then
        r = x
    else
        r = -x
    end if
    end function

end program
