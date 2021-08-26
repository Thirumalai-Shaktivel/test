module lfortran_intrinsic_util
implicit none

interface
    logical function present(x)
    integer, intent(in) :: x
    end function
end interface

end module
