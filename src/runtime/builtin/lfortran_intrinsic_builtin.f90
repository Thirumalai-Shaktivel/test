module lfortran_intrinsic_builtin
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

    real function tiny(x)
    integer, intent(in) :: x(:)
    end function

    real function real(x, kind)
    integer, intent(in) :: x(:)
    integer, intent(in) :: kind
    end function

    integer function int(x, kind)
    real, intent(in) :: x(:)
    integer, intent(in) :: kind
    end function

    character function char(x)
    integer, intent(in) :: x
    end function

    integer function len(x)
    character(len=*), intent(in) :: x
    end function

    logical function present(x)
    integer, optional, intent(in) :: x
    end function
end interface

end module
