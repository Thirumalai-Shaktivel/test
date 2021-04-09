program init_values

    integer, parameter :: i = 1.0, j = 2.0
    real :: r = 4.0
    complex :: c = (3.0, 4.0)
    integer :: a = i**2 + j**2
    print *, i, j, r, c, a

end program