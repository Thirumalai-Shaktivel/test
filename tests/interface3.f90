module interface3
implicit none
!checks the syntax
public :: x, y, z, assignment(=), operator(+), operator(.and.), operator(.in.)
public :: operator(*)
public :: operator(/)
public :: operator(/ )
public :: operator(// )

interface
    module procedure sample
end interface

interface A
    module procedure :: sample
end interface A

INTERFACE ASSIGNMENT ( = )
    SUBROUTINE LOGICAL_TO_NUMERIC (N, B)
    INTEGER, INTENT (OUT) :: N
    LOGICAL, INTENT (IN) :: B
    END SUBROUTINE LOGICAL_TO_NUMERIC
END INTERFACE ASSIGNMENT ( = )

interface operator (+)
    module procedure union
end interface operator (+)
interface operator ( / )
end interface operator ( / )
interface operator (/)
end interface operator (/)
interface operator (<=)
    module procedure subset
end interface operator (<=)
interface operator (.and.)
end interface operator (.and.)

abstract interface
end interface

public :: operator(//)

interface operator (//)
end interface  operator (//)

contains

    function f(operator)
    ! Currently parsed as an operator, but AST -> ASR phase can fix that:
    real, intent(in) :: operator (*)
    end function f


end module
