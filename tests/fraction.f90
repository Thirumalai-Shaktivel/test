program test_fraction
  real :: x
  x = 3.0
  print *, fraction(x), 0.75
  real :: y
  y = 1024.0
  print *, fraction(y), 0.5
  if (abs(fraction(x)-0.75) > 1e-5) error stop
  if (abs(fraction(y)-0.5) > 1e-5) error stop
end program test_fraction