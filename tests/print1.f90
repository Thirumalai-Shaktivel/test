program print1
implicit none

    real :: pi
    pi = 3.141592

    print "(f6.3)", pi
    print *, pi
    print 10
10  format(3x, "print")

end program
