module second_layer
implicit none

contains

    real(4) function signr32(x, y) result(r)
        real(4), intent(in) :: x, y
        r = x
        if ((x >= 0.0 .and. y >= 0.0) .or. (x <= 0.0 .and. y <= 0.0)) then
            r = x
        else
            r = -x
        end if
    end function
end module

module first_layer
use second_layer
implicit none

contains

    real function f_real(a) result(b)
        real, intent(in) :: a
        b = a + signr32(1.0, a)
    end function

end module

program functions_01
use first_layer
implicit none

    real :: p = 5, q

    q = f_real(p)
    print *, q

end program
