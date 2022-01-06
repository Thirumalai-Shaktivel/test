#ifndef LFORTRAN_PASS_UTILS_H
#define LFORTRAN_PASS_UTILS_H

#include <libasr/asr.h>
#include <libasr/containers.h>

namespace LFortran {

    namespace PassUtils {

        bool is_array(ASR::expr_t* x);

        void get_dim_rank(ASR::ttype_t* x_type, ASR::dimension_t*& m_dims, int& n_dims);

        ASR::ttype_t* set_dim_rank(ASR::ttype_t* x_type, ASR::dimension_t*& m_dims, int& n_dims,
                                    bool create_new=false, Allocator* al=nullptr);

        int get_rank(ASR::expr_t* x);

        ASR::expr_t* create_array_ref(ASR::expr_t* arr_expr, Vec<ASR::expr_t*>& idx_vars, Allocator& al);

        ASR::expr_t* create_array_ref(ASR::symbol_t* arr, Vec<ASR::expr_t*>& idx_vars, Allocator& al,
                                      const Location& loc, ASR::ttype_t* _type);

        void create_idx_vars(Vec<ASR::expr_t*>& idx_vars, int n_dims, const Location& loc,
                             Allocator& al, SymbolTable*& current_scope, std::string suffix="_k");

        ASR::expr_t* get_bound(ASR::expr_t* arr_expr, int dim, std::string bound,
                                Allocator& al, ASR::TranslationUnit_t& unit,
                                SymbolTable*& current_scope);


        ASR::expr_t* get_flipsign(ASR::expr_t* arg0, ASR::expr_t* arg1,
                                  Allocator& al, ASR::TranslationUnit_t& unit,
                                  SymbolTable*& current_scope);

        ASR::expr_t* to_int32(ASR::expr_t* x, ASR::ttype_t* int32type, Allocator& al);

        bool is_slice_present(const ASR::ArrayRef_t& x);

        bool is_slice_present(const ASR::expr_t* x);

    }

} // namespace LFortran

#endif // LFORTRAN_PASS_UTILS_H
