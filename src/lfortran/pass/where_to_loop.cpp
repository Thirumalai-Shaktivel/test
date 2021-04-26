#include <lfortran/asr.h>
#include <lfortran/containers.h>
#include <lfortran/exception.h>
#include <lfortran/asr_utils.h>
#include <lfortran/asr_verify.h>
#include <lfortran/pass/where_to_loop.h>


namespace LFortran {

using ASR::down_cast;
using ASR::is_a;

/*

This ASR pass replaces where statement with loop over the arrays. The function
`pass_replace_where` transforms the ASR tree in-place.

Converts:

    where (a >= 0)
        b = 1
    else where
        b = 0
    end where

to:

    i0 = 0
    do while (i0 <= size(a))
        if (a(i0) >= 0) then
            b(i0) = 1
        else
            b(i0) = 0
        i0 = i0 + 1
    end do

*/

Vec<ASR::stmt_t*> replace_where(Allocator &al, const ASR::Where_t &where) {
   
    /*
    std::cout << "Input:" << std::endl;
    std::cout << pickle((ASR::asr_t&)loop);
    std::cout << "Output:" << std::endl;
    std::cout << pickle((ASR::asr_t&)*stmt1);
    std::cout << pickle((ASR::asr_t&)*stmt2);
    std::cout << "--------------" << std::endl;
    */
}

class WhereVisitor : public ASR::BaseWalkVisitor<WhereVisitor>
{
private:
    Allocator &al;
    Vec<ASR::stmt_t*> where_result;
public:
    WhereVisitor(Allocator &al) : al{al} {
        where_result.n = 0;

    }

    void transform_stmts(ASR::stmt_t **&m_body, size_t &n_body) {
        Vec<ASR::stmt_t*> body;
        body.reserve(al, n_body);
        for (size_t i=0; i<n_body; i++) {
            // Not necessary after we check it after each visit_stmt in every
            // visitor method:
            select_case_result.n = 0;
            visit_stmt(*m_body[i]);
            if (select_case_result.size() > 0) {
                for (size_t j=0; j<select_case_result.size(); j++) {
                    body.push_back(al, select_case_result[j]);
                }
                select_case_result.n = 0;
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

    void visit_Where(const ASR::Where_t &x) {
        where_result = replace_where(al, x);
    }
};

void pass_replace_where(Allocator &al, ASR::TranslationUnit_t &unit) {
    WhereVisitor v(al);
    v.visit_TranslationUnit(unit);
    LFORTRAN_ASSERT(asr_verify(unit));
}


} // namespace LFortran
