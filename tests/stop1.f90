program stop1
implicit none
! AST only(Syntax check)
    integer :: stop = 15
    integer :: errstop = 25

    if(stop /=15) then
        stop
    else if(errstop /=25) then
        error stop
    end if

    if(stop /=15) then
        stop "message"
    else if(errstop /=25) then
        error stop "message"
    end if

    if(stop /=15) then
        stop 10
    else if(errstop /=25) then
        error stop 10
    end if

    if(stop /=15) then
        stop, quiet = .false.
    else if(errstop /=25) then
        error stop, quiet = .false.
    end if

    if(stop /=15) then
        stop 10, quiet = .true.
    else if(errstop /=25) then
        error stop 10, quiet = .true.
    end if

end program
