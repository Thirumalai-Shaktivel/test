program add
    integer :: addition, sum 
    ! addition = addNums(2, 5)
    ! sum = getSum(4)
    ! print *, sum
    integer :: hello
    hello = 5
    contains
    function addNums(a, b) result(r)
        implicit none
        integer, intent(in) :: a, b
        integer :: r 
        integer :: extra
        r = a + b
    end function addNums

    ! function getSum(n) result(sum)
    !     implicit none
        
    !     integer, intent(in) :: n
    !     integer :: i
    !     integer :: sum
 
    !     sum = 0
    !     do i = 1, n
    !         sum = sum + i
    !     end do
    ! end function getSum
end program
