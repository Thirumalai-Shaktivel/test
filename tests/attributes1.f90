program attributes1
character (len = 4) :: a, b
character (len = 3) :: c (2)
character, intent(in) :: str*(*)
character, parameter :: shtxt(0:6)*2 = (/'S ','P ','D ','F ','G ','H ','I '/)
integer, volatile :: d, e
real, external :: g
equivalence (a, c (1)), (b, c (2))
type details
    sequence
    integer:: age
    character(50):: name
    contains
        procedure, pass:: name => sample
end type details
intrinsic sin, cos
doubleprecision, intent (in) :: x(..)
end program
