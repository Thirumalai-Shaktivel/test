module lfortran_intrinsic_math
use, intrinsic :: iso_fortran_env, only: sp => real32, dp => real64
use, intrinsic :: iso_c_binding, only: c_float, c_double
implicit none

interface tan
    module procedure stan, dtan, ctan, ztan
end interface

contains

! tan --------------------------------------------------------------------------

elemental real(sp) function stan(x) result(r)
real(sp), intent(in) :: x
interface
    pure real(c_float) function c_stan(x) bind(c, name="_lfortran_stan")
    import :: c_float
    real(c_float), intent(in), value :: x
    end function
end interface
r = c_stan(x)
end function

elemental real(dp) function dtan(x) result(r)
real(dp), intent(in) :: x
interface
    pure real(c_double) function c_dtan(x) bind(c, name="_lfortran_dtan")
    import :: c_double
    real(c_double), intent(in), value :: x
    end function
end interface
r = c_dtan(x)
end function

elemental complex(sp) function ctan(x) result(r)
complex(sp), intent(in) :: x
interface
    pure complex(c_float) function c_ctan(x) bind(c, name="_lfortran_ctan")
    import :: c_float
    complex(c_float), intent(in), value :: x
    end function
end interface
r = c_ctan(x)
end function

elemental complex(dp) function ztan(x) result(r)
complex(dp), intent(in) :: x
interface
    pure subroutine c_ztan(x) bind(c, name="_lfortran_ztan")
    import :: c_double
    complex(c_double), intent(in), value :: x
    end subroutine

    pure complex(c_double) function c_ctan2(x) bind(c, name="_lfortran_ztan2")
    import :: c_double
    complex(c_double), intent(in), value :: x
    end function
end interface
call c_ztan(x)
r = c_ctan2(x)
end function


end module
