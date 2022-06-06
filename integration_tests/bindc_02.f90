! Tests c_ptr, c_f_pointer, c_loc
program bindc_02
use bindc_02b, only: driver
implicit none
print *, "Main program: calling driver()"
call driver()
end
