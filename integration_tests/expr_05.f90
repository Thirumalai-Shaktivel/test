program expr_05
! Test parantheses in expressions
implicit none
integer :: x, a, b, c
a = 3
b = 4
c = 5

x = 2*3
x = -2*3
x = 2*(-3)
x = -2*(-3)
x = (-2)*(-3)

x = a*b
x = -a*b
x = a*(-b)
x = (-a)*(-b)

x = a*b*c
x = -a*b*c
x = a*(-b)*c
x = a*b*(-c)
x = (-a)*(-b)*(-c)

x = 3+4*5
x = (3+4)*5
x = (3-2*a*b)*5
x = (-2*a*b+3)*5
x = (+2*a*b+3)*5
x = a**2 + 2*a*b + b**2
x = (a + b)*(a - b)
x = (a + b)**2

x = 2-(-2)
x = a-(-b-c)
x = a-(-2*b)
x = a-(2+3+4)
x = a+(2+3+4)
x = 2*a+a*b-(a*b+2*a)
x = 2*a+a*b-(a*b-2*a)
x = a-(b-(c-1))
end program
