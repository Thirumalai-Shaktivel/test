program main
  real :: x(10)
  integer :: i, j
  j = 10
  do i = 1, 10
   x(i) = i*j
  end do
  where (x>=50)
    x = 32.0
  elsewhere
    x = 0.0
  end where
  print *, x
end program
