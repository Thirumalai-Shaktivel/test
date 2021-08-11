module lfortran_intrinsic_array
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
   module procedure iint, rint, iintk, rintk, iinta, rinta
end interface

contains

! int --------------------------------------------------------------------------

elemental integer function iint(x)
integer, intent(in) :: x
end function

elemental integer function rint(x)
real, intent(in) :: x
end function

elemental integer function iinta(x)
integer, intent(in) :: x(:)
end function

elemental integer function rinta(x)
real, intent(in) :: x(:)
end function

elemental integer function iintk(x, kind)
integer, intent(in) :: x(:)
integer, intent(in) :: kind
end function

elemental integer function rintk(x, kind)
real, intent(in) :: x(:)
integer, intent(in) :: kind
end function

end module
