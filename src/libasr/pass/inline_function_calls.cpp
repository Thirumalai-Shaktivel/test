#include <libasr/asr.h>
#include <libasr/containers.h>
#include <libasr/exception.h>
#include <libasr/asr_utils.h>
#include <libasr/asr_verify.h>
#include <libasr/pass/inline_function_calls.h>
#include <libasr/pass/pass_utils.h>

#include <vector>
#include <map>
#include <utility>


namespace LFortran {

using ASR::down_cast;
using ASR::is_a;

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
class InlineFunctionCallVisitor : public PassUtils::PassVisitor<InlineFunctionCallVisitor>
{
private:

    std::string rl_path;

    ASR::expr_t* function_result_var;

    bool from_inline_function_call, inlining_function;

    std::map<std::string, ASR::symbol_t*> arg2value;

    std::string current_routine;

    ASR::ExprStmtDuplicator node_duplicator;

public:

    bool function_inlined;

    InlineFunctionCallVisitor(Allocator &al_, const std::string& rl_path_) : PassVisitor(al_, nullptr),
    rl_path(rl_path_), function_result_var(nullptr),
    from_inline_function_call(false), inlining_function(false),
    current_routine(""), node_duplicator(al_), function_inlined(false)
    {
        pass_result.reserve(al, 1);
    }

    void visit_Function(const ASR::Function_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        ASR::Function_t &xx = const_cast<ASR::Function_t&>(x);
        current_routine = std::string(xx.m_name);
        PassUtils::PassVisitor<InlineFunctionCallVisitor>::visit_Function(x);
        current_routine.clear();
    }

    void visit_Var(const ASR::Var_t& x) {
        ASR::Var_t& xx = const_cast<ASR::Var_t&>(x);
        ASR::Variable_t* x_var = ASR::down_cast<ASR::Variable_t>(x.m_v);
        std::string x_var_name = std::string(x_var->m_name);
        if( arg2value.find(x_var_name) != arg2value.end() ) {
            x_var = ASR::down_cast<ASR::Variable_t>(arg2value[x_var_name]);
            if( current_scope->scope.find(std::string(x_var->m_name)) != current_scope->scope.end() ) {
                xx.m_v = arg2value[x_var_name];
            }
            x_var = ASR::down_cast<ASR::Variable_t>(x.m_v);
        }
    }

    void visit_FunctionCall(const ASR::FunctionCall_t& x) {
        arg2value.clear();
        Vec<ASR::stmt_t*> pass_result_local;
        pass_result_local.reserve(al, 1);
        if( !from_inline_function_call || inlining_function ) {
            return ;
        }
        ASR::symbol_t* routine = x.m_name;
        if( !ASR::is_a<ASR::Function_t>(*routine) ) {
            return ;
        }

        ASR::Function_t* func = ASR::down_cast<ASR::Function_t>(routine);
        if( std::string(func->m_name) == current_routine ) {
            return ;
        }

        ASR::expr_t* return_var = nullptr;
        for( size_t i = 0; i < func->n_args + 1; i++ ) {
            ASR::expr_t *func_margs_i = nullptr, *x_m_args_i = nullptr;
            if( i < func->n_args ) {
                func_margs_i = func->m_args[i];
                x_m_args_i = x.m_args[i].m_value;
            } else {
                func_margs_i = func->m_return_var;
                x_m_args_i = nullptr;
            }
            if( !ASR::is_a<ASR::Var_t>(*func_margs_i) ) {
                arg2value.clear();
                return ;
            }
            ASR::Var_t* arg_var = ASR::down_cast<ASR::Var_t>(func_margs_i);
            // TODO: Expand to other symbol types, Function, Subroutine, ExternalSymbol
            if( !ASR::is_a<ASR::Variable_t>(*(arg_var->m_v)) ) {
                arg2value.clear();
                return ;
            }
            ASR::Variable_t* arg_variable = ASR::down_cast<ASR::Variable_t>(arg_var->m_v);
            std::string arg_variable_name = std::string(arg_variable->m_name);
            std::string arg_name = arg_variable_name + "@" + std::string(func->m_name);
            ASR::stmt_t* assign_stmt = nullptr;
            ASR::expr_t* call_arg_var = nullptr;
            if( x_m_args_i ) {
                call_arg_var = PassUtils::create_auxiliary_variable_for_expr(x_m_args_i, arg_name, al, current_scope, assign_stmt);
            } else {
                call_arg_var = PassUtils::create_auxiliary_variable(func_margs_i->base.loc, arg_name, al, current_scope, ASRUtils::expr_type(func_margs_i));
                return_var = call_arg_var;
            }
            if( assign_stmt ) {
                pass_result_local.push_back(al, assign_stmt);
            }
            arg2value[arg_variable_name] = ASR::down_cast<ASR::Var_t>(call_arg_var)->m_v;
        }

        bool success = true;
        std::vector<std::pair<ASR::expr_t*, ASR::symbol_t*>> exprs_to_be_visited;
        for( auto& itr : func->m_symtab->scope ) {
            ASR::Variable_t* func_var = ASR::down_cast<ASR::Variable_t>(itr.second);
            std::string func_var_name = itr.first;
            if( arg2value.find(func_var_name) == arg2value.end() ) {
                std::string local_var_name = func_var_name + "@" + std::string(func->m_name);
                node_duplicator.success = true;
                ASR::expr_t *m_symbolic_value = node_duplicator.duplicate_expr(func_var->m_symbolic_value);
                if( !node_duplicator.success ) {
                    success = false;
                    break;
                }
                node_duplicator.success = true;
                ASR::expr_t *m_value = node_duplicator.duplicate_expr(func_var->m_value);
                if( !node_duplicator.success ) {
                    success = false;
                    break;
                }
                ASR::symbol_t* local_var = (ASR::symbol_t*) ASR::make_Variable_t(
                                                al, func_var->base.base.loc, current_scope,
                                                s2c(al, local_var_name), ASR::intentType::Local,
                                                nullptr, nullptr, ASR::storage_typeType::Default,
                                                func_var->m_type, ASR::abiType::Source, ASR::accessType::Public,
                                                ASR::presenceType::Required, false);
                current_scope->scope[local_var_name] = local_var;
                arg2value[func_var_name] = local_var;
                if( m_symbolic_value ) {
                    exprs_to_be_visited.push_back(std::make_pair(m_symbolic_value, local_var));
                }
                if( m_value ) {
                    exprs_to_be_visited.push_back(std::make_pair(m_value, local_var));
                }
            }
        }

        for( size_t i = 0; i < exprs_to_be_visited.size() && success; i++ ) {
            ASR::expr_t* value = exprs_to_be_visited[i].first;
            visit_expr(*value);
            ASR::symbol_t* variable = exprs_to_be_visited[i].second;
            ASR::expr_t* var = LFortran::ASRUtils::EXPR(ASR::make_Var_t(al, variable->base.loc, variable));
            ASR::stmt_t* assign_stmt = ASRUtils::STMT(ASR::make_Assignment_t(al, var->base.loc, var, value, nullptr));
            pass_result_local.push_back(al, assign_stmt);
        }

        Vec<ASR::stmt_t*> func_copy;
        func_copy.reserve(al, func->n_body);
        for( size_t i = 0; i < func->n_body && success; i++ ) {
            node_duplicator.success = true;
            ASR::stmt_t* m_body_copy = node_duplicator.duplicate_stmt(func->m_body[i]);
            if( node_duplicator.success ) {
                func_copy.push_back(al, m_body_copy);
            } else {
                success = false;
            }
        }

        if( success ) {
            for( size_t i = 0; i < pass_result_local.size(); i++ ) {
                pass_result.push_back(al, pass_result_local[i]);
            }
            inlining_function = true;
            for( size_t i = 0; i < func->n_body; i++ ) {
                visit_stmt(*func_copy[i]);
                pass_result.push_back(al, func_copy[i]);
            }
            inlining_function = false;
            function_result_var = return_var;
        } else {
            for( auto& itr : arg2value ) {
                ASR::Variable_t* auxiliary_var = ASR::down_cast<ASR::Variable_t>(itr.second);
                current_scope->scope.erase(std::string(auxiliary_var->m_name));
            }
        }
        function_inlined = success;
        arg2value.clear();
    }

    void visit_BinOp(const ASR::BinOp_t& x) {
        ASR::BinOp_t& xx = const_cast<ASR::BinOp_t&>(x);
        from_inline_function_call = true;
        function_result_var = nullptr;
        visit_expr(*x.m_left);
        if( function_result_var ) {
            xx.m_left = function_result_var;
        }
        function_result_var = nullptr;
        visit_expr(*x.m_right);
        if( function_result_var ) {
            xx.m_right = function_result_var;
        }
        function_result_var = nullptr;
        from_inline_function_call = false;
    }

    void visit_Assignment(const ASR::Assignment_t& x) {
        from_inline_function_call = true;
        retain_original_stmt = true;
        ASR::Assignment_t& xx = const_cast<ASR::Assignment_t&>(x);
        function_result_var = nullptr;
        visit_expr(*x.m_target);
        function_result_var = nullptr;
        visit_expr(*x.m_value);
        if( function_result_var ) {
            xx.m_value = function_result_var;
        }
        function_result_var = nullptr;
        from_inline_function_call = false;
    }

};

void pass_inline_function_calls(Allocator &al, ASR::TranslationUnit_t &unit,
                                       const std::string& rl_path) {
    InlineFunctionCallVisitor v(al, rl_path);
    v.function_inlined = true;
    while( v.function_inlined ) {
        v.function_inlined = false;
        v.visit_TranslationUnit(unit);
    }
    LFORTRAN_ASSERT(asr_verify(unit));
}


} // namespace LFortran
