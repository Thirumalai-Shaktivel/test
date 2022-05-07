program intrinsics_45
    real(4) :: x4 = aint(1.234_4, 4)
    real(8) :: x8 = aint(4.321_8, 8)
    print *, x4, x8
    print *, aint(-1.234_4, 4), aint(-4.321_8, 8)
    print *, aint(-1.234_4), aint(4.321_8)
end program intrinsics_45
