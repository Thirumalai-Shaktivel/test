program loop_unroll

    implicit none
    integer :: i, first, last, array(16), x

    do i = 1, 16
        array(i) = i
    end do

    first = 1
    last = 16
    do i = first, last
        array(i) = array(i) + i
    end do

    do i = 1, 16
        x = array(i)
        call print_subrout(x)
    end do

contains

    subroutine print_subrout(x)
        integer :: x
        print *, x
    end subroutine

end program

