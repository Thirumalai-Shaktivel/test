program wasm_cosh
   use iso_c_binding
   integer, parameter :: dp = kind(0.d0)

   print *, add(1, 1)
   contains
   
   integer(kind=c_int) function add(x, y) result(r) bind(c, name="add")
       integer(kind=c_int), intent(in) :: x, y
       r = x + y + 2021
   end function
   
   real(dp) function fnc() result(r) bind(c)
      real(dp) :: x
      x = 2.3456728392_dp
      r = cosh(x)**2 - sinh(x)**2
   end function
end program
