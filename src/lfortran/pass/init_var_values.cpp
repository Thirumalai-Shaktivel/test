#include <lfortran/asr.h>
#include <lfortran/containers.h>
#include <lfortran/exception.h>
#include <lfortran/asr_utils.h>
#include <lfortran/asr_verify.h>
#include <lfortran/pass/init_var_values.h>


namespace LFortran {

using ASR::down_cast;
using ASR::is_a;

/*

This ASR pass replaces initializer expressions with evaluated values. The function
`pass_replace_init_var_values` transforms the ASR tree in-place.

Converts:

    integer, parameter :: i = 20
    integer :: a = i**2 + i

to:

    integer, parameter :: i = 20
    integer :: a = 420

*/

Vec<ASR::stmt_t*> replace_initvarvalues(Allocator &al, const ASR::Variable_t &var) {
    Vec<ASR::stmt_t*> body;
    /*
    std::cout << "Input:" << std::endl;
    std::cout << pickle((ASR::asr_t&)loop);
    std::cout << "Output:" << std::endl;
    std::cout << pickle((ASR::asr_t&)*stmt1);
    std::cout << pickle((ASR::asr_t&)*stmt2);
    std::cout << "--------------" << std::endl;
    */
    return body;
}

class VariableVisitor : public ASR::BaseWalkVisitor<VariableVisitor>
{
private:
    Allocator &al;
    Vec<ASR::stmt_t*> init_var_result;
public:

    VariableVisitor(Allocator &al) : al{al} {
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

    void visit_Variable(const ASR::Variable_t &x) {
        // init_var_result = replace_initvarvalues(al, x);
    }
};

void pass_replace_init_var_values(Allocator &al, ASR::TranslationUnit_t &unit) {
    VariableVisitor v(al);
    // Each call transforms only one layer of nested loops, so we call it twice
    // to transform doubly nested loops:
    v.visit_TranslationUnit(unit);
    v.visit_TranslationUnit(unit);
    LFORTRAN_ASSERT(asr_verify(unit));
}


} // namespace LFortran
