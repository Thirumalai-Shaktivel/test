module lfortran_intrinsic_misc
use, intrinsic :: iso_fortran_env, only: sp => real32, dp => real64
use, intrinsic :: iso_c_binding, only: c_float, c_double, c_int
implicit none

! Not supported yet
interface tiny
   ! TODO: tiny takes arrays too
   ! i --> integer
   ! s --> single precision real
   ! d --> double precision real
   ! c --> simple precision complex
   ! z --> double precision complex
    module procedure stiny, dtiny, ctiny, ztiny !, itiny
end interface

contains

! scalars --------------------------------------------------------------------------

! TODO: i is complicated since it causes an ambiguous interface in this simplistic approach
! elemental real(sp) function itiny(x) result(r)
! real(sp), intent(in) :: x
! interface
!     pure integer(c_int) function c_itiny(x) bind(c, name="_lfortran_itiny")
!     import :: c_int
!     integer(c_int), intent(in), value :: x
!     end function
! end interface
! r = c_itiny(x)
! end function

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

elemental complex(dp) function ztiny(x) result(r)
complex(dp), intent(in) :: x
interface
    pure complex(c_double) function c_ztiny(x) bind(c, name="_lfortran_ztiny")
    import :: c_double
    complex(c_double), intent(in), value :: x
    end function
end interface
r = c_ztiny(x)
end function

end module
