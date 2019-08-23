#ifndef LFORTRAN_PARSER_SEM4b_H
#define LFORTRAN_PARSER_SEM4b_H

/*
   This header file contains parser semantics: how the AST classes get
   constructed from the parser. This file only gets included in the generated
   parser cpp file, nowhere else.

   Note that this is part of constructing the AST from the source code, not the
   LFortran semantic phase (AST -> ASR).
*/

#include <cstring>

#include <lfortran/ast.h>

using LFortran::Location;

using LFortran::AST::astType;
using LFortran::AST::exprType;
using LFortran::AST::operatorType;
using LFortran::AST::stmtType;

using LFortran::AST::ast_t;
using LFortran::AST::decl_t;
using LFortran::AST::do_loop_head_t;
using LFortran::AST::expr_t;
using LFortran::AST::stmt_t;
using LFortran::AST::unit_decl2_t;

using LFortran::AST::Assignment_t;
using LFortran::AST::Name_t;
using LFortran::AST::Num_t;

using LFortran::AST::make_BinOp_t;
using LFortran::AST::make_DoLoop_t;
using LFortran::AST::make_Exit_t;
using LFortran::AST::make_Cycle_t;
using LFortran::AST::make_Return_t;
using LFortran::AST::make_Name_t;
using LFortran::AST::make_Num_t;
using LFortran::AST::make_WhileLoop_t;


static inline expr_t* EXPR(const ast_t *f)
{
    LFORTRAN_ASSERT(f->type == astType::expr);
    return (expr_t*)f;
}

static inline char* name2char(const expr_t *n)
{
    LFORTRAN_ASSERT(n->type == exprType::Name)
    char *s = ((Name_t*)n)->m_id;
    return s;
}

static inline do_loop_head_t DOLOOP_HEAD(const expr_t *i, expr_t *a,
        expr_t *b, expr_t *c)
{
    do_loop_head_t s;
    if (i) {
        s.m_var = name2char(i);
    } else {
        s.m_var = nullptr;
    }
    s.m_start = a;
    s.m_end = b;
    s.m_increment = c;
    return s;
}

template <typename T, astType type>
static inline T** vec_cast(const YYSTYPE::Vec &x) {
    T **s = (T**)x.p;
    for (size_t i=0; i < x.size(); i++) {
        LFORTRAN_ASSERT((s[i]->base.type == type))
    }
    return s;
}

#define VEC_CAST(x, type) vec_cast<type##_t, astType::type>(x)
#define DECLS(x) VEC_CAST(x, unit_decl2)
#define STMTS(x) VEC_CAST(x, stmt)

static inline stmt_t** IFSTMTS(Allocator &al, ast_t* x)
{
    stmt_t **s = al.allocate<stmt_t*>();
    LFORTRAN_ASSERT(x->type == astType::stmt);
    *s = (stmt_t*)x;
    LFORTRAN_ASSERT((*s)->base.type == astType::stmt)
    return s;
}

static inline ast_t* make_SYMBOL(Allocator &al, const Location &loc,
        const YYSTYPE::Str &x)
{
    // Copy the string into our own allocated memory.
    // `x` is not NULL terminated, but we need to make it null terminated.
    // TODO: Instead, we should pass a pointer to the Tokenizer's string of the
    // original source code (the string there is not zero terminated, as it's a
    // substring), and a length. And provide functions to deal with the
    // non-zero terminated string properly. That will be much faster.
    char *s = x.c_str(al);
    LFORTRAN_ASSERT(s[x.size()] == '\0');
    return make_Name_t(al, loc, s);
}

static inline decl_t* DECL(Allocator &al, ast_t* x, const YYSTYPE::Str &type)
{
    decl_t *s = al.allocate<decl_t>();
    s->m_sym = name2char(EXPR(x));
    s->m_sym_type = type.c_str(al);
    s->m_dims = nullptr;
    s->m_attrs = nullptr;
    return s;
}


#define TYPE ast_t*
#define ADD(x, y, l) make_BinOp_t(p.m_a, l, EXPR(x), operatorType::Add, EXPR(y))
#define SUB(x, y, l) make_BinOp_t(p.m_a, l, EXPR(x), operatorType::Sub, EXPR(y))
#define MUL(x, y, l) make_BinOp_t(p.m_a, l, EXPR(x), operatorType::Mul, EXPR(y))
#define DIV(x, y, l) make_BinOp_t(p.m_a, l, EXPR(x), operatorType::Div, EXPR(y))
#define POW(x, y, l) make_BinOp_t(p.m_a, l, EXPR(x), operatorType::Pow, EXPR(y))
#define SYMBOL(x, l) make_SYMBOL(p.m_a, l, x)
#define INTEGER(x, l) make_Num_t(p.m_a, l, x)
#define ASSIGNMENT(x, y, l) make_Assignment_t(p.m_a, l, EXPR(x), EXPR(y))
#define EXIT(l) make_Exit_t(p.m_a, l)
#define RETURN(l) make_Return_t(p.m_a, l)
#define CYCLE(l) make_Cycle_t(p.m_a, l)
#define SUBROUTINE(name, decl, stmts, l) make_Subroutine_t(p.m_a, l, \
        /*name*/ nullptr, \
        /*args*/ nullptr, \
        /*n_args*/ 0, \
        /*use*/ nullptr, \
        /*n_use*/ 0, \
        /*decl*/ DECLS(decl), \
        /*n_decl*/ decl.size(), \
        /*body*/ STMTS(stmts), \
        /*n_body*/ stmts.size(), \
        /*contains*/ nullptr, \
        /*n_contains*/ 0)
#define FUNCTION(name, decl, stmts, l) make_Function_t(p.m_a, l, \
        /*name*/ nullptr, \
        /*args*/ nullptr, \
        /*n_args*/ 0, \
        /*return_type*/ nullptr, \
        /*return_var*/ nullptr, \
        /*bind*/ nullptr, \
        /*use*/ nullptr, \
        /*n_use*/ 0, \
        /*decl*/ DECLS(decl), \
        /*n_decl*/ decl.size(), \
        /*body*/ STMTS(stmts), \
        /*n_body*/ stmts.size(), \
        /*contains*/ nullptr, \
        /*n_contains*/ 0)
#define PROGRAM(name, decl, stmts, l) make_Program_t(p.m_a, l, \
        /*name*/ name2char(EXPR(name)), \
        /*use*/ nullptr, \
        /*n_use*/ 0, \
        /*decl*/ DECLS(decl), \
        /*n_decl*/ decl.size(), \
        /*body*/ STMTS(stmts), \
        /*n_body*/ stmts.size(), \
        /*contains*/ nullptr, \
        /*n_contains*/ 0)
#define RESULT(x) p.result.push_back(x)

#define IF1(cond, body, l) make_If_t(p.m_a, l, \
        /*test*/ EXPR(cond), \
        /*body*/ STMTS(body), \
        /*n_body*/ body.size(), \
        /*a_orelse*/ nullptr, \
        /*n_orelse*/ 0)

#define IF2(cond, body, orelse, l) make_If_t(p.m_a, l, \
        /*test*/ EXPR(cond), \
        /*body*/ STMTS(body), \
        /*n_body*/ body.size(), \
        /*a_orelse*/ STMTS(orelse), \
        /*n_orelse*/ orelse.size())

#define IF3(cond, body, ifblock, l) make_If_t(p.m_a, l, \
        /*test*/ EXPR(cond), \
        /*body*/ STMTS(body), \
        /*n_body*/ body.size(), \
        /*a_orelse*/ IFSTMTS(p.m_a, ifblock), \
        /*n_orelse*/ 1)

#define LIST_NEW(l) l.reserve(p.m_a, 4)
#define LIST_ADD(l, x) l.push_back(p.m_a, x)

#define WHILE(cond, body, l) make_WhileLoop_t(p.m_a, l, \
        /*test*/ EXPR(cond), \
        /*body*/ STMTS(body), \
        /*n_body*/ body.size())

#define DO1(body, l) make_DoLoop_t(p.m_a, l, \
        /*head*/ DOLOOP_HEAD(nullptr, nullptr, nullptr, nullptr), \
        /*body*/ STMTS(body), \
        /*n_body*/ body.size())

#define DO2(i, a, b, body, l) make_DoLoop_t(p.m_a, l, \
        /*head*/ DOLOOP_HEAD(EXPR(i), EXPR(a), EXPR(b), nullptr), \
        /*body*/ STMTS(body), \
        /*n_body*/ body.size())

#define DO3(i, a, b, c, body, l) make_DoLoop_t(p.m_a, l, \
        /*head*/ DOLOOP_HEAD(EXPR(i), EXPR(a), EXPR(b), EXPR(c)), \
        /*body*/ STMTS(body), \
        /*n_body*/ body.size())

#define VAR_DECL(type, sym, l) make_Declaration_t(p.m_a, l, \
        DECL(p.m_a, sym, type), 1)

#endif
