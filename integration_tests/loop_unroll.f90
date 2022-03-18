program loop_unroll

    implicit none
    integer :: i, first, last, array(100), x

    do i = 1, 100
        array(i) = i
    end do

    first = 1
    last = 100
    do i = first, last
        array(i) = array(i) + i
    end do

    do i = 1, 100
        x = array(i)
        call print_subrout(x)
    end do

contains

    subroutine print_subrout(x)
        integer :: x
        print *, x
    end subroutine

end program

