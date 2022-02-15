#include <libasr/asr.h>
#include <libasr/containers.h>
#include <libasr/exception.h>
#include <libasr/asr_utils.h>
#include <libasr/asr_verify.h>
#include <libasr/pass/inline_functions_call.h>
#include <libasr/pass/pass_utils.h>

#include <vector>
#include <map>
#include <utility>


namespace LFortran {

using ASR::down_cast;
using ASR::is_a;

// Platform dependent fast unique hash:
uint64_t static get_hash(ASR::symbol_t *node)
{
    return (uint64_t)((ASR::asr_t*)node);
}

/*

This ASR pass replaces function calls expressions with the body of the function
itself. This helps in avoiding function call overhead in the backend code.

Converts:

    complex(4) function f(a) result(b)
    complex(4), intent(in) :: a
    b = a + 5
    end function

    c = f(a)

to:

    c = a + 5

*/
class InlineFunctionCallVisitor : public PassUtils::PassVisitor<FMAVisitor>
{
private:
    ASR::TranslationUnit_t &unit;

    std::string rl_path;

    ASR::expr_t* function_result_var;

    bool from_inline_function_call;

    std::map<uint64_t, ASR::expr_t*> arg2value;

    std::string current_routine;

public:
    InlineFunctionCallVisitor(Allocator &al_, ASR::TranslationUnit_t &unit_,
                              const std::string& rl_path_) : PassVisitor(al_),
    unit(unit_), rl_path(rl_path_), function_result_var(nullptr),
    from_inline_function_call(false), current_routine("")
    {
        pass_result.reserve(al, 1);
    }

    void visit_Function(const ASR::Function_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        ASR::Function_t &xx = const_cast<ASR::Function_t&>(x);
        current_routine = std::string(xx.m_name);
        current_scope = xx.m_symtab;
        PassUtils::PassVisitor::transform_stmts(xx.m_body, xx.n_body);
        current_routine.clear();
    }

    void visit_FunctionCall(const ASR::FunctionCall_t& x) {
        ASR::symbol_t* routine = ASRUtils::symbol_get_past_external(x.m_name);
        if( !ASR::is_a<ASR::Function_t>(*routine) ) {
            return ;
        }

        ASR::Function_t* func = ASR::down_cast<ASR::Function_t>(routine);
        if( std::string(func->m_name) == current_routine ) {
            return ;
        }

        for( auto& itr : func->m_symtab->scope ) {
            arg2value[itr.second]
        }
    }

    void visit_Assignment(const ASR::Assignment_t& x) {
        from_inline_function_call = true;
        ASR::Assignment_t& xx = const_cast<ASR::Assignment_t&>(x);
        function_result_var = nullptr;
        visit_expr(*x.m_value);
        if( function_result_var ) {
            xx.m_value = function_result_var;
        }
        function_result_var = nullptr;
        from_inline_function_call = false;
    }

};

void pass_replace_inline_function_call(Allocator &al, ASR::TranslationUnit_t &unit,
                                       const std::string& rl_path) {
    InlineFunctionCallVisitor v(al, unit, rl_path);
    v.visit_TranslationUnit(unit);
    LFORTRAN_ASSERT(asr_verify(unit));
}


} // namespace LFortran
