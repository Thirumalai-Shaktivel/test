program common1
implicit none
real A, B
common /sample/ A, B
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
