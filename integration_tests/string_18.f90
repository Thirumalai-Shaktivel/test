module stdlib_string_type

contains

    pure function char_string_range(start, last) result(character_string)
        integer, intent(in) :: start
        integer, intent(in) :: last
        character(len=last-start+1) :: character_string

    end function char_string_range

end module

program stdlib_string_use
use stdlib_string_type
implicit none

integer :: begin = 1, end = 5

print *, char_string_range(begin, end)

end program