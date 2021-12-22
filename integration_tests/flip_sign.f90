program flip_sign
    implicit none
    integer :: number

    number = 123

    if (modulo(number, 2) == 1 ) number = -number
    if (number /= -123) error stop

    number = 124
    if (modulo(number, 2) == 1 ) number = -number
    if (number /= 124) error stop

end program
