program expr2
    implicit none
    integer :: x
    time = 12

    if (time < 10) then         ! Comment 1
        print *, "Good morning"
    else if (time < 20) then    ! Comment 2
        print *, "Good day"
    else                        ! Comment 3
        print *, "Good evening"
    end if                      ! Comment 4

end program
