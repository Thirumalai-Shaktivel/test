program doloop_06
implicit none
integer :: i, j
i = 1
j = 0
do
    j = j + i
    if (i == 2) exit
    i = i + 1
end do
if (j /= 3) error stop

end program
