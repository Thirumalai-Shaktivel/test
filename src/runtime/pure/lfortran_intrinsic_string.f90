module lfortran_intrinsic_string
implicit none

contains

integer function len_trim(string) result(r)
character(len=*), intent(in) :: string
r = len(string)
do while(string(r) == " ")
    r = r - 1
    if (r == 0) exit
end do
end function

end module
