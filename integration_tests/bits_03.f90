program bits_03
    implicit none
    integer(4) :: from
    integer(4) :: to
    from = 10
    to = 4
    call mvbits(from, 2, 2, to, 0)
    if (from /= 10) error stop
    if (to /= 6) error stop
    call mvbits(from, 0, 2, to, 2)
    if (from /= 10) error stop
    if (to /= 10) error stop
    from  = -20
    to = 4
    call mvbits(from, 29, 2, to, 2)
    if (from /= -20) error stop
    if (to /= 12) error stop
    call mvbits(from, 2, 2, to, 29)
    if (from /= -20) error stop
    if (to /= 1610612748) error stop
end program