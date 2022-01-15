module module_variable
  implicit none
  integer :: a1
  integer, parameter :: a2 = 3
  integer, allocatable :: a3
  
  real :: arr1(8)
  integer, parameter :: arr2(3) = [1,2,3]
  real, allocatable :: arr3(:)
  
end module