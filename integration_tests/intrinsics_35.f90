program test_any
logical l
l = any((/.true., .true., .true./))
print *, l
call section()

contains

subroutine section()
    integer, intent(in) :: a(2, 3), b(2, 3)
    a = 1
    b = 1
    b(2, 2) = 2
    print *, any(a .eq. b, 1)
    print *, any(a .eq. b, 2)
end subroutine section
end program test_any