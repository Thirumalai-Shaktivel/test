module lfortran_intrinsic_misc
use, intrinsic :: iso_fortran_env, only: sp => real32, dp => real64
use, intrinsic :: iso_c_binding, only: c_float, c_double, c_int
implicit none

! Not supported yet
interface tiny
   ! TODO: tiny takes arrays too
   ! s --> single precision real
   ! d --> double precision real
    module procedure stiny, dtiny
end interface

contains

! scalars --------------------------------------------------------------------------

elemental real(sp) function stiny(x) result(r)
real(sp), intent(in) :: x
interface
    pure real(c_float) function c_stiny(x) bind(c, name="_lfortran_stiny")
    import :: c_float
    real(c_float), intent(in), value :: x
    end function
end interface
r = c_stiny(x)
end function

elemental real(dp) function dtiny(x) result(r)
real(dp), intent(in) :: x
interface
    pure real(c_double) function c_dtiny(x) bind(c, name="_lfortran_dtiny")
    import :: c_double
    real(c_double), intent(in), value :: x
    end function
end interface
r = c_dtiny(x)
end function

elemental complex(sp) function ctiny(x) result(r)
complex(sp), intent(in) :: x
interface
    pure complex(c_float) function c_ctiny(x) bind(c, name="_lfortran_ctiny")
    import :: c_float
    complex(c_float), intent(in), value :: x
    end function
end interface
r = c_ctiny(x)
end function

end module
