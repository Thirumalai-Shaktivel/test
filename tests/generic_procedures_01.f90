! reference - https://stackoverflow.com/a/44798961
module test_m
    implicit none

    type :: A_t
    contains
        procedure :: f_int
        procedure :: f_real
        generic :: f => f_int, f_real
    endtype

contains

    subroutine f_int( me, n )
        type(A_t) :: me
        integer :: n
        print *, "f_int  : ", n
    end subroutine

    subroutine f_real( me, x )
        type(A_t) :: me
        real :: x
        print *, "f_real :", x
    end subroutine

end module

program main
    use test_m, only: A_t
    implicit none
    type(A_t) :: A
    integer :: A_x = 100
    real :: A_r = 1.23

    call A % f( A_x )
    call A % f( A_r )

end