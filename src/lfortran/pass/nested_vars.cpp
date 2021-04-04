#include <lfortran/asr.h>
#include <lfortran/containers.h>
#include <lfortran/exception.h>
#include <lfortran/asr_utils.h>
#include <lfortran/asr_verify.h>
#include <lfortran/codegen/asr_to_llvm.h>


namespace LFortran {

using ASR::down_cast;
using ASR::is_a;

/*

This ASR pass is solely an analysis pass to determine if nested procedures
need access to variables from an outer scope. Such variables get hashed
and later compared to variable declarations while emitting IR. If we get a 
hash match, that variable is declared as a global, otherwise it remains solely 
local.

*/

uint64_t static get_hash(ASR::asr_t *node)
{
    return (uint64_t)node;
}

class NestedVarVisitor : public ASR::BaseWalkVisitor<NestedVarVisitor>
{
private:
    size_t nesting_depth = 0;
    SymbolTable* parent_scope;
    SymbolTable* current_scope;
public:
    std::vector<uint64_t> needed_globals;

    NestedVarVisitor() {};

    void track_vars(ASR::stmt_t **&m_body, size_t &n_body) {
        for (size_t i = 0; i < n_body; i++) {
            visit_stmt(*m_body[i]);
        }
    }

    void visit_Subroutine(const ASR::Subroutine_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        nesting_depth++;
        if (nesting_depth == 1) {
            parent_scope = x.m_symtab;
            for (auto &item : parent_scope->scope) {
                if (is_a<ASR::Subroutine_t>(*item.second)) {
                    ASR::Subroutine_t *s = ASR::down_cast<ASR::Subroutine_t>(
                            item.second);
                    visit_Subroutine(*s);
                }
                if (is_a<ASR::Function_t>(*item.second)) {
                    ASR::Function_t *s = ASR::down_cast<ASR::Function_t>(
                            item.second);
                    visit_Function(*s);
                }
            }
        } else {
            current_scope = x.m_symtab;
            ASR::Subroutine_t &xx = const_cast<ASR::Subroutine_t&>(x);
            track_vars(xx.m_body, xx.n_body);
        }
        nesting_depth--;
    }


    void visit_Function(const ASR::Function_t &x) {
        // FIXME: this is a hack, we need to pass in a non-const `x`,
        // which requires to generate a TransformVisitor.
        nesting_depth++;
        if (nesting_depth == 1) {
            parent_scope = x.m_symtab;
            for (auto &item : parent_scope->scope) {
                if (is_a<ASR::Subroutine_t>(*item.second)) {
                    ASR::Subroutine_t *s = ASR::down_cast<ASR::Subroutine_t>(
                            item.second);
                    visit_Subroutine(*s);
                }
                if (is_a<ASR::Function_t>(*item.second)) {
                    ASR::Function_t *s = ASR::down_cast<ASR::Function_t>(
                            item.second);
                    visit_Function(*s);
                }
            }
        } else {
            current_scope = x.m_symtab;
            ASR::Function_t &xx = const_cast<ASR::Function_t&>(x);
            track_vars(xx.m_body, xx.n_body);
        }
        nesting_depth--;
    }

    void visit_Var(const ASR::Var_t &x) {
        if ((bool&)x) { }

        // Only attempt if we are actually in a nested function

        if (nesting_depth > 1) {
            ASR::Variable_t *v = ASR::down_cast<ASR::Variable_t>(
                    symbol_get_past_external(x.m_v));
            // If the variable is defined in the parent scope and not defined in
            // the current scope, it is a "needed global" since we need to be able
            // to access it from the nested procedure.
            if (parent_scope->scope.find(v->m_name) != parent_scope->scope.end() 
                    && current_scope->scope.find(v->m_name) == 
                    current_scope->scope.end()) {
                uint32_t h = get_hash((ASR::asr_t*)v);
                needed_globals.push_back(h);
            }
        }
    }


};

std::vector<uint64_t> pass_find_nested_vars(ASR::TranslationUnit_t &unit) {
    NestedVarVisitor v;
    v.visit_TranslationUnit(unit);
    LFORTRAN_ASSERT(asr_verify(unit));
    return v.needed_globals;
}


}; // namespace LFortran
