#include <iostream>
#include <memory> 
#include <libasr/asr.h>
#include <libasr/containers.h>
#include <libasr/exception.h>
#include <libasr/asr_utils.h>
#include <libasr/string_utils.h>


namespace LFortran{
  
struct PyModVar
{  
  std::string pxd_fname;
  std::string module_name;
  
  std::string name; // variable name
  bool derived; // True if attribute of derived type
  std::string derived_name; // name of derived type, if derived
  std::string base_ftype; // e.g. integer or real
  std::string bindc_type; // e.g. c_int32_t
  std::string ctype; // e.g. int32_t
  std::string nptype; // e.g. np.int32
  size_t m_storage; // allocatable, parameter, default
  size_t n_dims; // number of dimensions
  std::vector<size_t> dims; // dimensions
}; 

struct PyFiles
{
  std::string f90 = "";
  std::string chdr = "";
  std::string pxd = "";
  std::string pyx = "";
};
  
  
// map between ASR types, iso_c_binding types, C types, and numpy types
void map2c(size_t type, int64_t m_kind, std::string &ctype, std::string &bindc_type, std::string &nptype){
   
   if (type == ASR::Integer && m_kind == 1){
     ctype = "int8_t";
     bindc_type = "c_int8_t";
     nptype = "np.int8";
   }
   else if (type == ASR::Integer && m_kind == 2){
     ctype = "int16_t";
     bindc_type = "c_int16_t";
     nptype = "np.int16";
   }
   else if (type == ASR::Integer && m_kind == 4){
     ctype = "int32_t";
     bindc_type = "c_int32_t";
     nptype = "np.int32";
   }
   else if (type == ASR::Integer && m_kind == 8){
     ctype = "int64_t";
     bindc_type = "c_int64_t";
     nptype = "np.int64";
   }
   else if (type == ASR::Real && m_kind == 4){
     ctype = "float";
     bindc_type = "c_float";
     nptype = "np.float32";
   }
   else if (type == ASR::Real && m_kind == 8){
     ctype = "double";
     bindc_type = "c_double";
     nptype = "np.float64";
   } 
}

// Generates getter and setters for integer, real, complex of any size (e.g. single precision)
void intrinsic1(PyModVar v, std::string &f90_tmp, 
  std::string &chdr_tmp, std::string &pxd_tmp, std::string &pyx_tmp)
{
  
  std::string getter_name;
  std::string setter_name;
  std::string a_name;
  if (v.derived){
    getter_name = v.module_name+"_"+v.derived_name+"_"+v.name+"_get";
    setter_name = v.module_name+"_"+v.derived_name+"_"+v.name+"_set";
    a_name = "t%"+v.name;
  }
  else {
    getter_name = v.module_name+"_"+v.name+"_get";
    setter_name = v.module_name+"_"+v.name+"_set";
    a_name = "a";
  }
  
  //////////////////
  // _wrapper.f90 //
  //////////////////
  
  // f90 getter //
  if (v.derived){
    f90_tmp =  "  subroutine "+getter_name+"(ptr, var) bind(c)\n";
    f90_tmp += "    use "+v.module_name+", only: dtype => "+v.derived_name+"\n";
    f90_tmp += "    type(c_ptr), intent(in) :: ptr\n";
  }
  else {
    f90_tmp =  "  subroutine "+getter_name+"(var) bind(c)\n";
    f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
  }
  f90_tmp += "    "+v.base_ftype+"("+v.bindc_type+"), intent(out) :: var\n";
  if (v.derived){
    f90_tmp += "    type(dtype), pointer :: t\n";
    f90_tmp += "    call c_f_pointer(ptr, t)\n";
  }
  if (v.m_storage == ASR::Allocatable){
    f90_tmp += "    if (allocated("+a_name+")) then\n";
    f90_tmp += "      var = "+a_name+"\n";
    f90_tmp += "    endif\n";
  }
  else{
    f90_tmp += "    var = "+a_name+"\n";
  }
  f90_tmp += "  end subroutine\n\n";
  
  // f90 setter //
  if (v.m_storage != ASR::Parameter){
    if (v.derived){
      f90_tmp +=  "  subroutine "+setter_name+"(ptr, var) bind(c)\n";
      f90_tmp += "    use "+v.module_name+", only: dtype => "+v.derived_name+"\n";
      f90_tmp += "    type(c_ptr), intent(in) :: ptr\n";
    }
    else {
      f90_tmp += "  subroutine "+setter_name+"(var) bind(c)\n";
      f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
    }
    f90_tmp += "    "+v.base_ftype+"("+v.bindc_type+"), intent(in) :: var\n";
    if (v.derived){
      f90_tmp += "    type(dtype), pointer :: t\n";
      f90_tmp += "    call c_f_pointer(ptr, t)\n";
    }
    if (v.m_storage == ASR::Allocatable){
      f90_tmp += "    if (.not. allocated("+a_name+")) then\n";
      f90_tmp += "      allocate("+a_name+")\n";
      f90_tmp += "    endif\n";
    }
    f90_tmp += "    "+a_name+" = var\n";
    f90_tmp += "  end subroutine\n\n";
  }
  
  //////////////
  // C header //
  //////////////
  chdr_tmp = "void "+getter_name+"(";
  if (v.derived){
    chdr_tmp += "void *ptr, ";
  }
  chdr_tmp += v.ctype+" *var);\n";
  if (v.m_storage != ASR::Parameter){
    chdr_tmp += "void "+setter_name+"(";
    if (v.derived){
      chdr_tmp += "void *ptr, ";
    }
    chdr_tmp += v.ctype+" *var);\n";
  }
  
  ///////////////////
  // Cython header //
  ///////////////////
  pxd_tmp = "  cdef void "+getter_name+"(";
  if (v.derived){
    pxd_tmp += "void *ptr, ";
  }
  pxd_tmp += v.ctype+" *var);\n";
  if (v.m_storage != ASR::Parameter){
    pxd_tmp += "  cdef void "+setter_name+"(";
    if (v.derived){
      pxd_tmp += "void *ptr, ";
    }
    pxd_tmp += v.ctype+" *var);\n";
  }
  
  ////////////////
  // Cython pyx //
  ////////////////
  pyx_tmp =  "  property "+v.name+":\n";
  pyx_tmp += "    def __get__(self):\n";
  pyx_tmp += "      cdef "+v.ctype+" var\n";
  if (v.derived){
    pyx_tmp += "      "+v.pxd_fname+"."+getter_name+"(&self._ptr, &var)\n";
  } 
  else {
     pyx_tmp += "      "+v.pxd_fname+"."+getter_name+"(&var)\n";
  }
  pyx_tmp += "      return var\n";
  if (v.m_storage != ASR::Parameter){
    pyx_tmp += "    def __set__(self, "+v.ctype+" var):\n";
    if (v.derived){
      pyx_tmp += "      "+v.pxd_fname+"."+setter_name+"(&self._ptr, &var)\n";
    }
    else {
      pyx_tmp += "      "+v.pxd_fname+"."+setter_name+"(&var)\n";
    }
  }
  pyx_tmp += "\n";
  
}

// Generates getter and setters for integer, real, complex of any size (e.g. single precision)
// for fixed size arrays.
void intrinsic2(PyModVar v, std::string &f90_tmp, 
  std::string &chdr_tmp, std::string &pxd_tmp, std::string &pyx_tmp)
{
  
  std::string getter_name;
  std::string setter_name;
  std::string a_name;
  if (v.derived){
    getter_name = v.module_name+"_"+v.derived_name+"_"+v.name+"_get";
    setter_name = v.module_name+"_"+v.derived_name+"_"+v.name+"_set";
    a_name = "t%"+v.name;
  }
  else {
    getter_name = v.module_name+"_"+v.name+"_get";
    setter_name = v.module_name+"_"+v.name+"_set";
    a_name = "a";
  }
  
  //////////////////
  // _wrapper.f90 //
  //////////////////
  
  // f90 getter //
  if (v.derived){
    f90_tmp =  "  subroutine "+getter_name+"(ptr, var) bind(c)\n";
    f90_tmp += "    use "+v.module_name+", only: dtype => "+v.derived_name+"\n";
    f90_tmp += "    type(c_ptr), intent(in) :: ptr\n";
  }
  else {
    f90_tmp =  "  subroutine "+getter_name+"(var) bind(c)\n";
    f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
  }
  f90_tmp += "    "+v.base_ftype+"("+v.bindc_type+"), intent(out) :: var(";
  for (size_t i = 0; i < v.n_dims; i++){
    if (i == v.n_dims-1){
      f90_tmp += std::to_string(v.dims[i])+")\n";
    }
    else {
      f90_tmp += std::to_string(v.dims[i])+",";
    }
  }
  if (v.derived){
    f90_tmp += "    type(dtype), pointer :: t\n";
    f90_tmp += "    call c_f_pointer(ptr, t)\n";
  }
  f90_tmp += "    var = "+a_name+"\n";
  f90_tmp += "  end subroutine\n\n";
  
  // f90 setter //
  if (v.m_storage != ASR::Parameter){
    if (v.derived){
      f90_tmp +=  "  subroutine "+setter_name+"(ptr, var) bind(c)\n";
      f90_tmp += "    use "+v.module_name+", only: dtype => "+v.derived_name+"\n";
      f90_tmp += "    type(c_ptr), intent(in) :: ptr\n";
    }
    else {
      f90_tmp += "  subroutine "+setter_name+"(var) bind(c)\n";
      f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
    }
    f90_tmp += "    "+v.base_ftype+"("+v.bindc_type+"), intent(in) :: var(";
    for (size_t i = 0; i < v.n_dims; i++){
      if (i == v.n_dims-1){
        f90_tmp += std::to_string(v.dims[i])+")\n";
      }
      else {
        f90_tmp += std::to_string(v.dims[i])+",";
      }
    }
    if (v.derived){
      f90_tmp += "    type(dtype), pointer :: t\n";
      f90_tmp += "    call c_f_pointer(ptr, t)\n";
    }
    f90_tmp += "    "+a_name+" = var\n";
    f90_tmp += "  end subroutine\n\n";
  }
  
  //////////////
  // C header //
  //////////////
  chdr_tmp = "void "+getter_name+"(";
  if (v.derived){
    chdr_tmp += "void *ptr, ";
  }
  chdr_tmp += v.ctype+" *var);\n";
  if (v.m_storage != ASR::Parameter){
    chdr_tmp += "void "+setter_name+"(";
    if (v.derived){
      chdr_tmp += "void *ptr, ";
    }
    chdr_tmp += v.ctype+" *var);\n";
  }
  
  ///////////////////
  // Cython header //
  ///////////////////
  pxd_tmp = "  cdef void "+getter_name+"(";
  if (v.derived){
    pxd_tmp += "void *ptr, ";
  }
  pxd_tmp += v.ctype+" *var);\n";
  if (v.m_storage != ASR::Parameter){
    pxd_tmp += "  cdef void "+setter_name+"(";
    if (v.derived){
      pxd_tmp += "void *ptr, ";
    }
    pxd_tmp += v.ctype+" *var);\n";
  }
  
  ////////////////
  // Cython pyx //
  ////////////////  
  pyx_tmp =  "  property "+v.name+":\n";
  pyx_tmp += "    def __get__(self):\n";
  pyx_tmp += "      cdef ndarray var = np.empty((";
  for (size_t i = 0; i < v.n_dims; i++){
    if (i == v.n_dims-1){
      pyx_tmp += std::to_string(v.dims[i])+",)";
    }
    else {
      pyx_tmp += std::to_string(v.dims[i])+",";
    }
  }
  pyx_tmp += ", "+v.nptype+", order='F')\n";
  if (v.derived){
    pyx_tmp += "      "+v.pxd_fname+"."+getter_name+"(&self._ptr, <"+v.ctype+" *>var.data)\n";
  } 
  else {
    pyx_tmp += "      "+v.pxd_fname+"."+getter_name+"(<"+v.ctype+" *>var.data)\n";
  }
  pyx_tmp += "      return var\n";
  if (v.m_storage != ASR::Parameter){
    pyx_tmp += "    def __set__(self, ndarray["+v.ctype+", ndim="+std::to_string(v.n_dims)+"] var_in):\n";
    pyx_tmp += "      cdef ndarray var = np.asfortranarray(var_in)\n";
    pyx_tmp += "      cdef ndarray d = np.empty((var.ndim,), np.int64)\n";
    pyx_tmp += "      for i in range(var.ndim):\n";
    pyx_tmp += "        d[i] = var.shape[i]\n";
    pyx_tmp += "      if (";
    for (size_t i = 0; i < v.n_dims; i++){
      pyx_tmp += std::to_string(v.dims[i])+",";
    }
    pyx_tmp += ") != tuple(d):\n";
    pyx_tmp += "        raise ValueError('"+v.name+" has the wrong shape')\n";
    if (v.derived){
      pyx_tmp += "      "+v.pxd_fname+"."+setter_name+"(&self._ptr, <"+v.ctype+" *>var.data)\n";
    } 
    else {
      pyx_tmp += "      "+v.pxd_fname+"."+setter_name+"(<"+v.ctype+" *>var.data)\n";
    }
  }
  pyx_tmp += "\n";
  
}  

// Generates getter and setters for integer, real, complex of any size (e.g. single precision)
// for allocatable arrays
void intrinsic3(PyModVar v, std::string &f90_tmp, 
    std::string &chdr_tmp, std::string &pxd_tmp, std::string &pyx_tmp){
      
    std::string getter_name;
    std::string setter_name;
    std::string a_name;
    if (v.derived){
      getter_name = v.module_name+"_"+v.derived_name+"_"+v.name+"_get";
      setter_name = v.module_name+"_"+v.derived_name+"_"+v.name+"_set";
      a_name = "t%"+v.name;
    }
    else {
      getter_name = v.module_name+"_"+v.name+"_get";
      setter_name = v.module_name+"_"+v.name+"_set";
      a_name = "a";
    }
    
    //////////////////
    // _wrapper.f90 //
    //////////////////
    
    // f90 get size //
    if (v.derived){
      f90_tmp =  "  subroutine "+getter_name+"_size(ptr, d) bind(c)\n";
      f90_tmp += "    use "+v.module_name+", only: dtype => "+v.derived_name+"\n";
      f90_tmp += "    type(c_ptr), intent(in) :: ptr\n";
    }
    else {
      f90_tmp =  "  subroutine "+getter_name+"_size(d) bind(c)\n";
      f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
    }
    f90_tmp += "    integer(c_int64_t), intent(out) :: d("+std::to_string(v.n_dims)+")\n";
    if (v.derived){
      f90_tmp += "    type(dtype), pointer :: t\n";
      f90_tmp += "    call c_f_pointer(ptr, t)\n";
    }
    f90_tmp += "    if (allocated("+a_name+")) then\n";
    f90_tmp += "      d = shape("+a_name+")\n";
    f90_tmp += "    else\n";
    f90_tmp += "      d = 0\n";
    f90_tmp += "    endif\n";
    f90_tmp += "  end subroutine\n\n";  
    
    // f90 getter //
    if (v.derived){
      f90_tmp += "  subroutine "+getter_name+"(ptr, d, var) bind(c)\n";
      f90_tmp += "    use "+v.module_name+", only: dtype => "+v.derived_name+"\n";
      f90_tmp += "    type(c_ptr), intent(in) :: ptr\n";
    }
    else {
      f90_tmp += "  subroutine "+getter_name+"(d, var) bind(c)\n";
      f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
    }
    f90_tmp += "    integer(c_int64_t), intent(in) :: d("+std::to_string(v.n_dims)+")\n";
    f90_tmp += "    "+v.base_ftype+"("+v.bindc_type+"), intent(out) :: var(";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "d("+std::to_string(i+1)+")";
      if (i < v.n_dims - 1){
        f90_tmp += ", ";
      }
    }
    f90_tmp += ")\n";
    if (v.derived){
      f90_tmp += "    type(dtype), pointer :: t\n";
      f90_tmp += "    call c_f_pointer(ptr, t)\n";
    }
    f90_tmp += "    if (allocated("+a_name+")) then\n";
    f90_tmp += "      var = "+a_name+"\n";
    f90_tmp += "    endif\n";
    f90_tmp += "  end subroutine\n\n"; 
    
    // f90 setter //
    if (v.derived){
      f90_tmp +=  "  subroutine "+setter_name+"(ptr, d, var) bind(c)\n";
      f90_tmp += "    use "+v.module_name+", only: dtype => "+v.derived_name+"\n";
      f90_tmp += "    type(c_ptr), intent(in) :: ptr\n";
    }
    else {
      f90_tmp += "  subroutine "+setter_name+"(d, var) bind(c)\n";
      f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
    }
    f90_tmp += "    integer(c_int64_t), intent(in) :: d("+std::to_string(v.n_dims)+")\n";
    f90_tmp += "    "+v.base_ftype+"("+v.bindc_type+"), intent(in) :: var(";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "d("+std::to_string(i+1)+")";
      if (i < v.n_dims - 1){
        f90_tmp += ", ";
      }
    }
    f90_tmp += ")\n";
    if (v.derived){
      f90_tmp += "    type(dtype), pointer :: t\n";
      f90_tmp += "    call c_f_pointer(ptr, t)\n";
    }
    f90_tmp += "    if (allocated("+a_name+")) deallocate("+a_name+")\n";
    f90_tmp += "    allocate("+a_name+"(";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "d("+std::to_string(i+1)+")";
      if (i < v.n_dims - 1){
        f90_tmp += ", ";
      }
    }
    f90_tmp += "))\n";
    f90_tmp += "    "+a_name+" = var\n";
    f90_tmp += "  end subroutine\n\n";
    
    //////////////
    // C header //
    //////////////
    chdr_tmp = "void "+getter_name+"_size(";
    if (v.derived){
      chdr_tmp += "void *ptr, ";
    }
    chdr_tmp += "int64_t *d);\n";
    chdr_tmp += "void "+getter_name+"(";
    if (v.derived){
      chdr_tmp += "void *ptr, ";
    }
    chdr_tmp += "int64_t *d, "+v.ctype+" *var);\n";

    chdr_tmp += "void "+setter_name+"(";
    if (v.derived){
      chdr_tmp += "void *ptr, ";
    }
    chdr_tmp += "int64_t *d, "+v.ctype+" *var);\n";
    
    ///////////////////
    // Cython header //
    ///////////////////
    pxd_tmp = "  cdef void "+getter_name+"_size(";
    if (v.derived){
      pxd_tmp += "void *ptr, ";
    }
    pxd_tmp += "int64_t *d);\n";
    pxd_tmp += "  cdef void "+getter_name+"(";
    if (v.derived){
      pxd_tmp += "void *ptr, ";
    }
    pxd_tmp += "int64_t *d, "+v.ctype+" *var);\n";

    pxd_tmp += "  cdef void "+setter_name+"(";
    if (v.derived){
      pxd_tmp += "void *ptr, ";
    }
    pxd_tmp += "int64_t *d, "+v.ctype+" *var);\n";
    
    
    ////////////////
    // Cython pyx //
    ////////////////
    pyx_tmp =  "  property "+v.name+":\n";
    pyx_tmp += "    def __get__(self):\n";
    pyx_tmp += "      cdef ndarray d = np.empty(("+std::to_string(v.n_dims)+",), np.int64)\n";
    if (v.derived){
      pyx_tmp += "      "+v.pxd_fname+"."+getter_name+"_size(&self._ptr, <int64_t *> d.data)\n";
    }
    else {
      pyx_tmp += "      "+v.pxd_fname+"."+getter_name+"_size(<int64_t *> d.data)\n";
    }
    pyx_tmp += "      cdef ndarray var = np.empty(tuple(d), "+v.nptype+", order='F')\n";
    pyx_tmp += "      "+v.pxd_fname+"."+getter_name+"(";
    if (v.derived){
      pyx_tmp += "&self._ptr, ";
    }
    pyx_tmp += "<int64_t *> d.data, <"+v.ctype+" *>var.data)\n";
    pyx_tmp += "      return var\n";
    pyx_tmp += "    def __set__(self, ndarray["+v.ctype+", ndim="+std::to_string(v.n_dims)+"] var_in):\n";
    pyx_tmp += "      cdef ndarray var = np.asfortranarray(var_in)\n";
    pyx_tmp += "      cdef ndarray d = np.empty((var.ndim,), np.int64)\n";
    pyx_tmp += "      for i in range(var.ndim):\n";
    pyx_tmp += "        d[i] = var.shape[i]\n";
    if (v.derived){
      pyx_tmp += "      "+v.pxd_fname+"."+setter_name+"(&self._ptr, <int64_t *> d.data, <"+v.ctype+" *>var.data)\n";
    } 
    else {
      pyx_tmp += "      "+v.pxd_fname+"."+setter_name+"(<int64_t *> d.data, <"+v.ctype+" *>var.data)\n";
    }
    pyx_tmp += "\n";
    
}

void dtype_init(std::string pxd_fname, std::string module_name, std::string type_name,
  std::string &f90_tmp, std::string &chdr_tmp, std::string &pxd_tmp, std::string &pyx_tmp)
{
  
  //////////////////
  // _wrapper.f90 //
  //////////////////
  f90_tmp =  "  subroutine "+module_name+"_"+type_name+"_alloc(ptr) bind(c)\n";
  f90_tmp += "    use "+module_name+", only: dtype => "+type_name+"\n";
  f90_tmp += "    type(c_ptr), intent(out) :: ptr\n";
  f90_tmp += "    type(dtype), pointer :: t\n";
  f90_tmp += "    allocate(t)\n";
  f90_tmp += "    ptr = c_loc(t)\n";
  f90_tmp += "  end subroutine\n\n";
  
  f90_tmp += "  subroutine "+module_name+"_"+type_name+"_dealloc(ptr) bind(c)\n";
  f90_tmp += "    use "+module_name+", only: dtype => "+type_name+"\n";
  f90_tmp += "    type(c_ptr), intent(in) :: ptr\n";
  f90_tmp += "    type(dtype), pointer :: t\n";
  f90_tmp += "    call c_f_pointer(ptr, t)\n";
  f90_tmp += "    deallocate(t)\n";
  f90_tmp += "  end subroutine\n\n";
  
  //////////////
  // C header //
  //////////////
  chdr_tmp =  "void "+module_name+"_"+type_name+"_alloc(void *ptr);\n";
  chdr_tmp += "void "+module_name+"_"+type_name+"_dealloc(void *ptr);\n";
  
  ///////////////////
  // Cython header //
  ///////////////////
  pxd_tmp =  "  cdef void "+module_name+"_"+type_name+"_alloc(void *ptr);\n";
  pxd_tmp += "  cdef void "+module_name+"_"+type_name+"_dealloc(void *ptr);\n";
  
  ////////////////
  // Cython pyx //
  ////////////////
  pyx_tmp =  "  def __cinit__(self, bint alloc = True):\n";
  pyx_tmp += "    if alloc:\n";
  pyx_tmp += "      self._destroy = True\n";
  pyx_tmp += "      "+pxd_fname+"."+module_name+"_"+type_name+"_alloc(&self._ptr)\n";
  pyx_tmp += "    else:\n";
  pyx_tmp += "      self._destroy = False\n\n";
  
  pyx_tmp += "  def __dealloc__(self):\n";
  pyx_tmp += "    if self._destroy:\n";
  pyx_tmp += "      "+pxd_fname+"."+module_name+"_"+type_name+"_dealloc(&self._ptr)\n\n";
  
}


namespace {

    // Local exception that is only used in this file to exit the visitor
    // pattern and caught later (not propagated outside)
    class CodeGenError
    {
    public:
        diag::Diagnostic d;
    public:
        CodeGenError(const std::string &msg)
            : d{diag::Diagnostic(msg, diag::Level::Error, diag::Stage::CodeGen)}
        { }
    };

}

using ASR::is_a;
using ASR::down_cast;
using ASR::down_cast2;

class ASRToPyVisitor : public ASR::BaseVisitor<ASRToPyVisitor>
{
public:
  
  std::string module_name; // module to be wrapped
  
  std::string pyx; // Cython implementation file (.pyx)
  std::string pxd; // Cython header file (.pxd)
  std::string f90; // Fortran wrapper file (.f90)
  std::string chdr; // C header file for Fortran wrapper (.h)
  
  std::string pxd_fname = "pxd_fname";
  std::string chdr_fname = "chdr_fname";
  
  void visit_TranslationUnit(const ASR::TranslationUnit_t &x){
    
    pxd_fname = module_name+"_pxd";
    chdr_fname = module_name+"_wrapper";
    
    chdr =  "// This file was automatically generated by the LFortran compiler.\n\n";
    chdr += "#include <stdint.h>\n\n";
    
    pxd =   "# This file was automatically generated by the LFortran compiler.\n\n";
    pxd +=  "from libc.stdint cimport int8_t, int16_t, int32_t, int64_t\n\n";
    pxd +=  "cdef extern from \""+chdr_fname+".h\":\n";
    
    pyx =  "# This file was automatically generated by the LFortran compiler.\n\n";
    pyx += "from numpy cimport ndarray, int8_t, int16_t, int32_t, int64_t\n";
    pyx += "import numpy as np\n";
    pyx += "cimport "+pxd_fname+" \n\n";  
    
    f90 =  "! This file was automatically generated by the LFortran compiler.\n\n";
    f90 += "module "+module_name+"_wrapper\n";
    f90 += "  use iso_c_binding\n";
    f90 += "  implicit none\n\n";
    f90 += "contains\n\n";
    
    std::vector<std::string> build_order
        = ASRUtils::determine_module_dependencies(x);
    
    // loop over modules in the translation unit
    for(auto item : build_order){
      ASR::symbol_t *s = x.m_global_scope->scope[item];
      if (s->type == ASR::Module){
        ASR::Module_t *mod = ASR::down_cast<ASR::Module_t>(s);
        wrap_Module(*mod);
      }    
    }
    
    f90 += "end module";
    pyx += "\n"+module_name+" = __"+module_name+"()\n";
  }

  void wrap_Module(const ASR::Module_t &x)
  {
    // check if module is the right one
    if (x.m_name != module_name){
      return;
    }
    
    std::vector<std::string> dtype_names = {};
    // first wrap any derived types
    for (auto &item : x.m_symtab->scope) {
      if (is_a<ASR::DerivedType_t>(*item.second)) {
        ASR::DerivedType_t *s = ASR::down_cast<ASR::DerivedType_t>(item.second);
        PyFiles f = wrap_DerivedType(*s);
        f90 += f.f90;
        chdr += f.chdr;
        pxd += f.pxd;
        pyx += f.pyx;
        dtype_names.push_back(s->m_name);
      }
      else if (is_a<ASR::ExternalSymbol_t>(*item.second)) {
        ASR::ExternalSymbol_t *e = ASR::down_cast<ASR::ExternalSymbol_t>(item.second);
        if (e->m_external->type == ASR::DerivedType){
          ASR::DerivedType_t *s = ASR::down_cast<ASR::DerivedType_t>(e->m_external);
          PyFiles f = wrap_DerivedType(*s);
          f90 += f.f90;
          chdr += f.chdr;
          pxd += f.pxd;
          pyx += f.pyx;
          dtype_names.push_back(s->m_name);
        }
      }
    }
    
    pyx += "cdef class __"+module_name+":\n";
    
    // loop through symbols in module
    for (auto &item : x.m_symtab->scope) {
      
      if (is_a<ASR::Variable_t>(*item.second)) {
        // variables in module
        ASR::Variable_t *s = ASR::down_cast<ASR::Variable_t>(item.second);
        std::string empty_str = "";
        PyFiles f = wrap_Variable(*s, false, empty_str);
        f90 += f.f90;
        chdr += f.chdr;
        pxd += f.pxd;
        pyx += f.pyx;
      }
      else if (is_a<ASR::ExternalSymbol_t>(*item.second)) {
        ASR::ExternalSymbol_t *e = ASR::down_cast<ASR::ExternalSymbol_t>(item.second);
        if (e->m_external->type == ASR::Variable){
          ASR::Variable_t *s = ASR::down_cast<ASR::Variable_t>(e->m_external);
          std::string empty_str = "";
          PyFiles f = wrap_Variable(*s, false, empty_str);
          f90 += f.f90;
          chdr += f.chdr;
          pxd += f.pxd;
          pyx += f.pyx;
        }
      }
      
    }
    
    // Tack derived types onto the module
    if (dtype_names.size() > 0){
      for (size_t i = 0; i < dtype_names.size(); i++){
        pyx += "  property "+dtype_names[i]+":\n";
        pyx += "    def __get__(self):\n";
        pyx += "      return __"+module_name+"_"+dtype_names[i]+"\n\n";
        
      }      
    }
    
    // wrap functions and subroutines
    for (auto &item : x.m_symtab->scope) {
      if (is_a<ASR::Subroutine_t>(*item.second)){
        ASR::Subroutine_t *s = ASR::down_cast<ASR::Subroutine_t>(item.second);
        std::cout << "Subroutine: " << s->m_name << std::endl;
        // wrap subroutines
      }
      else if (is_a<ASR::Function_t>(*item.second)){
        ASR::Function_t *s = ASR::down_cast<ASR::Function_t>(item.second);
        std::cout << "Function: " << s->m_name << std::endl;
        // wrap function
      }
      
    }
  
  }
  
  
  PyFiles wrap_DerivedType(const ASR::DerivedType_t &x)
  {
    PyFiles f;
    std::cout << "Wrapping derived type: " << x.m_name << std::endl;
    if (x.m_access == ASR::Private){
      std::cout << x.m_name << " is private so not wrapping" << std::endl;
      return f;
    }
  
    // Beginning of type
    std::string tmp = x.m_name;
    f.pyx = "cdef class __"+module_name+"_"+tmp+":\n";
    f.pyx += "  cdef void *_ptr\n";
    f.pyx += "  cdef bint _destroy\n\n";
    
    std::string f90_tmp = "";
    std::string chdr_tmp = "";
    std::string pxd_tmp = "";
    std::string pyx_tmp = "";
    
    // we make allocator first
    dtype_init(pxd_fname, module_name, tmp, 
      f90_tmp, chdr_tmp, pxd_tmp, pyx_tmp);
    
    f.f90 += f90_tmp;
    f.chdr += chdr_tmp;
    f.pxd += pxd_tmp;
    f.pyx += pyx_tmp;
    
    // loop through symbols in derived type
    for (auto &item : x.m_symtab->scope) {
      
      if (is_a<ASR::Variable_t>(*item.second)) {
        ASR::Variable_t *s = ASR::down_cast<ASR::Variable_t>(item.second);
        PyFiles f1 = wrap_Variable(*s, true, tmp);
        f.f90 += f1.f90;
        f.chdr += f1.chdr;
        f.pxd += f1.pxd;
        f.pyx += f1.pyx;
      }
      else {
        // There shouldn't be anything but variables?
      }
      
    }
    
    return f;
    
  }
  
  PyFiles wrap_Variable(const ASR::Variable_t &x, bool derived, std::string &derived_name)
  {
    
    PyFiles f;
    std::string f90_tmp = "";
    std::string chdr_tmp = "";
    std::string pxd_tmp = "";
    std::string pyx_tmp = "";
    
    if (x.m_access == ASR::Private){
      return f;
    }
    
    PyModVar v;
    v.derived = derived; // if the varaible is CONTAINED within a derived type
    if (v.derived){
      v.derived_name = derived_name;
    }
    
    v.name = x.m_name;
    v.module_name = module_name;
    v.pxd_fname = pxd_fname;
    v.m_storage = x.m_storage; 
    if (x.m_type->type == ASR::Integer){
      ASR::Integer_t *tmp = down_cast<ASR::Integer_t>(x.m_type);
      map2c(ASR::Integer, tmp->m_kind, v.ctype, v.bindc_type, v.nptype);
      v.base_ftype = "integer";
      v.n_dims = tmp->n_dims;
      
      // get dimensions if not allocatable
      if (v.n_dims > 0 && v.m_storage != ASR::Allocatable){
        v.dims.resize(v.n_dims);
        for (size_t i = 0; i < v.n_dims; i++){
          ASR::ConstantInteger_t *d_start = down_cast<ASR::ConstantInteger_t>(tmp->m_dims[i].m_start);
          ASR::ConstantInteger_t *d_end = down_cast<ASR::ConstantInteger_t>(tmp->m_dims[i].m_end);
          v.dims[i] = (d_end->m_n - d_start->m_n + 1);
        }
      }
        
    } 
    else if (x.m_type->type == ASR::Real) {
      ASR::Real_t *tmp = down_cast<ASR::Real_t>(x.m_type);
      map2c(ASR::Real, tmp->m_kind, v.ctype, v.bindc_type, v.nptype);
      v.base_ftype = "real";
      v.n_dims = tmp->n_dims;
      
      // get dimensions if not allocatable
      if (v.n_dims > 0 && v.m_storage != ASR::Allocatable){
        v.dims.resize(v.n_dims);
        for (size_t i = 0; i < v.n_dims; i++){
          ASR::ConstantInteger_t *d_start = down_cast<ASR::ConstantInteger_t>(tmp->m_dims[i].m_start);
          ASR::ConstantInteger_t *d_end = down_cast<ASR::ConstantInteger_t>(tmp->m_dims[i].m_end);
          v.dims[i] = (d_end->m_n - d_start->m_n + 1);
        }
      }
    }
    else if (x.m_type->type == ASR::Derived){
      if (v.derived){
        std::cout << "  ";
      }
      std::cout << "Skipping: " << v.name << std::endl;
      return f;

      ASR::Derived_t *tmp = down_cast<ASR::Derived_t>(x.m_type);
      ASR::DerivedType_t *tmp_d = down_cast<ASR::DerivedType_t>(tmp->m_derived_type);
      v.ctype = "void *";
      v.bindc_type = "c_ptr";
      v.base_ftype = "type";
      // v.nptype = "";
      v.n_dims = tmp->n_dims;
      if (v.n_dims > 0 && v.m_storage != ASR::Allocatable){
        v.dims.resize(v.n_dims);
        for (size_t i = 0; i < v.n_dims; i++){
          ASR::ConstantInteger_t *d_start = down_cast<ASR::ConstantInteger_t>(tmp->m_dims[i].m_start);
          ASR::ConstantInteger_t *d_end = down_cast<ASR::ConstantInteger_t>(tmp->m_dims[i].m_end);
          v.dims[i] = (d_end->m_n - d_start->m_n + 1);
        }
      }
    }
    else {
      std::cout << "Type not supported: " << v.name << std::endl;
      return f;
    }
    
    if (x.m_type->type == ASR::Derived){
      if (v.derived){
        std::cout << "  ";
      }
      std::cout << "Wrapping: " << v.name << std::endl;
      // dtype1(v, f90_tmp, chdr_tmp, pxd_tmp, pyx_tmp);
      
    }
    else {
    
    if (v.n_dims == 0){
      if (v.derived){
        std::cout << "  ";
      }
      std::cout << "Wrapping: " << v.name << std::endl;
      intrinsic1(v, f90_tmp, chdr_tmp, pxd_tmp, pyx_tmp);
    }
    else if (v.n_dims != 0 && v.m_storage != ASR::Allocatable){
      if (v.derived){
        std::cout << "  ";
      }
      std::cout << "Wrapping: " << v.name << std::endl;
      intrinsic2(v, f90_tmp, chdr_tmp, pxd_tmp, pyx_tmp);
    }
    else if (v.n_dims != 0 && v.m_storage == ASR::Allocatable){
      if (v.derived){
        std::cout << "  ";
      }
      std::cout << "Wrapping: " << v.name << std::endl;
      intrinsic3(v, f90_tmp, chdr_tmp, pxd_tmp, pyx_tmp);
    }
    else {
      // std::cout << "Skipping " << v.name << std::endl;
      throw CodeGenError("Type not supported!");
    }
    
    } // if derived
    
    f.f90 = f90_tmp;
    f.chdr = chdr_tmp;
    f.pxd = pxd_tmp;
    f.pyx = pyx_tmp;
    
    return f;
  }

};


std::tuple<std::string, std::string, std::string, std::string> asr_to_py(ASR::TranslationUnit_t &asr, std::string module)
{
    ASRToPyVisitor v;
    v.module_name = module;
    v.visit_asr((ASR::asr_t &)asr);
    
    return std::make_tuple(v.f90, v.chdr, v.pxd, v.pyx);
}

} // namespace LFortran
