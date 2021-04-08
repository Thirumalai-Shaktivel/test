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
    integer :: a = 420

*/

class VarVisitor : public ASR::BaseWalkVisitor<VarVisitor>
{
private:
    Allocator &al;
    Vec<ASR::stmt_t*> init_var_result;
public:

    VarVisitor(Allocator &al) : al{al} {
        init_var_result.n = 0;
    }

    void transform_stmts(ASR::stmt_t **&m_body, size_t &n_body) {
        Vec<ASR::stmt_t*> body;
        body.reserve(al, n_body);
        for (size_t i=0; i<n_body; i++) {
            // Not necessary after we check it after each visit_stmt in every
            // visitor method:
            init_var_result.n = 0;
            visit_stmt(*m_body[i]);
            if (init_var_result.size() > 0) {
                for (size_t j=0; j<init_var_result.size(); j++) {
                    body.push_back(al, init_var_result[j]);
                }
                init_var_result.n = 0;
            } else {
                body.push_back(al, m_body[i]);
            }
        }
        m_body = body.p;
        n_body = body.size();
    }

    // TODO: Only Program and While is processed, we need to process all calls
    // to visit_stmt().

    void visit_Program(const ASR::Program_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        ASR::Program_t &xx = const_cast<ASR::Program_t&>(x);
        transform_stmts(xx.m_body, xx.n_body);

        // Transform nested functions and subroutines
        for (auto &item : x.m_symtab->scope) {
            if (is_a<ASR::Subroutine_t>(*item.second)) {
                ASR::Subroutine_t *s = down_cast<ASR::Subroutine_t>(item.second);
                visit_Subroutine(*s);
            }
            if (is_a<ASR::Function_t>(*item.second)) {
                ASR::Function_t *s = down_cast<ASR::Function_t>(item.second);
                visit_Function(*s);
            }
        }
    }

    void visit_Subroutine(const ASR::Subroutine_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        ASR::Subroutine_t &xx = const_cast<ASR::Subroutine_t&>(x);
        transform_stmts(xx.m_body, xx.n_body);
    }

    void visit_Function(const ASR::Function_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        ASR::Function_t &xx = const_cast<ASR::Function_t&>(x);
        transform_stmts(xx.m_body, xx.n_body);
    }

    void visit_WhileLoop(const ASR::WhileLoop_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        ASR::WhileLoop_t &xx = const_cast<ASR::WhileLoop_t&>(x);
        transform_stmts(xx.m_body, xx.n_body);
    }

    ASR::expr_t* get_value(ASR::expr_t* x) {
        ASR::expr_t* value = nullptr;
        switch( x->type ) {
            case ASR::exprType::Var: {
                ASR::Var_t* x_var = (ASR::Var_t*)(&(x->base));
                ASR::Variable_t *v = ASR::down_cast<ASR::Variable_t>(
                    symbol_get_past_external(x_var->m_v));
                if( v->m_storage == ASR::storage_typeType::Parameter ) {
                    value = v->m_value;
                }
                break;
            }
            case ASR::exprType::ImplicitCast: {
                ASR::ImplicitCast_t* x_implicitcast = (ASR::ImplicitCast_t*)(&(x->base));
                if( x_implicitcast->m_value != nullptr ) {
                    value = x_implicitcast->m_value;
                }
                break;
            }
            default:
                break;
        }
        return value;
    }

    void visit_ImplicitCast(const ASR::ImplicitCast_t& x) {
        this->visit_expr(*x.m_arg);
        ASR::expr_t* arg = x.m_arg;
        ASR::expr_t* value = get_value(arg);
        double val;
        if( value ) {
            switch( value->type ) {
                case ASR::exprType::ConstantInteger: {
                    val = ((ASR::ConstantInteger_t*)(&(value->base)))->m_n;
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }

    void visit_BinOp(const ASR::BinOp_t& x) {
        this->visit_expr(*x.m_left);
        ASR::expr_t* left = x.m_left;
        this->visit_expr(*x.m_right);
        ASR::expr_t* right = x.m_right;
        ASR::expr_t *left_val, *right_val;
        left_val = get_value(left);
        right_val = get_value(right);
        if( left_val && right_val ) {
            switch( left_val->type ) {
                case ASR::exprType::ConstantInteger : {
                    int left_val_i = ((ASR::ConstantInteger_t*)(&(left_val->base)))->m_n;
                    int right_val_i = ((ASR::ConstantInteger_t*)(&(right_val->base)))->m_n;
                    switch( x.m_op ) {
                        case ASR::binopType::Add : {
                            int res_val = left_val_i + right_val_i;
                            ASR::ttype_t *type = TYPE(ASR::make_Integer_t(al, x.base.base.loc,
                            4, nullptr, 0));
                            ASR::expr_t* x_const = EXPR(ASR::make_ConstantInteger_t(al, x.base.base.loc, res_val, type));
                            ASR::expr_t** addr_of_x_expr_ptr = &(const_cast<ASR::BinOp_t&>(x).m_value);
                            *addr_of_x_expr_ptr = x_const;
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
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
