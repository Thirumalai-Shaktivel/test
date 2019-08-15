#ifndef LFORTRAN_PARSER_SEM4b_H
#define LFORTRAN_PARSER_SEM4b_H

#include <lfortran/casts.h>
#include <lfortran/parser/alloc.h>
#include <lfortran/ast.h>

namespace LFortran {

extern Allocator al;

}

using LFortran::al;
using LFortran::AST::operatorType;
using LFortran::AST::expr_t;
using LFortran::AST::Name_t;
using LFortran::AST::BaseWalkVisitor;
using LFortran::AST::make_BinOp_t;
using LFortran::AST::make_Name_t;
using LFortran::AST::make_Num_t;

class CountVisitor : public BaseWalkVisitor<CountVisitor>
{
    int c_;
public:
    CountVisitor() : c_{0} {}
    void visit_Name(const Name_t &x) { c_ += 1; }
    int get_count() {
        return c_;
    }
};

static inline int count(const expr_t &b) {
    CountVisitor v;
    v.visit_expr(b);
    return v.get_count();
}

#define TYPE expr_t*
#define ADD(x, y) make_BinOp_t(al, x, operatorType::Add, y)
#define SUB(x, y) make_BinOp_t(al, x, operatorType::Sub, y)
#define MUL(x, y) make_BinOp_t(al, x, operatorType::Mul, y)
#define DIV(x, y) make_BinOp_t(al, x, operatorType::Div, y)
#define POW(x, y) make_BinOp_t(al, x, operatorType::Pow, y)
#define SYMBOL(x) make_Name_t(al, &x[0])
#define INTEGER(x) make_Num_t(al, x[0])
//#define PRINT(x) std::cout << x->type << std::endl
#define PRINT(x) std::cout << count(*x) << std::endl;

#endif
