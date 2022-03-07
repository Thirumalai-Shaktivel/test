module a
implicit none

contains

    function trim_wrapper(s) result(r)
        character(len=*) :: s
        character(len=len(s)) :: r
        r = trim(s)
    end function

end module

program functions_06
    use :: a, only: trim_wrapper
    implicit none

    character(len=5) :: string = "  abc"
    print *, trim_wrapper(string)

end program
