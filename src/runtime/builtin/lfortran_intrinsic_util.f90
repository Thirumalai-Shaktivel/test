module lfortran_intrinsic_util
implicit none

interface
    logical function present(x)
    integer, intent(in) :: x
    end function

    integer function bit_size(x)
    integer, intent(in) :: x
    end function

    integer function not(x)
    integer, intent(in) :: x
    end function

    integer function iachar(c, kind)
    character(len=1), intent(in) :: c
    integer, optional :: kind
    end function

    character(len=1) function achar(i, kind)
    integer, intent(in) :: i
    integer, optional :: kind
    end function
end interface

end module
