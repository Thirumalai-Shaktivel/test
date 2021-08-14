#include <lfortran/asr.h>
#include <lfortran/containers.h>
#include <lfortran/exception.h>
#include <lfortran/asr_utils.h>
#include <lfortran/asr_verify.h>
#include <lfortran/pass/where_expression.h>
#include <lfortran/pass/pass_utils.h>


namespace LFortran {

using ASR::down_cast;
using ASR::is_a;

/*
This ASR pass replaces where expressions with if-else constructs.
The function `pass_replace_where_expressions` transforms the ASR tree in-place.

Converts:

  where (x>=50)
    x = 32.0
  elsewhere
    x = 0.0
  end where

to:

  do i = 1, 10
     if (x(i)>=50) then
        x(i) = 32.0
     else
        x(i) = 0.0
    end if
  end do

**/

class WhereExpressionVisitor : public ASR::BaseWalkVisitor<WhereExpressionVisitor>
{
private:
    Allocator &al;
    ASR::TranslationUnit_t &unit;
    Vec<ASR::stmt_t*> where_expression_result;
    SymbolTable* current_scope;
public:
    WhereExpressionVisitor(Allocator &al, ASR::TranslationUnit_t& unit) : al{al}, unit{unit},
    current_scope{nullptr} {
        where_expression_result.reserve(al, 1);

    }

    void transform_stmts(ASR::stmt_t **&m_body, size_t &n_body) {
        // TODO
    }

    void visit_Program(const ASR::Program_t &x) {
        // TODO
    }

    void visit_Subroutine(const ASR::Subroutine_t &x) {
        // TODO
    }

    void visit_Function(const ASR::Function_t &x) {
        // TODO
    }

    void visit_Var(const ASR::Var_t& x) {
        // TODO
    }

    void visit_ConstantInteger(const ASR::ConstantInteger_t&) {
        // TODO
    }

    void visit_ConstantReal(const ASR::ConstantReal_t&) {
        // TODO
    }

    void visit_ConstantComplex(const ASR::ConstantComplex_t&) {
        // TODO
    }

    void visit_ConstantLogical(const ASR::ConstantLogical_t&) {
        // TODO
    }

    void visit_BinOp(const ASR::BinOp_t& x) {
        // TODO
    }

    void create_where_expression(ASR::WhereExpression_t* idoloop, ASR::Var_t* arr_var, ASR::expr_t* arr_idx=nullptr) {
        // TODO
    }

    void visit_Assignment(const ASR::Assignment_t &x) {
        // TODO
};

void pass_replace_where_expressions(Allocator &al, ASR::TranslationUnit_t &unit) {
    WhereExpressionVisitor v(al, unit);
    v.visit_TranslationUnit(unit);
    LFORTRAN_ASSERT(asr_verify(unit));
}


} // namespace LFortran
