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
end interface

end module
