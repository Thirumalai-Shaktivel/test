program test_radix
  print *, "The radix for the default integer kind is", radix(0)
  print *, "The radix for the default real kind is", radix(0.0)
  if (radix(0) /= 2) error stop
  if (radix(0.0) /= 2) error stop
end program test_radix