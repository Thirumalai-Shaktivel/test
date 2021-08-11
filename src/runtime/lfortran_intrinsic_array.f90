module lfortran_intrinsic_array
use, intrinsic :: iso_fortran_env, only: sp => real32, dp => real64
implicit none

interface
    integer function size(x)
    integer, intent(in) :: x(:)
    end function

    integer function lbound(x, dim)
    integer, intent(in) :: x(:)
    integer, intent(in) :: dim
    end function

    integer function ubound(x, dim)
    integer, intent(in) :: x(:)
    integer, intent(in) :: dim
    end function

    integer function max(a, b)
    integer, intent(in) :: a, b
    end function

    integer function min(a, b)
    integer, intent(in) :: a, b
    end function

    logical function allocated(x)
    integer, intent(in) :: x(:)
    end function

    integer function minval(x)
    integer, intent(in) :: x(:)
    end function

    integer function maxval(x)
    integer, intent(in) :: x(:)
    end function

    integer function sum(x)
    integer, intent(in) :: x(:)
    end function

    integer function abs(x)
    integer, intent(in) :: x(:)
    end function

    real function tiny(x)
    integer, intent(in) :: x(:)
    end function

    real function real(x, kind)
    integer, intent(in) :: x(:)
    integer, intent(in) :: kind
    end function
end interface

interface int
   ! TODO: Handle arrays and more precision
   module procedure iint, rint, iintk, rintk
end interface

contains

! int --------------------------------------------------------------------------

elemental integer(sp) function iint(x)
integer, intent(in) :: x
end function

elemental integer(sp) function rint(x)
real, intent(in) :: x
end function

elemental integer(sp) function iintk(x, kind)
integer, intent(in) :: x(:)
integer, intent(in) :: kind
end function

elemental integer(sp) function rintk(x, kind)
real, intent(in) :: x(:)
integer, intent(in) :: kind
end function

end module
