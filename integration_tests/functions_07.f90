module first_layer
implicit none

contains

    real function f_real(u) result(v)
        real, intent(in) :: u
        v = u + 1.0
    end function

end module

module third_layer
use first_layer
implicit none

contains

    real function f_real_extra(a) result(b)
        real, intent(in) :: a
        b = f_real(a)
    end function

end module

program functions_01
use third_layer
implicit none

    real :: p = 5, q

    q = f_real_extra(p)
    print *, q

end program