program fma

    implicit none
    real :: x = 4.0
    real, parameter :: pi = 3.14
    real :: S1 = 1.0, S2 = 2.0
    real :: z = 1.0
    x = (x * 4 + 0.5_4*sign(1._4, x)) - x * pi
    z = S1+z*S2
    print *, x, z

end program

