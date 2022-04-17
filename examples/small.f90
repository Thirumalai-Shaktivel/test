program small
    implicit none
    contains

    function sqr(x) result(r)
        implicit none
        integer, intent(in):: x
        integer :: r
        r = x * x
        return
    end function

    function computeCircleArea(radius) result(area)
        implicit none
        integer, intent(in) :: radius
        integer :: PI, area
        PI = 3 ! it supports only integers so pi is currently 3 and not 3.14
        area = PI * sqr(radius)
        return
    end function
end program
