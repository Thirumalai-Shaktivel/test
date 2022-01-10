
from numpy cimport import_array, ndarray, int8_t, int16_t, int32_t, int64_t
cimport module_variable_pxd
  
cdef class __module_variable: 
  """module module_variable
  integer :: a
end module"""
  property a:
    def __get__(self):
      cdef int32_t a
      module_variable_pxd.module_variable_get_a(&a)
      return a
    def __set__(self, int32_t a):
      module_variable_pxd.module_variable_set_a(&a)
      
module_variable = __module_variable()
