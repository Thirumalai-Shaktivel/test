program string_12
    implicit none

    character(len=*), public, parameter :: fullhex_digits = "0123456789ABCDEFabcdef" !! 0 .. 9A .. Fa .. f
    character(len=*), public, parameter :: hex_digits = fullhex_digits(1:16) !! 0 .. 9A .. F
    character(len=*), public, parameter :: lowerhex_digits = "0123456789abcdef" !! 0 .. 9a .. f
    character(len=*), public, parameter :: digits = hex_digits(1:10) !! 0 .. 9
    character(len=*), public, parameter :: octal_digits = digits(1:8) !! 0 .. 7
    character(len=*), public, parameter :: letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" !! A .. Za .. z
    character(len=*), public, parameter :: uppercase = letters(1:26) !! A .. Z
    character(len=*), public, parameter :: lowercase = letters(27:) !! a .. z

    print *, fullhex_digits
    print *, hex_digits
    print *, lowerhex_digits
    print *, digits
    print *, octal_digits
    print *, letters
    print *, uppercase
    print *, lowercase

end program