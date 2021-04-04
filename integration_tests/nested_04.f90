module a
implicit none

contains

subroutine b()
integer x
x = 6
print *, "b()"
call c()
contains
    subroutine c()
    print *, 5
    print *, x
    end subroutine c
end subroutine b

end module

program nested_02
use a, only: b
implicit none

call b()

end
