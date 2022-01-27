#include <libasr/asr.h>
#include <libasr/containers.h>
#include <libasr/exception.h>
#include <libasr/asr_utils.h>
#include <libasr/asr_verify.h>
#include <libasr/pass/div_to_mul.h>
#include <libasr/pass/pass_utils.h>

#include <vector>
#include <utility>


namespace LFortran {

using ASR::down_cast;
using ASR::is_a;

/*

This ASR pass replaces divison operation with multiplication
if the divisor can be evaluated to a constant at compile time.

Converts:

    real :: x
    real, parameter :: divisor = 2.0
    print *, x/divisor

to:

    real :: x
    real, parameter :: divisor = 2.0
    print *, x * 0.5

*/
class DivToMulVisitor : public PassUtils::PassVisitor<DivToMulVisitor>
{
private:
    Allocator &al;
    ASR::TranslationUnit_t &unit;

    std::string rl_path;
    Vec<ASR::stmt_t*> div_to_mul_result;

    SymbolTable* current_scope;

public:
    DivToMulVisitor(Allocator &al_, ASR::TranslationUnit_t &unit_, const std::string& rl_path_) : al(al_), unit(unit_),
    rl_path{rl_path_}
    {
        div_to_mul_result.reserve(al, 1);
    }

    // TODO: Only Program and While is processed, we need to process all calls
    // to visit_stmt().

    void visit_Program(const ASR::Program_t &x) {
        current_scope = x.m_symtab;

        // Transform nested functions and subroutines
        for (auto &item : x.m_symtab->scope) {
            if (is_a<ASR::Subroutine_t>(*item.second)) {
                ASR::Subroutine_t *s = ASR::down_cast<ASR::Subroutine_t>(item.second);
                visit_Subroutine(*s);
            }
            if (is_a<ASR::Function_t>(*item.second)) {
                ASR::Function_t *s = down_cast<ASR::Function_t>(item.second);
                visit_Function(*s);
            }
        }

        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        ASR::Program_t &xx = const_cast<ASR::Program_t&>(x);

        transform_stmts(xx.m_body, xx.n_body, al, div_to_mul_result);

    }

    void visit_Subroutine(const ASR::Subroutine_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        ASR::Subroutine_t &xx = const_cast<ASR::Subroutine_t&>(x);
        current_scope = xx.m_symtab;
        transform_stmts(xx.m_body, xx.n_body, al, div_to_mul_result);
    }

    void visit_Function(const ASR::Function_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        ASR::Function_t &xx = const_cast<ASR::Function_t&>(x);
        current_scope = xx.m_symtab;
        transform_stmts(xx.m_body, xx.n_body, al, div_to_mul_result);
    }

    void visit_BinOp(const ASR::BinOp_t& x) {
        visit_expr(*x.m_left);
        visit_expr(*x.m_right);
        if( x.m_op == ASR::binopType::Div ) {
            ASR::expr_t* right_value = ASRUtils::expr_value(x.m_right);
            if( right_value ) {
                double value;
                if( ASRUtils::extract_value<double>(right_value, value) ) {
                    bool is_feasible = false;
                    ASR::expr_t* right_inverse = nullptr;
                    switch( x.m_type->type ) {
                        case ASR::ttypeType::Real: {
                            is_feasible = true;
                            right_inverse = ASRUtils::EXPR(ASR::make_ConstantReal_t(al, x.m_right->base.loc, 1.0/value, x.m_type));
                            break;
                        }
                        default:
                            break;
                    }
                    if( is_feasible ) {
                        ASR::BinOp_t& xx = const_cast<ASR::BinOp_t&>(x);
                        xx.m_op = ASR::binopType::Mul;
                        xx.m_right = right_inverse;
                    }
                }
            }
        }
    }

};

void pass_replace_div_to_mul(Allocator &al, ASR::TranslationUnit_t &unit,
                            const std::string& rl_path) {
    DivToMulVisitor v(al, unit, rl_path);
    v.visit_TranslationUnit(unit);
    LFORTRAN_ASSERT(asr_verify(unit));
}


} // namespace LFortran
