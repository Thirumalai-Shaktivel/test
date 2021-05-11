module lfortran_intrinsic_array
implicit none

interface
    integer function size(x) result(r)
    integer, intent(in) :: x(:)
    end function
end interface

interface
    integer function lbound(x, dim) result(r)
    integer, intent(in) :: x(:)
    integer, intent(in) :: dim
    end function
end interface

interface
    integer function ubound(x, dim) result(r)
    integer, intent(in) :: x(:)
    integer, intent(in) :: dim
    end function
end interface

contains

logical function allocated(x) result(r)
real(8), intent(in) :: x(:)
! TODO: implement allocated()
r = .true.
end function

real(8) function real(x, knd) result(r)
integer, intent(in) :: x
integer, intent(in) :: knd
! TODO: implement real()
r = 0
end function

real(8) function sum(x) result(r)
real(8), intent(in) :: x(:)
! TODO: implement sum()
r = 0
end function

real(8) function abs(x) result(r)
real(8), intent(in) :: x(:)
! TODO: implement abs()
r = 0
end function

integer function maxval(x) result(r)
real(8), intent(in) :: x(:)
! TODO: implement maxval()
r = 0
end function

integer function minval(x) result(r)
real(8), intent(in) :: x(:)
! TODO: implement minval()
r = 0
end function

integer function max(a, b) result(r)
integer, intent(in) :: a, b
! TODO: implement max()
r = 0
end function

integer function min(a, b) result(r)
integer, intent(in) :: a, b
! TODO: implement min()
r = 0
end function

end module
