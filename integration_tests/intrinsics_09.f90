program intrinsics_09
  real, parameter :: z=tiny(1.0)/2.0
  if (z < tiny(1.0)) then
     print*, z
  end if
end program
