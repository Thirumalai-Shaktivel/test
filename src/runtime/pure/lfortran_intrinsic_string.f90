module lfortran_intrinsic_string
implicit none

contains

integer elemental function len_trim(string) result(r)
character(len=*), intent(in) :: string
r = len(string)
if (r == 0) return
do while(string(r:r) == " ")
    r = r - 1
    if (r == 0) exit
end do
end function

function trim(x) result(r)
character(len=*),intent(in) :: x
character(len=len_trim(x)) :: r
! This does not work yet in LFortran:
!r = x(1:len(r))
! So we do this workaroud that works:
integer :: i
do i = 1, len(r)
    r(i:i) = x(i:i)
end do
end function

integer elemental function index(string_, substring_) result(idx)
character(len=*), intent(in) :: string_
character(len=*), intent(in) :: substring_
integer :: i, j, k, pos
logical :: found = .true.
idx = 0
i = 1
do while (i < len(string_) .and. found)
    k = 0
    j = 1
    do while (j < len(substring_) .and. found)
        pos = i + k
        if( string_(pos:pos) /= substring_(j:j) ) then
            found = .false.
        end if
        k = k + 1
        j = j + 1
    end do
    if( found ) then
        idx = i
        found = .false.
    else
        found = .true.
    end if
    i = i + 1
end do
end function

end module
