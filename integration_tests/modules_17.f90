program modules_17
use modules_17b, only: Circle
implicit none

type(Circle) :: c     ! Declare a variable of type Circle.
c = Circle(1.5)       ! Use the implicit constructor, radius = 1.5.
call c%print          ! Call the type-bound subroutine
c%radius = 2.0
call c%print          ! Call the type-bound subroutine
end program
