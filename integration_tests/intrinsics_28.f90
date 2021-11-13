program debug
    implicit none
    integer :: a, b, c, d, e, f, g, h
    a = 10
    b = -20
    c = 30
    d = -40
    e = 0
    f = 50
    g = 60
    h = -70
    print *, min(a, b, c, d, e, f)
    print *, max(a, b, c, d, e)
    print *, min(a, b, c, d)
    print *, max(a, b, c)
    print *, min(a, b)
end program