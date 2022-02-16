program functions_01
implicit none

    integer :: x = 5, y
    y = f(x)
    print *, y

    real :: a, b, c
    c = signr32(a, b)
    print *, c


contains

    integer function f(a) result(b)
    integer, intent(in) :: a
    b = a + signr32(1.0_4, a)
    end function

    real(4) function signr32(x, y) result(r)
    real(4), intent(in) :: x, y
    if ((x >= 0 .and. y >= 0) .or. (x <= 0 .and. y <= 0)) then
        r = x
    else
        r = -x
    end if
    end function

end program
