#include <libasr/asr.h>
#include <lfortran/containers.h>
#include <lfortran/exception.h>
#include <libasr/asr_utils.h>
#include <libasr/asr_verify.h>
#include <libasr/pass/flip_sign.h>
#include <libasr/pass/pass_utils.h>

#include <vector>
#include <utility>


namespace LFortran {

using ASR::down_cast;
using ASR::is_a;

class FlipSignVisitor : public ASR::BaseWalkVisitor<FlipSignVisitor>
{
private:
    Allocator &al;
    ASR::TranslationUnit_t &unit;
    Vec<ASR::stmt_t*> flip_sign_result;

    ASR::expr_t *flip_sign_signal_variable, *flip_sign_variable;

    SymbolTable* current_scope;

    bool is_if_present;
    bool is_compare_present;
    bool is_function_call_present, is_function_modulo, is_divisor_2;
    bool is_one_present;
    bool is_unary_op_present, is_operand_same_as_input;
    bool is_flip_sign_present;

public:
    FlipSignVisitor(Allocator &al_, ASR::TranslationUnit_t &unit_) : al(al_), unit(unit_)
    {
    }

    void transform_stmts(ASR::stmt_t **&m_body, size_t &n_body) {
        Vec<ASR::stmt_t*> body;
        body.reserve(al, n_body);
        for (size_t i=0; i<n_body; i++) {
            // Not necessary after we check it after each visit_stmt in every
            // visitor method:
            flip_sign_result.reserve(al, 0);
            visit_stmt(*m_body[i]);
            if (flip_sign_result.size() > 0) {
                for (size_t j=0; j<flip_sign_result.size(); j++) {
                    body.push_back(al, flip_sign_result[j]);
                }
                flip_sign_result.n = 0;
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

        transform_stmts(xx.m_body, xx.n_body);

    }

    void visit_Subroutine(const ASR::Subroutine_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        ASR::Subroutine_t &xx = const_cast<ASR::Subroutine_t&>(x);
        current_scope = xx.m_symtab;
        transform_stmts(xx.m_body, xx.n_body);
    }

    void visit_Function(const ASR::Function_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        ASR::Function_t &xx = const_cast<ASR::Function_t&>(x);
        current_scope = xx.m_symtab;
        transform_stmts(xx.m_body, xx.n_body);
    }

    void visit_If(const ASR::If_t& x) {
        is_if_present = true;
        is_compare_present = false;
        is_function_call_present = false;
        is_function_modulo = false, is_divisor_2 = false;
        is_one_present = false;
        is_unary_op_present = false;
        is_operand_same_as_input = false;
        flip_sign_signal_variable = flip_sign_variable = nullptr;
        visit_expr(*(x.m_test));
        if( x.n_body == 1 && x.n_orelse == 0 ) {
            if( x.m_body[0]->type == ASR::stmtType::Assignment ) {
                visit_stmt(*(x.m_body[0]));
            }
        }
        set_flip_sign();
        if( is_flip_sign_present ) {
            // xi = xor(shiftl(int(Nd),63), xi)
            LFORTRAN_ASSERT(flip_sign_signal_variable);
            LFORTRAN_ASSERT(flip_sign_variable);
            ASR::expr_t* left = PassUtils::get_ishift(flip_sign_signal_variable,
                                    63, al, unit, current_scope);
            ASR::expr_t* right = flip_sign_variable;
            ASR::asr_t* xor_op_asr = ASR::make_BoolOp_t(al, left->base.loc, left, ASR::boolopType::Xor, right,
                                                     ASRUtils::expr_type(right), nullptr);
            ASR::expr_t* xor_op = LFortran::ASRUtils::EXPR(xor_op_asr);
            ASR::stmt_t* assign = LFortran::ASRUtils::STMT(ASR::make_Assignment_t(al, flip_sign_variable->base.loc,
                                    flip_sign_variable, xor_op, nullptr));
            flip_sign_result.push_back(al, assign);
        }
    }

    void set_flip_sign() {
        is_flip_sign_present = (is_if_present &&
                                is_compare_present &&
                                is_function_call_present &&
                                is_function_modulo &&
                                is_divisor_2 &&
                                is_one_present &&
                                is_unary_op_present &&
                                is_operand_same_as_input);
    }

    void visit_Assignment(const ASR::Assignment_t& x) {
        if( x.m_value->type == ASR::exprType::UnaryOp ) {
            is_unary_op_present = true;
            ASR::symbol_t* sym = nullptr;
            ASR::UnaryOp_t* negation = ASR::down_cast<ASR::UnaryOp_t>(x.m_value);
            if( negation->m_operand->type == ASR::exprType::Var ) {
                ASR::Var_t* var = ASR::down_cast<ASR::Var_t>(negation->m_operand);
                sym = var->m_v;
            }
            if( x.m_target->type == ASR::exprType::Var ) {
                ASR::Var_t* var = ASR::down_cast<ASR::Var_t>(x.m_target);
                is_operand_same_as_input = sym == var->m_v;
                flip_sign_variable = x.m_target;
            }
        }
    }

    void visit_Compare(const ASR::Compare_t& x) {
        is_compare_present = true;
        ASR::expr_t* potential_one = nullptr;
        ASR::expr_t* potential_func_call = nullptr;
        if( x.m_left->type == ASR::exprType::FunctionCall ) {
            potential_one = x.m_right;
            potential_func_call = x.m_left;
        } else if( x.m_right->type == ASR::exprType::FunctionCall ) {
            potential_one = x.m_left;
            potential_func_call = x.m_right;
        }
        if( potential_one &&
            potential_one->type == ASR::exprType::ConstantInteger ) {
            ASR::ConstantInteger_t* const_int = ASR::down_cast<ASR::ConstantInteger_t>(potential_one);
            is_one_present = const_int->m_n == 1;
        }
        if( potential_func_call && is_one_present ) {
            visit_expr(*potential_func_call);
        }
    }

    void visit_FunctionCall(const ASR::FunctionCall_t& x) {
        is_function_call_present = true;
        ASR::symbol_t* func_name = nullptr;
        if( x.m_original_name ) {
            func_name = x.m_original_name;
        } else if( x.m_name ) {
            func_name = x.m_name;
        }
        if( func_name && func_name->type == ASR::symbolType::ExternalSymbol ) {
            ASR::ExternalSymbol_t* ext_sym = ASR::down_cast<ASR::ExternalSymbol_t>(func_name);
            if( std::string(ext_sym->m_original_name) == "modulo" &&
                std::string(ext_sym->m_module_name) == "lfortran_intrinsic_math2" ) {
                is_function_modulo = true;
            }
        }
        if( is_function_modulo && x.n_args == 2) {
            ASR::expr_t* arg0 = x.m_args[0];
            ASR::expr_t* arg1 = x.m_args[1];
            bool cond_for_arg0 = false, cond_for_arg1 = false;
            ASR::ttype_t* arg0_ttype = ASRUtils::expr_type(arg0);
            cond_for_arg0 = arg0_ttype->type == ASR::ttypeType::Integer;
            if( arg1->type == ASR::exprType::ConstantInteger ) {
                ASR::ConstantInteger_t* const_int = ASR::down_cast<ASR::ConstantInteger_t>(arg1);
                cond_for_arg1 = const_int->m_n == 2;
            }
            is_divisor_2 = cond_for_arg0 && cond_for_arg1;
            flip_sign_signal_variable = arg0;
        }
    }

};

void pass_replace_flip_sign(Allocator &al, ASR::TranslationUnit_t &unit) {
    FlipSignVisitor v(al, unit);
    v.visit_TranslationUnit(unit);
    LFORTRAN_ASSERT(asr_verify(unit));
}


} // namespace LFortran
