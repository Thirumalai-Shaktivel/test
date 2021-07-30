module lfortran_intrinsic_math_inter
implicit none

interface
   real function sin(x)
   real, intent(in) :: x(:)
   end function sin

end interface

end module
