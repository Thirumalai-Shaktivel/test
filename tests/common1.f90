program common1
implicit real(a-z)
real A, B
common /sample/ A, B
common /c/ c, /b/ d, e, f, /c/ g, h, i(5)
common x, y, z(10)
A = 10
B = 20
call pass()
stop
end program

subroutine pass()
real A, B
common /sample/ A, B
print *, A, B
return
end subroutine
