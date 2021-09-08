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

integer elemental function index(string_, substring_, back_, kind_) result(idx)
character(len=*), intent(in) :: string_
character(len=*), intent(in) :: substring_
logical, optional :: back_
integer, optional :: kind_
integer :: i, j, k
logical :: found
idx = -1
do i = 1, len(string_)
    k = 0
    found = .true.
    do j = 1, len(substring_)
        if( string_(i + k:i + k) .neqv. substring_(j:j) ) then
            found = .false.
            exit
        end if
    end do
    if( found ) then
        if( back_ ) then
            if( idx .lt. i ) then
                idx = i
            end if
        else
            idx = i
        end if
    end if
    if( .not. back_ .and. found ) exit
end do
end function

end module
