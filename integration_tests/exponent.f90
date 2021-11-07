program test_exponent
  real :: x = 1.0
  integer :: i
  i = exponent(x)
  print *, i
  print *, exponent(0.0)
  if (abs(exponent(1.0) - 0.0) > 1e-5) error stop
end program test_exponent