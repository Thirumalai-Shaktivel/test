module modules_14_b
implicit none

interface a
    module procedure a1
end interface

contains

    subroutine a1(a)
    integer, intent(inout) :: a
    a = a + 1
    end subroutine

end module
