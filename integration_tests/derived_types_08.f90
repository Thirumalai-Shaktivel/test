module shape_mod

type shape
    integer :: color
    logical :: filled
    integer :: x
    integer :: y
contains
    procedure :: initialize
end type shape

type, extends(shape) :: rectangle
        integer :: length
        integer :: width
end type rectangle

contains

subroutine initialize(sh, color, filled, x, y)
    ! initialize shape objects
    class(shape) :: sh
    integer :: color
    logical :: filled
    integer :: x
    integer :: y

    sh%color = color
    sh%filled = filled
    sh%x = x
    sh%y = y
end subroutine initialize

end module

program derived_types_08
    use shape_mod, only : shape, rectangle, initialize
    type(shape) :: shp                                  ! declare an instance of shape
    type(rectangle) :: rect                             ! declare an instance of rectangle
    call shp%initialize(1, .true., 10, 20)              ! initialize shape
    call rect%initialize(2, .false., 100, 200)  ! initialize rectangle

    print *, shp%color, shp%filled, shp%x, shp%y
    print *, rect%color, rect%filled, rect%x, rect%y

    rect%length = 100
    rect%width = 200

    print *, rect%length, rect%width
end program