module module_variable
  implicit none
  integer :: a1
  integer, parameter :: a2 = 3
  integer, allocatable :: a3
  
  real :: arr1(8)
  integer, parameter :: arr2(3) = [1,2,3]
  real, allocatable :: arr3(:)
  
  type :: myothertype
    integer :: n
  end type
  
  type :: mytype
    integer :: a10
    real, allocatable :: arr10(:)
    real(8), allocatable :: arr11(:,:)
    type(myothertype) :: my
  end type
  
  
  
end module