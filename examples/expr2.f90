program expr2
implicit none

integer :: x
complex(4) :: c
complex(8) :: z

x = (2+3)*5
print *, x
print *, tan(0.5)
c = (1.5, 3.5)
print *, c
print *, tan(c)
z = (10.5_8, 30.5_8)
print *, z
print *, tan(z)

end program
