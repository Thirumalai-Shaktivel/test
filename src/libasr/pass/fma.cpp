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

    ASR::expr_t* fma_var;

public:
    FMAVisitor(Allocator &al_, ASR::TranslationUnit_t &unit_, const std::string& rl_path_) : PassVisitor(al_, nullptr), unit(unit_),
    rl_path(rl_path_), count(0), fma_var(nullptr)
    {
        pass_result.reserve(al, 1);
    }

    void visit_BinOp(const ASR::BinOp_t& x_const) {
        ASR::BinOp_t& x = const_cast<ASR::BinOp_t&>(x_const);

        fma_var = nullptr;
        visit_expr(*x.m_left);
        if( fma_var ) {
            x.m_left = fma_var;
        }

        fma_var = nullptr;
        visit_expr(*x.m_right);
        if( fma_var ) {
            x.m_right = fma_var;
        }
        fma_var = nullptr;

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
        } else {
            return ;
        }

        if( is_other_expr_negative ) {
            other_expr = ASRUtils::EXPR(ASR::make_UnaryOp_t(al, other_expr->base.loc,
                            ASR::unaryopType::USub, other_expr, ASRUtils::expr_type(other_expr),
                            nullptr));
        }

        if( !ASR::is_a<ASR::Var_t>(*other_expr) ) {
            std::string name = "~fma@" + std::to_string(count);
            ASR::stmt_t* assign_stmt;
            other_expr = PassUtils::create_auxiliary_variable_for_expr(other_expr, name, al, current_scope, assign_stmt);
            pass_result.push_back(al, assign_stmt);
            count += 1;
        }

        ASR::BinOp_t* mul_binop = ASR::down_cast<ASR::BinOp_t>(mul_expr);
        ASR::expr_t *first_arg = mul_binop->m_left, *second_arg = mul_binop->m_right;

        if( is_mul_expr_negative ) {
            first_arg = ASRUtils::EXPR(ASR::make_UnaryOp_t(al, first_arg->base.loc,
                            ASR::unaryopType::USub, first_arg, ASRUtils::expr_type(first_arg),
                            nullptr));
        }

        if( !ASR::is_a<ASR::Var_t>(first_arg) ) {
            std::string name = "~fma_arg@" + std::to_string(count);
            ASR::stmt_t* assign_stmt;
            first_arg = PassUtils::create_auxiliary_variable_for_expr(first_arg, name, al, current_scope, assign_stmt);
            pass_result.push_back(al, assign_stmt);
            count += 1;
        }

        if( !ASR::is_a<ASR::Var_t>(second_arg) ) {
            std::string name = "~fma_arg@" + std::to_string(count);
            ASR::stmt_t* assign_stmt;
            second_arg = PassUtils::create_auxiliary_variable_for_expr(second_arg, name, al, current_scope, assign_stmt);
            pass_result.push_back(al, assign_stmt);
            count += 1;
        }

        ASR::stmt_t* fma_op;
        fma_var = PassUtils::get_fma(other_expr, first_arg, second_arg,
                                     al, unit, rl_path, current_scope, fma_op, count,
                                     [&](const std::string &msg, const Location &) { throw LFortranException(msg); });
        count += 1;
        pass_result.push_back(al, fma_op);
    }

};

void pass_replace_fma(Allocator &al, ASR::TranslationUnit_t &unit,
                            const std::string& rl_path) {
    FMAVisitor v(al, unit, rl_path);
    v.visit_TranslationUnit(unit);
    LFORTRAN_ASSERT(asr_verify(unit));
}


} // namespace LFortran