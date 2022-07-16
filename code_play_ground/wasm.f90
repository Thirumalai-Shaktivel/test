program main
   use iso_c_binding

!    print *, add(1, 1)
contains

   integer(kind=c_int) function add(x, y) result(r) bind(c, name="add")
      integer(kind=c_int), intent(in) :: x, y
      r = x + y + 2021
   end function

end program
