program flip_sign

    implicit none
    integer :: a = 1, b, c, Nd
    real :: x = 4.0
    real, parameter :: pi = 3.14
    real(8) :: S1 = 1.0, S2 = 2.0
    complex :: z = (1.0, 1.0)
    b = 2
    c = 3
    print *, a + b * c
    x = (x/pi + 0.5_8*sign(1._4, x)) - Nd*pi
    z = S1+z*S2
    print *, x, z

end program

