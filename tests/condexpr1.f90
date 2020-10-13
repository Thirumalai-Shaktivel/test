program condexpr_01
integer :: i
i = if (.true.) then 10 else 20 endif
print *, i
end
