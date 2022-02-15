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

    std::map<uint64_t, ASR::symbol_t*> arg2value;

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

    void visit_Var(const ASR::Var_t& x) {
        ASR::Var_t& xx = const_cast<ASR::Var_t&>(x);
        if( arg2value.find(get_hash(x.m_v)) != arg2value.end() ) {
            xx.m_v = arg2value[get_hash(x.m_v)];
        }
    }

    void visit_FunctionCall(const ASR::FunctionCall_t& x) {
        if( !from_inline_function_call ) {
            return ;
        }
        ASR::symbol_t* routine = x.m_name;
        if( ASR::is_a<ASR::ExternalSymbol_t>(*routine) ||
            !ASR::is_a<ASR::Function_t>(*routine) ) {
            return ;
        }

        ASR::Function_t* func = ASR::down_cast<ASR::Function_t>(routine);
        if( std::string(func->m_name) == current_routine ) {
            return ;
        }

        ASR::expr_t* return_var = nullptr;
        arg2value.clear();
        for( size_t i = 0; i < func->n_args + 1; i++ ) {
            ASR::expr_t *func_margs_i = nullptr, *x_m_args_i = nullptr;
            if( i < func->n_args ) {
                func_margs_i = func->m_args[i];
                x_m_args_i = x.m_args[i];
            } else {
                func_margs_i = func->m_return_var;
                x_m_args_i = nullptr;
            }
            if( !ASR::is_a<ASR::Var_t>(func_margs_i) ) {
                return ;
            }
            ASR::Var_t* arg_var = ASR::down_cast<ASR::Var_t>(func_margs_i);
            // TODO: Expand to other symbol types, Function, Subroutine, ExternalSymbol
            if( !ASR::is_a<ASR::Variable_t>(arg_var->m_v) ) {
                return ;
            }
            ASR::Variable_t* arg_variable = ASR::down_cast<ASR::Variable_t>(arg_var->m_v);
            std::string arg_name = std::string(arg_variable->m_name) + "@" + std::string(func->m_name);
            ASR::stmt_t* assign_stmt = nullptr;
            ASR::expr_t* call_arg_var = PassUtils::create_auxiliary_variable_for_expr(x.m_args[i], arg_name, al, current_scope, assign_stmt);
            if( assign_stmt ) {
                pass_result.push_back(al, assign_stmt);
                return_var = call_arg_var;
            }
            arg2value[get_hash(arg_var->m_v)] = ASR::down_cast<ASR::Var_t>(call_arg_var)->m_v;
        }

        for( size_t i = 0; i < func->n_body; i++ ) {
            visit_stmt(*func->m_body[i]);
            pass_result.push_back(al, func->m_body[i]);
        }
        function_result_var = return_var;
    }

    void visit_Assignment(const ASR::Assignment_t& x) {
        from_inline_function_call = true;
        retain_original_stmt = true;
        ASR::Assignment_t& xx = const_cast<ASR::Assignment_t&>(x);
        function_result_var = nullptr;
        visit_expr(*x.m_value);
        if( function_result_var ) {
            xx.m_value = function_result_var;
        }
        function_result_var = nullptr;
        from_inline_function_call = false;
        retain_original_stmt = false;
    }

};

void pass_replace_inline_function_call(Allocator &al, ASR::TranslationUnit_t &unit,
                                       const std::string& rl_path) {
    InlineFunctionCallVisitor v(al, unit, rl_path);
    v.visit_TranslationUnit(unit);
    LFORTRAN_ASSERT(asr_verify(unit));
}


} // namespace LFortran
