module module_variable_wrapper
  use iso_c_binding
  implicit none
contains
  
  subroutine module_variable_get_a(var) bind(c)
    use module_variable, only: a
    integer(c_int32_t), intent(out) :: var
    var = a
  end subroutine
  
  subroutine module_variable_set_a(var) bind(c)
    use module_variable, only: a
    integer(c_int32_t), intent(in) :: var
    a = var
  end subroutine

end module