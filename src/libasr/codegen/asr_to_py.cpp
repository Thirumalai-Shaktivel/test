#include <iostream>
#include <memory> 
#include <libasr/asr.h>
#include <libasr/containers.h>
#include <libasr/exception.h>
#include <libasr/asr_utils.h>
#include <libasr/string_utils.h>


namespace LFortran{
 
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

struct PyModVar
{  
  std::string pxd_fname;
  std::string module_name;
  
  std::string name; // variable name
  std::string base_ftype; // e.g. integer or real
  std::string bindc_type; // e.g. c_int32_t
  std::string ctype; // e.g. int32_t
  std::string nptype; // e.g. np.int32
  size_t m_storage; // allocatable, parameter, default
  size_t n_dims; // number of dimensions
  std::vector<size_t> dims; // dimensions
}; 

// Generates getter and setters for integer, real, complex of any size (e.g. single precision)
// for module variables
void intrinsic1(PyModVar v, std::string &f90_tmp, 
  std::string &chdr_tmp, std::string &pxd_tmp, std::string &pyx_tmp)
{
    
  std::string getter_name = v.module_name+"_get_"+v.name;
  std::string setter_name = v.module_name+"_set_"+v.name;
  
  
  f90_tmp =  "  subroutine "+getter_name+"(var) bind(c)\n";
  f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
  f90_tmp += "    "+v.base_ftype+"("+v.bindc_type+"), intent(out) :: var\n";
  if (v.m_storage == ASR::Allocatable){
    f90_tmp += "    if (allocated(a)) then\n";
    f90_tmp += "      var = a\n";
    f90_tmp += "    endif\n";
  }
  else{
    f90_tmp += "    var = a\n";
  }
  f90_tmp += "  end subroutine \n\n";
  
  if (v.m_storage != ASR::Parameter){
    f90_tmp += "  subroutine "+setter_name+"(var) bind(c)\n";
    f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
    f90_tmp += "    "+v.base_ftype+"("+v.bindc_type+"), intent(in) :: var\n";
    if (v.m_storage == ASR::Allocatable){
      f90_tmp += "    if (.not. allocated(a)) then\n";
      f90_tmp += "      allocate(a)\n";
      f90_tmp += "    endif\n";
    }
    f90_tmp += "    a = var\n";
    f90_tmp += "  end subroutine "+setter_name+"\n\n";
  }
  
  chdr_tmp = "void "+getter_name+"("+v.ctype+" *var);\n";
  if (v.m_storage != ASR::Parameter){
    chdr_tmp +=  "void "+setter_name+"("+v.ctype+" *var);\n";
  }
  
  pxd_tmp = "  cdef void "+getter_name+"("+v.ctype+" *var);\n";
  if (v.m_storage != ASR::Parameter){
    pxd_tmp +=  "  cdef void "+setter_name+"("+v.ctype+" *var);\n";
  }
  
  
  pyx_tmp =  "  property "+v.name+":\n";
  pyx_tmp += "    def __get__(self):\n";
  pyx_tmp += "      cdef "+v.ctype+" var\n";
  pyx_tmp += "      "+v.pxd_fname+"."+getter_name+"(&var)\n";
  pyx_tmp += "      return var\n";
  if (v.m_storage != ASR::Parameter){
    pyx_tmp += "    def __set__(self, "+v.ctype+" var):\n";
    pyx_tmp += "      "+v.pxd_fname+"."+setter_name+"(&var)\n";
  }
  pyx_tmp += "\n";
  
}

void intrinsic2(PyModVar v, std::string &f90_tmp, 
  std::string &chdr_tmp, std::string &pxd_tmp, std::string &pyx_tmp)
{
  std::string getter_name = v.module_name+"_get_"+v.name;
  std::string setter_name = v.module_name+"_set_"+v.name;
  
  
  f90_tmp =  "  subroutine "+getter_name+"(var) bind(c)\n";
  f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
  f90_tmp += "    "+v.base_ftype+"("+v.bindc_type+"), intent(out) :: var(";
  for (size_t i = 0; i < v.n_dims; i++){
    if (i == v.n_dims-1){
      f90_tmp += std::to_string(v.dims[i])+")\n";
    }
    else {
      f90_tmp += std::to_string(v.dims[i])+",";
    }
  }
  f90_tmp += "    var = a\n";
  f90_tmp += "  end subroutine \n\n";
  
  if (v.m_storage != ASR::Parameter){
    f90_tmp += "  subroutine "+setter_name+"(var) bind(c)\n";
    f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
    f90_tmp += "    "+v.base_ftype+"("+v.bindc_type+"), intent(in) :: var(";
    for (size_t i = 0; i < v.n_dims; i++){
      if (i == v.n_dims-1){
        f90_tmp += std::to_string(v.dims[i])+")\n";
      }
      else {
        f90_tmp += std::to_string(v.dims[i])+",";
      }
    }
    f90_tmp += "    a = var\n";
    f90_tmp += "  end subroutine "+setter_name+"\n\n";
  }
  
  chdr_tmp = "void "+getter_name+"("+v.ctype+" *var);\n";
  if (v.m_storage != ASR::Parameter){
    chdr_tmp +=  "void "+setter_name+"("+v.ctype+" *var);\n";
  }
  
  pxd_tmp = "  cdef void "+getter_name+"("+v.ctype+" *var);\n";
  if (v.m_storage != ASR::Parameter){
    pxd_tmp +=  "  cdef void "+setter_name+"("+v.ctype+" *var);\n";
  }
  
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
  pyx_tmp += ", "+v.nptype+")\n";
  pyx_tmp += "      "+v.pxd_fname+"."+getter_name+"(<"+v.ctype+" *>var.data)\n";
  pyx_tmp += "      return var\n";
  if (v.m_storage != ASR::Parameter){
    pyx_tmp += "    def __set__(self, ndarray["+v.ctype+", ndim="+std::to_string(v.n_dims)+"] var):\n";
    for (size_t i = 0; i < v.n_dims; i++){
      pyx_tmp += "      cdef int32_t dim"+std::to_string(i+1)+" = var.shape["+std::to_string(i)+"]\n";
    }
    pyx_tmp += "      shape = (";
    for (size_t i = 0; i < v.n_dims; i++){
      pyx_tmp += "dim"+std::to_string(i+1)+",";
    }
    pyx_tmp += ")\n";
    pyx_tmp += "      if (";
    for (size_t i = 0; i < v.n_dims; i++){
      pyx_tmp += std::to_string(v.dims[i])+",";
    }
    pyx_tmp += ") != shape:\n";
    pyx_tmp += "        raise ValueError('"+v.name+" has the wrong shape')\n";
    pyx_tmp += "      "+v.pxd_fname+"."+setter_name+"(<"+v.ctype+" *>var.data)\n";
  }
  pyx_tmp += "\n";
  
}  

void intrinsic3(PyModVar v, std::string &f90_tmp, 
    std::string &chdr_tmp, std::string &pxd_tmp, std::string &pyx_tmp){
      
    std::string getter_name = v.module_name+"_get_"+v.name;
    std::string setter_name = v.module_name+"_set_"+v.name;
    
    f90_tmp =  "  subroutine "+getter_name+"_size(";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "dim"+std::to_string(i+1);
      if (i < v.n_dims - 1){
        f90_tmp += ", ";
      }
    }
    f90_tmp += ") bind(c)\n";
    
    f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "    integer(c_int32_t), intent(out) :: dim"+std::to_string(i+1)+"\n";
    }
    f90_tmp += "    if (allocated(a)) then\n";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "      dim"+std::to_string(i+1)+" = size(a, "+std::to_string(i+1)+")\n";
    }
    f90_tmp += "    else\n";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "      dim"+std::to_string(i+1)+" = 0\n";
    }
    f90_tmp += "    endif\n";
    f90_tmp += "  end subroutine "+getter_name+"_size\n\n";  
    
    f90_tmp += "  subroutine "+getter_name+"(";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "dim"+std::to_string(i+1)+", ";
    }
    f90_tmp += "var) bind(c)\n";
    f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "    integer(c_int32_t), intent(in) :: dim"+std::to_string(i+1)+"\n";
    }
    f90_tmp += "    "+v.base_ftype+"("+v.bindc_type+"), intent(out) :: var(";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "dim"+std::to_string(i+1);
      if (i < v.n_dims - 1){
        f90_tmp += ", ";
      }
    }
    f90_tmp += ")\n";
    f90_tmp += "    if (allocated(a)) then\n";
    f90_tmp += "      var = a\n";
    f90_tmp += "    endif\n";
    f90_tmp += "  end subroutine "+getter_name+"\n\n"; 
    
    f90_tmp += "  subroutine "+setter_name+"(";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "dim"+std::to_string(i+1)+", ";
    }
    f90_tmp += "var) bind(c)\n";
    f90_tmp += "    use "+v.module_name+", only: a => "+v.name+"\n";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "    integer(c_int32_t), intent(in) :: dim"+std::to_string(i+1)+"\n";
    }
    f90_tmp += "    "+v.base_ftype+"("+v.bindc_type+"), intent(in) :: var(";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "dim"+std::to_string(i+1);
      if (i < v.n_dims - 1){
        f90_tmp += ", ";
      }
    }
    f90_tmp += ")\n";
    f90_tmp += "    if (allocated(a)) deallocate(a)\n";
    f90_tmp += "    allocate(a(";
    for (size_t i = 0; i < v.n_dims; i++){
      f90_tmp += "dim"+std::to_string(i+1);
      if (i < v.n_dims - 1){
        f90_tmp += ", ";
      }
    }
    f90_tmp += "))\n";
    f90_tmp += "    a = var\n";
    f90_tmp += "  end subroutine "+setter_name+"\n\n";
    
    chdr_tmp = "void "+getter_name+"_size(";
    for (size_t i = 0; i < v.n_dims; i++){
      chdr_tmp += "int32_t *dim"+std::to_string(i+1);
      if (i < v.n_dims - 1){
        chdr_tmp += ", ";
      }
    }
    chdr_tmp += ");\n";
    chdr_tmp += "void "+getter_name+"(";
    for (size_t i = 0; i < v.n_dims; i++){
      chdr_tmp += "int32_t *dim"+std::to_string(i+1)+", ";
    }
    chdr_tmp += v.ctype+" *var);\n";
    chdr_tmp +=  "void "+setter_name+"(";
    for (size_t i = 0; i < v.n_dims; i++){
      chdr_tmp += "int32_t *dim"+std::to_string(i+1)+", ";
    }
    chdr_tmp += v.ctype+" *var);\n";
    
    
    pxd_tmp = "  cdef void "+getter_name+"_size(";
    for (size_t i = 0; i < v.n_dims; i++){
      pxd_tmp += "int32_t *dim"+std::to_string(i+1);
      if (i < v.n_dims - 1){
        pxd_tmp += ", ";
      }
    }
    pxd_tmp += ");\n";
    pxd_tmp += "  cdef void "+getter_name+"(";
    for (size_t i = 0; i < v.n_dims; i++){
      pxd_tmp += "int32_t *dim"+std::to_string(i+1)+", ";
    }
    pxd_tmp += v.ctype+" *var);\n";
    pxd_tmp +=  "  cdef void "+setter_name+"(";
    for (size_t i = 0; i < v.n_dims; i++){
      pxd_tmp += "int32_t *dim"+std::to_string(i+1)+", ";
    }
    pxd_tmp += v.ctype+" *var);\n";
    
    pyx_tmp =  "  property "+v.name+":\n";
    pyx_tmp += "    def __get__(self):\n";
    for (size_t i = 0; i < v.n_dims; i++){
      pyx_tmp += "      cdef int32_t dim"+std::to_string(i+1)+"\n";
    }
    pyx_tmp += "      "+v.pxd_fname+"."+getter_name+"_size(";
    for (size_t i = 0; i < v.n_dims; i++){
      pyx_tmp += "&dim"+std::to_string(i+1);
      if (i < v.n_dims - 1){
        pyx_tmp += ", ";
      }
    }
    pyx_tmp += ")\n";
    pyx_tmp += "      cdef ndarray var = np.empty((";
    for (size_t i = 0; i < v.n_dims; i++){
      pyx_tmp += "dim"+std::to_string(i+1);
      if (i < v.n_dims - 1){
        pyx_tmp += ", ";
      }
    }
    pyx_tmp += ",), "+v.nptype+")\n";
    pyx_tmp += "      "+v.pxd_fname+"."+getter_name+"(";
    for (size_t i = 0; i < v.n_dims; i++){
      pyx_tmp += "&dim"+std::to_string(i+1)+", ";
    }
    pyx_tmp += "<"+v.ctype+" *>var.data)\n";
    pyx_tmp += "      return var\n";
    pyx_tmp += "    def __set__(self, ndarray["+v.ctype+", ndim="+std::to_string(v.n_dims)+"] var):\n";
    for (size_t i = 0; i < v.n_dims; i++){
      pyx_tmp += "      cdef int32_t dim"+std::to_string(i+1)+" = var.shape["+std::to_string(i)+"]\n";
    }
    pyx_tmp += "      "+v.pxd_fname+"."+setter_name+"(";
    for (size_t i = 0; i < v.n_dims; i++){
      pyx_tmp += "&dim"+std::to_string(i+1)+", ";
    }
    pyx_tmp += "<"+v.ctype+" *>var.data)\n";
    pyx_tmp += "\n";
    
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
    pyx += "cdef class __"+module_name+":\n";
    
    f90 =  "! This file was automatically generated by the LFortran compiler.\n\n";
    f90 += "module "+module_name+"_wrapper\n";
    f90 += "  use iso_c_binding\n";
    f90 += "  implicit none\n\n";
    f90 += "contains\n\n";
    
    std::vector<std::string> build_order
        = ASRUtils::determine_module_dependencies(x);
    
    // loop over stuff in tranlation unit
    for(auto item : build_order){
      ASR::symbol_t *mod = x.m_global_scope->scope[item];    
      visit_symbol(*mod);
    }
    
    f90 += "end module";
  }

  void visit_Module(const ASR::Module_t &x)
  {
    // check if module is the right one
    if (x.m_name != module_name){
      return;
    }
    // std::cout << "Module: " << x.m_name << std::endl;
    
    // loop through symbols in module
    for (auto &item : x.m_symtab->scope) {
      
      if (is_a<ASR::Variable_t>(*item.second)) {
        // variables in module
        ASR::Variable_t *s = ASR::down_cast<ASR::Variable_t>(item.second);
        visit_Variable(*s);
      }
      else{
        // functions...
      }
      
    }
    pyx += "\n"+module_name+" = __"+module_name+"()\n";
    
    // std::cout << f90 << std::endl;
    // std::cout << chdr << std::endl;
    // std::cout << pxd << std::endl;
    // std::cout << pyx << std::endl;
    
  }
  
  void visit_Variable(const ASR::Variable_t &x)
  {
    
    // All these are module variables. We must create setters and getters for
    // each of them. 
    std::string f90_tmp = "";
    std::string chdr_tmp = "";
    std::string pxd_tmp = "";
    std::string pyx_tmp = "";
    
    PyModVar v;
    
    if (x.m_access == ASR::Private){
      return;
    }

    v.name = x.m_name;
    v.module_name = module_name;
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
    else {
      // throw CodeGenError("Type not supported");
      std::cout << "Type not supported: " << v.name << std::endl;
      return;
    }
    v.pxd_fname = pxd_fname;
    
    if (v.n_dims == 0){
      std::cout << "Wrapping: " << v.name << std::endl;
      intrinsic1(v, f90_tmp, chdr_tmp, pxd_tmp, pyx_tmp);
    }
    else if (v.n_dims != 0 && v.m_storage != ASR::Allocatable){
      std::cout << "Wrapping: " << v.name << std::endl;
      intrinsic2(v, f90_tmp, chdr_tmp, pxd_tmp, pyx_tmp);
    }
    else if (v.n_dims != 0 && v.m_storage == ASR::Allocatable){
      std::cout << "Wrapping: " << v.name << std::endl;
      intrinsic3(v, f90_tmp, chdr_tmp, pxd_tmp, pyx_tmp);
    }
    else {
      // std::cout << "Skipping " << v.name << std::endl;
      throw CodeGenError("Type not supported!");
    }
    
    f90 += f90_tmp;
    chdr += chdr_tmp;
    pxd += pxd_tmp;
    pyx += pyx_tmp;
    
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
