module m
    abstract interface
        subroutine sub(x,y)
            integer, intent(in) :: x
            integer, intent(in) :: y
        end subroutine
    end interface
end module

program interface_08
    use m, only : sub
    procedure (sub) sub1
    procedure (sub), pointer :: p
    p => sub1
    call p(5, 10)
end program

subroutine sub1 (x, y)
        integer ,intent(in) :: x
        integer ,intent(in) :: y
        print *, "the sum of x and y is: ", x + y
end subroutine