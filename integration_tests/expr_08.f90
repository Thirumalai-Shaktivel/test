program expr_08
implicit none
integer :: x, A(10), i
real :: r
x = 5
print *, x
x += 3
print *, x
if (x /= 8) error stop

r = 5.5
print *, r
r += 3.5
print *, r
if (abs(r - 9) > 1e-5) error stop

A = 0
do i = 1, 10
    A(i) += 3
end do
print *, A
do i = 1, 10
    A(i) += i
end do
print *, A
end program
