program where_01
implicit none
real :: a(3, 2), b(3, 2)
where (a >= 0)
    b = 1
else where
    b = 0
end where

print *, a, b

where (a >= 0)
    b = 1
elsewhere
    b = 0
end where

print *, a, b

where (a >= 0)
    b = 1
elsewhere
    b = 0
endwhere

print *, a, b

end program
