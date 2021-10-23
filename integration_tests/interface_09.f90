module interface_09_m
    abstract interface
        subroutine add(x, y, z)
            integer, intent(in) :: x
            integer, intent(in) :: y
            integer, intent(in) :: z
        end subroutine
    end interface
end module

program interface_09
    use interface_09_m, only : add
    procedure (add) :: add_def
    procedure (add), pointer :: p
    p => add_def
    call p(5, 10)
end program

subroutine add_def(x, y)
        integer ,intent(in) :: x
        integer ,intent(in) :: y
        print *, "the sum of x and y is: ", x + y
end subroutine