#include <lfortran/asr.h>
#include <lfortran/containers.h>
#include <lfortran/exception.h>
#include <lfortran/asr_utils.h>
#include <lfortran/asr_verify.h>
#include <lfortran/pass/param_to_const.h>


namespace LFortran {

using ASR::down_cast;
using ASR::is_a;

/*

This ASR pass replaces initializer expressions with evaluated values. The function
`pass_replace_param_to_const` transforms the ASR tree in-place.

Converts:

    integer, parameter :: i = 20
    integer :: a = i**2 + i

to:

    integer, parameter :: i = 20
    integer :: a = 20**2 + 20

*/

class VarVisitor : public ASR::BaseWalkVisitor<VarVisitor>
{
private:
    Allocator &al;

public:

    VarVisitor(Allocator &al) : al{al} {
    }

    // TODO: Only Program and While is processed, we need to process all calls
    // to visit_stmt().

    void visit_Program(const ASR::Program_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        for (auto &item : x.m_symtab->scope) {
            if (is_a<ASR::Variable_t>(*item.second)) {
                ASR::Variable_t *v = down_cast<ASR::Variable_t>(item.second);
                visit_Variable(*v);
            }
        }
    }

    void visit_Variable(const ASR::Variable_t& x) {
        ASR::expr_t*& init_expr = const_cast<ASR::expr_t*&>(x.m_value); 
        switch( init_expr->type ) {
            case ASR::exprType::Var: {
                ASR::Var_t* _init_var = (ASR::Var_t*)(&(init_expr->base));
                ASR::Variable_t *init_var = ASR::down_cast<ASR::Variable_t>(symbol_get_past_external(_init_var->m_v));
                if( init_var->m_storage == ASR::storage_typeType::Parameter ) {
                    switch( init_var->m_value->type ) {
                        case ASR::exprType::ConstantInteger: 
                        case ASR::exprType::ConstantReal:
                        case ASR::exprType::ConstantComplex:
                        case ASR::exprType::ConstantLogical: {
                            init_expr = init_var->m_value;
                            break;
                        }
                        default: {
                            this->visit_expr(*(init_var->m_value));
                            init_expr = init_var->m_value;
                        }
                    }
                }
                break;
            }
            default:
                break;
        }
    }
};

void pass_replace_param_to_const(Allocator &al, ASR::TranslationUnit_t &unit) {
    VarVisitor v(al);
    // Each call transforms only one layer of nested loops, so we call it twice
    // to transform doubly nested loops:
    v.visit_TranslationUnit(unit);
    v.visit_TranslationUnit(unit);
    LFORTRAN_ASSERT(asr_verify(unit));
}

} // namespace LFortran
