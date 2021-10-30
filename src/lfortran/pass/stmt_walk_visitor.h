// #include <lfortran/pass/do_loops.h>

namespace LFortran {

using ASR::down_cast;
using ASR::is_a;

template <class Derived>
class StatementWalkVisitor : public ASR::BaseWalkVisitor<Derived>
{
private:
    Vec<ASR::stmt_t*> stmts;

public:
    Allocator &al;
    StatementWalkVisitor(Allocator &al) : al{al} {
    }

    void transform_stmts(ASR::stmt_t **&m_body, size_t &n_body) {
        Vec<ASR::stmt_t*> body;
        body.reserve(al, n_body);
        for (size_t i=0; i<n_body; i++) {
            // Not necessary after we check it after each visit_stmt in every
            // visitor method:
            stmts.n = 0;
            this->visit_stmt(*m_body[i]);
            if (stmts.size() > 0) {
                for (size_t j=0; j<stmts.size(); j++) {
                    body.push_back(al, stmts[j]);
                }
                stmts.n = 0;
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

    void visit_DoLoop(const ASR::DoLoop_t &x) {
        // do_loop_result = replace_doloop(al, x);
    }
};

} // namespace LFortran
