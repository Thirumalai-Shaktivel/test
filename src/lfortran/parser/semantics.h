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
using LFortran::AST::cmpopType;
using LFortran::AST::stmtType;

using LFortran::AST::ast_t;
using LFortran::AST::decl_t;
using LFortran::AST::dimension_t;
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

template <typename T, astType type>
static inline T** vec_cast(const YYSTYPE::VecAST &x) {
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

static inline decl_t* DECL(Allocator &al, const YYSTYPE::VecDecl &x,
        const YYSTYPE::Str &type)
{
    decl_t *s = al.allocate<decl_t>(x.size());
    for (size_t i=0; i < x.size(); i++) {
        s[i] = x.p[i];
        s[i].m_sym_type = type.c_str(al);
    }
    return s;
}

static inline decl_t* DECL3(Allocator &al, ast_t* n,
        const YYSTYPE::VecDim *d, expr_t *e)
{
    decl_t *s = al.allocate<decl_t>();
    s->m_sym = name2char(EXPR(n));
    s->m_sym_type = nullptr;
    if (d) {
        s->n_dims = d->size();
        s->m_dims = d->p;
    } else {
        s->n_dims = 0;
        s->m_dims = nullptr;
    }
    s->n_attrs = 0;
    s->m_attrs = nullptr;
    s->m_initializer = e;
    return s;
}

static inline dimension_t DIM1(expr_t *a, expr_t *b)
{
    dimension_t s;
    s.m_start = a;
    s.m_end = b;
    return s;
}


#define TYPE ast_t*

// Assign last_* location to `a` from `b`
#define LLOC(a, b) a.last_line = b.last_line; a.last_column = b.last_column;

#define ADD(x, y, l) make_BinOp_t(p.m_a, l, EXPR(x), operatorType::Add, EXPR(y))
#define SUB(x, y, l) make_BinOp_t(p.m_a, l, EXPR(x), operatorType::Sub, EXPR(y))
#define MUL(x, y, l) make_BinOp_t(p.m_a, l, EXPR(x), operatorType::Mul, EXPR(y))
#define DIV(x, y, l) make_BinOp_t(p.m_a, l, EXPR(x), operatorType::Div, EXPR(y))
#define POW(x, y, l) make_BinOp_t(p.m_a, l, EXPR(x), operatorType::Pow, EXPR(y))

#define EQ(x, y, l)  make_Compare_t(p.m_a, l, EXPR(x), cmpopType::Eq, EXPR(y))
#define NE(x, y, l)  make_Compare_t(p.m_a, l, EXPR(x), cmpopType::NotEq, EXPR(y))
#define LT(x, y, l)  make_Compare_t(p.m_a, l, EXPR(x), cmpopType::Lt, EXPR(y))
#define LE(x, y, l)  make_Compare_t(p.m_a, l, EXPR(x), cmpopType::LtE, EXPR(y))
#define GT(x, y, l)  make_Compare_t(p.m_a, l, EXPR(x), cmpopType::Gt, EXPR(y))
#define GE(x, y, l)  make_Compare_t(p.m_a, l, EXPR(x), cmpopType::GtE, EXPR(y))


#define SYMBOL(x, l) make_SYMBOL(p.m_a, l, x)
#define INTEGER(x, l) make_Num_t(p.m_a, l, x)
#define ASSIGNMENT(x, y, l) make_Assignment_t(p.m_a, l, EXPR(x), EXPR(y))
#define EXIT(l) make_Exit_t(p.m_a, l)
#define RETURN(l) make_Return_t(p.m_a, l)
#define CYCLE(l) make_Cycle_t(p.m_a, l)
#define SUBROUTINE(name, decl, stmts, l) make_Subroutine_t(p.m_a, l, \
        /*name*/ name2char(EXPR(name)), \
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
        /*name*/ name2char(EXPR(name)), \
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
#define PLIST_ADD(l, x) l.push_back(p.m_a, *x)

#define WHILE(cond, body, l) make_WhileLoop_t(p.m_a, l, \
        /*test*/ EXPR(cond), \
        /*body*/ STMTS(body), \
        /*n_body*/ body.size())

#define DO1(body, l) make_DoLoop_t(p.m_a, l, \
        nullptr, nullptr, nullptr, nullptr, \
        /*body*/ STMTS(body), \
        /*n_body*/ body.size())

#define DO2(i, a, b, body, l) make_DoLoop_t(p.m_a, l, \
        name2char(EXPR(i)), EXPR(a), EXPR(b), nullptr, \
        /*body*/ STMTS(body), \
        /*n_body*/ body.size())

#define DO3(i, a, b, c, body, l) make_DoLoop_t(p.m_a, l, \
        name2char(EXPR(i)), EXPR(a), EXPR(b), EXPR(c), \
        /*body*/ STMTS(body), \
        /*n_body*/ body.size())

#define VAR_DECL(type, syms, l) make_Declaration_t(p.m_a, l, \
        DECL(p.m_a, syms, type), syms.size())

#define VAR_SYM_DECL1(id, l)         DECL3(p.m_a, id, nullptr, nullptr)
#define VAR_SYM_DECL2(id, e, l)      DECL3(p.m_a, id, nullptr, EXPR(e))
#define VAR_SYM_DECL3(id, a, l)      DECL3(p.m_a, id, &a, nullptr)
#define VAR_SYM_DECL4(id, a, e, l)   DECL3(p.m_a, id, &a, EXPR(e))

#define ARRAY_COMP_DECL1(a, l)       DIM1(EXPR(INTEGER(1, l)), EXPR(a))
#define ARRAY_COMP_DECL2(a, b, l)    DIM1(EXPR(a), EXPR(b))
#define ARRAY_COMP_DECL3(a, l)       DIM1(EXPR(a), nullptr)
#define ARRAY_COMP_DECL4(b, l)       DIM1(nullptr, EXPR(b))
#define ARRAY_COMP_DECL5(l)          DIM1(nullptr, nullptr)

#endif
