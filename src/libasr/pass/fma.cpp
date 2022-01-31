#include <libasr/asr.h>
#include <libasr/containers.h>
#include <libasr/exception.h>
#include <libasr/asr_utils.h>
#include <libasr/asr_verify.h>
#include <libasr/pass/fma.h>
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
class FMAVisitor : public PassUtils::PassVisitor<FMAVisitor>
{
private:
    ASR::TranslationUnit_t &unit;

    std::string rl_path;

    size_t count;

public:
    FMAVisitor(Allocator &al_, ASR::TranslationUnit_t &unit_, const std::string& rl_path_) : PassVisitor(al_, nullptr), unit(unit_),
    rl_path(rl_path_), count(0)
    {
        pass_result.reserve(al, 1);
    }

    void visit_BinOp(const ASR::BinOp_t& x) {
        visit_expr(*x.m_left);
        visit_expr(*x.m_right);
        if( x.m_op == ASR::binopType::Mul || x.m_op == ASR::binopType::Div ) {
            return ;
        }
        ASR::expr_t *mul_expr = nullptr, *other_expr = nullptr;
        bool is_mul_expr_negative = false, is_other_expr_negative = false;
        if( is_BinOpMul(x.m_right) ) {
            mul_expr = x.m_right;
            other_expr = x.m_left;
            is_mul_expr_negative = (x.m_op == ASR::binopType::Sub);
        } else if( is_BinOpMul(x.m_left) ) {
            mul_expr = x.m_left;
            other_expr = x.m_right;
            is_other_expr_negative = (x.m_op == ASR::binopType::Sub);
        }
        if( !ASR::is_a<ASR::Var_t>(*other_expr) ) {
            std::string name = "~fma@" + std::to_string(count);
            other_expr = ASRUtils::create_auxiliary_variable_for_expr(other_expr, name, al, current_scope);
            count += 1;
        }
        if( !ASR::is_a<ASR::Var_t>(*mul_expr->) )
    }

};

void pass_replace_fma(Allocator &al, ASR::TranslationUnit_t &unit,
                            const std::string& rl_path) {
    FMAVisitor v(al, unit, rl_path);
    v.visit_TranslationUnit(unit);
    LFORTRAN_ASSERT(asr_verify(unit));
}


} // namespace LFortran
