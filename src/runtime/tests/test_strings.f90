program test_strings
use lfortran_intrinsic_string, only: len_trim
implicit none
character(*), parameter :: s1 = " A B ", s2 = " "

if (len_trim(s1) /= 4) error stop
if (len_trim(s2) /= 0) error stop

end program
