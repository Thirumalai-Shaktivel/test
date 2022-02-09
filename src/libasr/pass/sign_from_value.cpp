#include <libasr/asr.h>
#include <libasr/containers.h>
#include <libasr/exception.h>
#include <libasr/asr_utils.h>
#include <libasr/asr_verify.h>
#include <libasr/pass/sign_from_value.h>
#include <libasr/pass/pass_utils.h>

#include <vector>
#include <utility>


namespace LFortran {

using ASR::down_cast;
using ASR::is_a;

/*

This ASR pass converts assigning of sign from a value to
different value with a call to internal optimization routine.
This allows backend specific code generation for better performance.

Converts:

    real :: a, b, c
    c = a*sign(1.0, b)

to:

    c = sign_from_value(a, b)

*/
class SignFromValueVisitor : public PassUtils::SkipOptimizationFunctionVisitor<SignFromValueVisitor>
{
private:
    ASR::TranslationUnit_t &unit;

    std::string rl_path;

    ASR::expr_t* sign_from_value_var;

    // To make sure that SignFromValue is applied only for
    // the nodes implemented in this class
    bool from_sign_from_value;

public:
    SignFromValueVisitor(Allocator &al_, ASR::TranslationUnit_t &unit_, const std::string& rl_path_) : SkipOptimizationFunctionVisitor(al_),
    unit(unit_), rl_path(rl_path_), sign_from_value_var(nullptr), from_sign_from_value(false)
    {
        pass_result.reserve(al, 1);
    }

    void visit_BinOp(const ASR::BinOp_t& x_const) {
        if( !from_sign_from_value ) {
            return ;
        }

        from_sign_from_value = true;
        ASR::BinOp_t& x = const_cast<ASR::BinOp_t&>(x_const);

        sign_from_value_var = nullptr;
        visit_expr(*x.m_left);
        if( sign_from_value_var ) {
            x.m_left = sign_from_value_var;
        }

        sign_from_value_var = nullptr;
        visit_expr(*x.m_right);
        if( sign_from_value_var ) {
            x.m_right = sign_from_value_var;
        }
        sign_from_value_var = nullptr;

        if( x.m_op != ASR::binopType::Mul ) {
            return ;
        }

        ASR::expr_t *first_arg = nullptr, *second_arg = nullptr;

        sign_from_value_var = PassUtils::get_sign_from_value(first_arg, second_arg,
                                     al, unit, rl_path, current_scope, x.base.base.loc,
                                     [&](const std::string &msg, const Location &) { throw LFortranException(msg); });
        from_sign_from_value = false;
    }

    void visit_Assignment(const ASR::Assignment_t& x) {
        from_sign_from_value = true;
        ASR::Assignment_t& xx = const_cast<ASR::Assignment_t&>(x);
        sign_from_value_var = nullptr;
        visit_expr(*x.m_value);
        if( sign_from_value_var ) {
            xx.m_value = sign_from_value_var;
        }
        sign_from_value_var = nullptr;
        from_sign_from_value = false;
    }

};

void pass_replace_sign_from_value(Allocator &al, ASR::TranslationUnit_t &unit,
                            const std::string& rl_path) {
    SignFromValueVisitor v(al, unit, rl_path);
    v.visit_TranslationUnit(unit);
    LFORTRAN_ASSERT(asr_verify(unit));
}


} // namespace LFortran
