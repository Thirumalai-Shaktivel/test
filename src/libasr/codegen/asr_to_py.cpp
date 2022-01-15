#include <iostream>
#include <memory> 
#include <libasr/asr.h>
#include <libasr/containers.h>
#include <libasr/exception.h>
#include <libasr/asr_utils.h>
#include <libasr/string_utils.h>


/*
 * 
 * This back-end generates wrapper code that allows Fortran to automatically be called from Python.
 * It also generates a C header file, so I suppose it indirectly generates C wrappers as well.
 * Currently, it outputs Cython, rather than the Python C API directly - much easier to implement.
 * The actual output files are:
 *  - a .h file, containing C-language function declarations *  
 *  - a .pxd file, basically containing the same information as the .h file, but in Cython's format.
 *  - a .pyx file, which is a Cython file that includes the actual python-callable wrapper functions.
 *
 *  Currently, this back-end only wraps functions that are marked "bind (c)" in the Fortran source.
 *  At some later point we will offer the functionality to generate bind (c) wrapper functions for
 *  normal Fortran subprograms, but for now, we don't offer this functionality.
 *
 *  --- H. Snyder, Aug 2021 
 *  
 * */


/*  
 * The following technique is called X-macros, if you don't recognize it.
 * You should be able to look it up under that name for an explanation.
 */
namespace LFortran{
 
 
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
  std::string cythontype; // e.g. int32_t
  std::string nptype; //
  size_t m_storage; // allocatable, parameter, default
  size_t n_dims; // number of dimensions
  std::vector<size_t> dims; // dimensions
}; 

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
    
    chdr_tmp = "void "+getter_name+"("+v.ctype+" *var);\n";
    chdr_tmp +=  "void "+setter_name+"("+v.ctype+" *var);\n";
    
    pxd_tmp = "  cdef void "+getter_name+"("+v.ctype+" *var);\n";
    pxd_tmp +=  "  cdef void "+setter_name+"("+v.ctype+" *var);\n";

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
      pyx_tmp += "dim"+std::to_string(i+1)+", ";
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
 
}

#define CTYPELIST \
    _X(ASR::Integer_t, 1, "int8_t" ) \
    _X(ASR::Integer_t, 2, "int16_t" ) \
    _X(ASR::Integer_t, 4, "int32_t" ) \
    _X(ASR::Integer_t, 8, "int64_t" ) \
    \
    _X(ASR::Real_t,    4, "float" ) \
    _X(ASR::Real_t,    8, "double" ) \
    \
    _X(ASR::Complex_t, 4, "float _Complex" ) \
    _X(ASR::Complex_t, 8, "double _Complex" ) \
    \
    _X(ASR::Logical_t, 1,   "_Bool" ) \
    _X(ASR::Character_t, 1, "char" ) 


/*
 * We will use this list instead, once the ASR has symbolic kind information.

#define CTYPELIST_FUTURE \
    _X(ASR::Integer_t, "c_int",           "int"   ) \
    _X(ASR::Integer_t, "c_short",         "short" ) \
    _X(ASR::Integer_t, "c_long",          "long"  ) \
    _X(ASR::Integer_t, "c_long_long",     "long long" ) \
    _X(ASR::Integer_t, "c_signed_char",   "signed char" ) \
    _X(ASR::Integer_t, "c_size_t",        "size_t" ) \
    \
    _X(ASR::Integer_t, "c_int8_t",        "int8_t" ) \
    _X(ASR::Integer_t, "c_int16_t",       "int16_t" ) \
    _X(ASR::Integer_t, "c_int32_t",       "int32_t" ) \
    _X(ASR::Integer_t, "c_int64_t",       "int64_t" ) \
    \
    _X(ASR::Integer_t, "c_int_least8_t",  "int_least8_t" ) \
    _X(ASR::Integer_t, "c_int_least16_t", "int_least16_t" ) \
    _X(ASR::Integer_t, "c_int_least32_t", "int_least32_t" ) \
    _X(ASR::Integer_t, "c_int_least64_t", "int_least64_t" ) \
    \
    _X(ASR::Integer_t, "c_int_fast8_t",   "int_fast8_t" ) \
    _X(ASR::Integer_t, "c_int_fast16_t",  "int_fast16_t" ) \
    _X(ASR::Integer_t, "c_int_fast32_t",  "int_fast32_t" ) \
    _X(ASR::Integer_t, "c_int_fast64_t",  "int_fast64_t" ) \
    \
    _X(ASR::Integer_t, "c_intmax_t",      "intmax_t" ) \
    _X(ASR::Integer_t, "c_intptr_t",      "intptr_t" ) \
    _X(ASR::Integer_t, "c_ptrdiff_t",     "ptrdiff_t" ) \
    \
    _X(ASR::Real_t,    "c_float",         "float" ) \
    _X(ASR::Real_t,    "c_double",        "double" ) \
    _X(ASR::Real_t,    "c_long_double",   "long double" ) \
    \
    _X(ASR::Complex_t, "c_float_complex", "float _Complex" ) \
    _X(ASR::Complex_t, "c_double_complex", "double _Complex" ) \
    _X(ASR::Complex_t, "c_long_double_complex", "long double _Complex" ) \
    \
    _X(ASR::Logical_t, "c_bool",          "_Bool" ) \
    _X(ASR::Character_t, "c_char",        "char" ) 
 */ 

namespace LFortran {

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
    // These store the strings that will become the contents of the generated .h, .pxd, .pyx files
    std::string chdr, pxd, pyx;

    // Stores the name of the current module being visited.
    // Value is meaningless after calling ASRToPyVisitor::visit_asr.
    std::string cur_module;

    // Are we assuming arrays to be in C order (row-major)? If not, assume Fortran order (column-major).
    bool c_order;

    // What's the file name of the C header file we're going to generate? (needed for the .pxd)
    std::string chdr_filename;
    // What's the name of the pxd file (minus the .pxd extension)
    std::string pxdf;

    ASRToPyVisitor(bool c_order_, std::string chdr_filename_) : 
            c_order(c_order_), 
            chdr_filename(chdr_filename_),
            pxdf(chdr_filename_)
    {
        // we need to get get the pxd filename (minus extension), so we can import it in the pyx file
        // knock off ".h" from the c header filename
        pxdf.erase(--pxdf.end());
        pxdf.erase(--pxdf.end());   
        // this is an unfortuante hack, but we have to add something so that the pxd and pyx filenames
        // are different (beyond just their extensions). If we don't, the cython emits a warning.
        // TODO we definitely need to change this somehow because right now this "append _pxd" trick
        // exists in two places (bin/lfortran.cpp, and here), which could easily cause breakage.
        pxdf += "_pxd";
    }

    std::tuple<std::string, std::string, std::string, std::string, std::string>
    helper_visit_arguments(size_t n_args, ASR::expr_t ** args)
    {

        struct arg_info {
            ASR::Variable_t*  asr_obj;
            std::string       ctype;
            int               ndims;
            
            std::vector<std::string> ubound_varnames;
            std::vector<std::pair<std::string,int> > i_am_ubound_of;
        };

        std::vector<arg_info> arg_infos;


        /* get_arg_infos */ for (size_t i=0; i<n_args; i++) {

            ASR::Variable_t *arg = ASRUtils::EXPR2VAR(args[i]);
            LFORTRAN_ASSERT(ASRUtils::is_arg_dummy(arg->m_intent));

            // TODO add support for (or emit error on) assumed-shape arrays
            // TODO add support for interoperable derived types

            arg_info this_arg_info;

            const char * errmsg1 = "pywrap does not yet support array dummy arguments with lower bounds other than 1.";
            const char * errmsg2 = "pywrap can only generate wrappers for array dummy arguments " 
                                   "if the upper bound is a constant integer, or another (scalar) dummy argument.";

            // Generate a sequence of if-blocks to determine the type, using the type list defined above
            #define _X(ASR_TYPE, KIND, CTYPE_STR) \
            if ( is_a<ASR_TYPE>(*arg->m_type) && (down_cast<ASR_TYPE>(arg->m_type)->m_kind == KIND) ) { \
                this_arg_info.asr_obj = arg;                                                       \
                this_arg_info.ctype   = CTYPE_STR;                                                 \
                auto tmp_arg          = down_cast<ASR_TYPE>(arg->m_type);                          \
                this_arg_info.ndims   = tmp_arg->n_dims;                                           \
                for (int j = 0; j < this_arg_info.ndims; j++) {                                    \
                    auto lbound_ptr = tmp_arg->m_dims[j].m_start;                                  \
                    if (!is_a<ASR::ConstantInteger_t>(*lbound_ptr)) {                              \
                        throw CodeGenError(errmsg1);                                               \
                    }                                                                              \
                    if (down_cast<ASR::ConstantInteger_t>(lbound_ptr)->m_n != 1) {                 \
                        throw CodeGenError(errmsg1);                                               \
                    }                                                                              \
                    if (is_a<ASR::Var_t>(*tmp_arg->m_dims[j].m_end)) {                             \
                        ASR::Variable_t *dimvar = ASRUtils::EXPR2VAR(tmp_arg->m_dims[j].m_end);    \
                        this_arg_info.ubound_varnames.push_back(dimvar->m_name);                   \
                    } else if (!is_a<ASR::ConstantInteger_t>(*lbound_ptr)) {                       \
                        throw CodeGenError(errmsg2);                                               \
                    }                                                                              \
                }                                                                                  \
            } else       

            CTYPELIST {
                // We end up in this block if none of the above if-blocks were triggered
                throw CodeGenError("Type not supported");
            };
            #undef _X

            arg_infos.push_back(this_arg_info);

        } /* get_arg_infos */


        /* mark_array_bound_vars */ for(auto arg_iter = arg_infos.begin(); arg_iter != arg_infos.end(); arg_iter++) {

            /* some dummy args might just be the sizes of other dummy args, e.g.:

            subroutine foo(n,x)
                integer :: n, x(n)
            end subroutine  

            We don't actually want `n` in the python wrapper's arguments - the Python programmer
            shouldn't need to explicitly pass sizes. From the get_arg_infos block, we already have
            the mapping from `x` to `n`, but we also need the opposite - we need be able to look at 
            `n` and know that it's related to `x`. So let's do a pass over the arg_infos list and 
            assemble that information.

            */
            
            for (auto bound_iter =  arg_iter->ubound_varnames.begin(); 
                      bound_iter != arg_iter->ubound_varnames.end(); 
                      bound_iter++ ) {
                for (unsigned int j = 0; j < arg_infos.size(); j++) {
                    if (0 == std::string(arg_infos[j].asr_obj->m_name).compare(*bound_iter)) {
                        arg_infos[j].i_am_ubound_of.push_back(std::make_pair(arg_iter->asr_obj->m_name, j));
                    }
                }
            }

        } /* mark_array_bound_vars */


        /* apply_c_order */ if(c_order) {

            for(auto arg_iter = arg_infos.begin(); arg_iter != arg_infos.end(); arg_iter++) {
            
                for (auto bound =  arg_iter->i_am_ubound_of.begin();
                          bound != arg_iter->i_am_ubound_of.end();
                          bound++) {
                    auto x = std::make_pair(bound->first, - bound->second -1);
                    bound->swap(x);
                }  
            
            }

        } /* apply_c_order */

        std::string c, cyargs, fargs, pyxbody, return_statement;

        /* build_return_strings */ for(auto it = arg_infos.begin(); it != arg_infos.end(); it++) {
           
            std::string c_wip, cyargs_wip, fargs_wip, rtn_wip;          

            c_wip = it->ctype;

            // Get type for cython wrapper argument, from the C type name
            if (it->ndims > 0) {
                std::string mode     = c_order   ? ", mode=\"c\"" : ", mode=\"fortran\"";
                std::string strndims = it->ndims > 1 ? ", ndim="+std::to_string(it->ndims) : "";
                cyargs_wip += "ndarray[" + it->ctype + strndims + mode + "]";
            } else {
                cyargs_wip += it->ctype;
            }

            // Fortran defaults to pass-by-reference, so the C argument is a pointer, unless
            // it is not an array AND it has the value type.
            if (it->ndims > 0 || !it->asr_obj->m_value_attr) {
                c_wip += " *";
                fargs_wip = "&";
                // If the argument is intent(in) and a pointer, it should be a ptr-to-const.
                if (ASR::intentType::In == it->asr_obj->m_intent) c_wip = "const " + c_wip;
            }

            c_wip += " ";
            c_wip += it->asr_obj->m_name;

            cyargs_wip += " ";
            cyargs_wip += it->asr_obj->m_name;

            fargs_wip += it->asr_obj->m_name;
            if(it->ndims > 0) {
                fargs_wip += "[0";
                for(int h = 1; h < it->ndims; h++) 
                    fargs_wip += ",0";
                fargs_wip += "]";
            }

            if (ASR::intentType::Out == it->asr_obj->m_intent ||
                ASR::intentType::InOut == it->asr_obj->m_intent) {
                rtn_wip = it->asr_obj->m_name;
            }

             
            if(!it->i_am_ubound_of.empty()) {
                cyargs_wip.clear();
                auto& i_am_ubound_of = it->i_am_ubound_of[0];
                pyxbody += "    cdef " + it->ctype + " "; 
                pyxbody += it->asr_obj->m_name;
                pyxbody += " = ";
                pyxbody += i_am_ubound_of.first + ".shape[" + std::to_string(i_am_ubound_of.second) + "]\n";
                for(unsigned int k = 1; k < it->i_am_ubound_of.size(); k++) {
                    auto& i_am_ubound_of_k = it->i_am_ubound_of[k];
                    pyxbody += "    assert(" + i_am_ubound_of_k.first + ".shape[" + std::to_string(i_am_ubound_of_k.second) + "] == "
                                             + i_am_ubound_of.first   + ".shape[" + std::to_string(i_am_ubound_of.second)   + "])\n";
                }
            }

            if(!c.empty()       && !c_wip.empty())      c += ", ";
            if(!fargs.empty()   && !fargs_wip.empty())  fargs += ", ";
            if(!cyargs.empty()  && !cyargs_wip.empty()) cyargs += ", ";
            if(!return_statement.empty() && !rtn_wip.empty()) return_statement += ", ";

            c      += c_wip;
            fargs  += fargs_wip;
            cyargs += cyargs_wip;
            return_statement += rtn_wip;
            

        } /* build_return_strings */

        return std::make_tuple(c, cyargs, fargs, pyxbody, return_statement);
    }

    void visit_TranslationUnit(const ASR::TranslationUnit_t &x) {
        // All loose statements must be converted to a function, so the items
        // must be empty:
        LFORTRAN_ASSERT(x.n_items == 0);

        std::string chdr_tmp ;
        std::string pxd_tmp  ;
        std::string pyx_tmp  ;

        chdr_tmp =  "// This file was automatically generated by the LFortran compiler.\n";
        chdr_tmp += "// Editing by hand is discouraged.\n\n";
        chdr_tmp += "#include <stdint.h>\n\n";

        pxd_tmp =   "# This file was automatically generated by the LFortran compiler.\n";
        pxd_tmp +=  "# Editing by hand is discouraged.\n\n";
        pxd_tmp +=  "from libc.stdint cimport int8_t, int16_t, int32_t, int64_t\n";
        pxd_tmp +=  "cdef extern from \"" + chdr_filename + "\":\n";

        
        pyx_tmp =  "# This file was automatically generated by the LFortran compiler.\n";
        pyx_tmp += "# Editing by hand is discouraged.\n\n";
        pyx_tmp += "from numpy cimport import_array, ndarray, int8_t, int16_t, int32_t, int64_t\n";
        pyx_tmp += "from numpy import empty, int8, int16, int32, int64\n";
        pyx_tmp += "cimport " + pxdf + " \n\n";

        // Process loose procedures first
        for (auto &item : x.m_global_scope->scope) {
            if (is_a<ASR::Function_t>(*item.second)
                    || is_a<ASR::Subroutine_t>(*item.second)) {
                visit_symbol(*item.second);

                chdr_tmp += chdr;
                pxd_tmp  += pxd;
                pyx_tmp  += pyx;
            }
        }

        // Then do all the modules in the right order
        std::vector<std::string> build_order
            = ASRUtils::determine_module_dependencies(x);
        for (auto &item : build_order) {
            LFORTRAN_ASSERT(x.m_global_scope->scope.find(item)
                    != x.m_global_scope->scope.end());
            if (!startswith(item, "lfortran_intrinsic")) {
                ASR::symbol_t *mod = x.m_global_scope->scope[item];
                visit_symbol(*mod);

                chdr_tmp += chdr;
                pxd_tmp  += pxd;
                pyx_tmp  += pyx;
            }
        }

        // There's no need to process the `program` statement, which 
        // is the only other thing that can appear at the top level.

        chdr = chdr_tmp;
        pyx  = pyx_tmp;
        pxd  = pxd_tmp;
    }

    void visit_Module(const ASR::Module_t &x) {
        cur_module = x.m_name;

        // Generate code for nested subroutines and functions first:
        std::string chdr_tmp ;
        std::string pxd_tmp  ;
        std::string pyx_tmp  ;

        for (auto &item : x.m_symtab->scope) {
            if (is_a<ASR::Subroutine_t>(*item.second)) {
                ASR::Subroutine_t *s = ASR::down_cast<ASR::Subroutine_t>(item.second);
                visit_Subroutine(*s);

                chdr_tmp += chdr;
                pxd_tmp  += pxd;
                pyx_tmp  += pyx;
            }
            if (is_a<ASR::Function_t>(*item.second)) {
                ASR::Function_t *s = ASR::down_cast<ASR::Function_t>(item.second);
                visit_Function(*s);

                chdr_tmp += chdr;
                pxd_tmp  += pxd;
                pyx_tmp  += pyx;
            }
        }


        chdr = chdr_tmp;
        pyx  = pyx_tmp;
        pxd  = pxd_tmp;

        cur_module.clear();
    }

    void visit_Subroutine(const ASR::Subroutine_t &x) {

        // Only process bind(c) subprograms for now
        if (x.m_abi != ASR::abiType::BindC) return;

        // Return type and function name
        bool bindc_name_not_given = x.m_bindc_name == NULL || !strcmp("",x.m_bindc_name);
        std::string effective_name = bindc_name_not_given ? x.m_name : x.m_bindc_name;

        chdr = "void " + effective_name + " (";

        std::string c_args, cy_args, call_args, pyx_body, rtn_statement;       
        std::tie(c_args,cy_args,call_args,pyx_body,rtn_statement) = helper_visit_arguments(x.n_args, x.m_args);

        if (!rtn_statement.empty()) rtn_statement = "    return " + rtn_statement;
        
        chdr += c_args + ")";
        pxd = "    " + chdr + "\n";
        chdr += ";\n" ;

        pyx = "def " + effective_name + " (" + cy_args + "):\n";
        pyx += pyx_body;
        pyx += "    " + pxdf +"."+ effective_name + " (" + call_args + ")\n";
        pyx += rtn_statement + "\n\n";
    }


    void visit_Function(const ASR::Function_t &x) {

        // Only process bind(c) subprograms for now
        if (x.m_abi != ASR::abiType::BindC) return;

        // Return type and function name
        bool bindc_name_not_given = x.m_bindc_name == NULL || !strcmp("",x.m_bindc_name);
        std::string effective_name = bindc_name_not_given ? x.m_name : x.m_bindc_name;
        
        ASR::Variable_t *rtnvar = ASRUtils::EXPR2VAR(x.m_return_var);
        std::string rtnvar_type;
        #define _X(ASR_TYPE, KIND, CTYPE_STR) \
        if ( is_a<ASR_TYPE>(*rtnvar->m_type) && (down_cast<ASR_TYPE>(rtnvar->m_type)->m_kind == KIND) ) { \
            rtnvar_type = CTYPE_STR;                                                                     \
        } else  

        CTYPELIST { 
            throw CodeGenError("Unrecognized or non-interoperable return type/kind"); 
        }
        #undef _X
        std::string rtnvar_name = effective_name + "_rtnval__";

        chdr = rtnvar_type + " " + effective_name + " (";

        std::string c_args, cy_args, call_args, pyx_body, rtn_statement;       
        std::tie(c_args,cy_args,call_args,pyx_body,rtn_statement) = helper_visit_arguments(x.n_args, x.m_args);

        std::string rtnarg_str =  rtnvar_name;
        if(!rtn_statement.empty()) rtnarg_str += ", ";
        rtn_statement = "    return " + rtnarg_str  + rtn_statement;
        
        chdr += c_args + ")";
        pxd = "    " + chdr + "\n";
        chdr += ";\n" ;

        pyx = "def " + effective_name + " (" + cy_args + "):\n";
        pyx += pyx_body;
        pyx += "    cdef " + rtnvar_type + " " + rtnvar_name  + " = " + pxdf +"."+ effective_name + " (" + call_args + ")\n";
        pyx += rtn_statement + "\n\n";

    }

};

struct CVariable{
  
};

class ASRToPyVisitor1 : public ASR::BaseVisitor<ASRToPyVisitor1>
{
public:
  
  std::string module_name; // module to be wrapped
  
  std::string pyx; // Cython implementation file (.pyx)
  std::string pxd; // Cython header file (.pxd)
  std::string f90; // Fortran wrapper file (.f90)
  std::string chdr; // C header file for Fortran wrapper (.h)
  
  std::string pyx_fname = "pyx_fname";
  std::string pxd_fname = "pxd_fname";
  std::string f90_fname = "f90_fname";
  std::string chdr_fname = "chdr_fname";
  
  void visit_TranslationUnit(const ASR::TranslationUnit_t &x){
    
    chdr =  "// This file was automatically generated by the LFortran compiler.\n";
    chdr += "// Editing by hand is discouraged.\n\n";
    chdr += "#include <stdint.h>\n\n";
    
    pxd =   "# This file was automatically generated by the LFortran compiler.\n";
    pxd +=  "# Editing by hand is discouraged.\n\n";
    pxd +=  "from libc.stdint cimport int8_t, int16_t, int32_t, int64_t\n\n";
    pxd +=  "cdef extern from \""+chdr_fname+".h\":\n";
    
    pyx =  "# This file was automatically generated by the LFortran compiler.\n";
    pyx += "# Editing by hand is discouraged.\n\n";
    pyx += "from numpy cimport ndarray, int8_t, int16_t, int32_t, int64_t\n";
    pyx += "import numpy as np\n";
    pyx += "cimport "+pxd_fname+" \n\n";  
    pyx += "cdef class __"+module_name+":\n";
    
    f90 =  "! This file was automatically generated by the LFortran compiler.\n";
    f90 += "! Editing by hand is discouraged.\n\n";
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
    v.cythontype =  v.ctype;
    
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
      std::cout << "Skipping " << v.name << std::endl;
      return;
    }
    
    f90 += f90_tmp;
    chdr += chdr_tmp;
    pxd += pxd_tmp;
    pyx += pyx_tmp;
    
  }

};


std::tuple<std::string, std::string, std::string> asr_to_py(ASR::TranslationUnit_t &asr, bool c_order, std::string chdr_filename)
{
    ASRToPyVisitor v (c_order, chdr_filename);
    v.visit_asr((ASR::asr_t &)asr);

    return std::make_tuple(v.chdr, v.pxd, v.pyx);
}

std::tuple<std::string, std::string, std::string, std::string> asr_to_py1(ASR::TranslationUnit_t &asr, std::string module)
{
    ASRToPyVisitor1 v;
    v.module_name = module;
    v.visit_asr((ASR::asr_t &)asr);
    
    return std::make_tuple(v.f90, v.chdr, v.pxd, v.pyx);
}

} // namespace LFortran
