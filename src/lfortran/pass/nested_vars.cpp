#include <lfortran/asr.h>
#include <lfortran/containers.h>
#include <lfortran/exception.h>
#include <lfortran/asr_utils.h>
#include <lfortran/asr_verify.h>
#include <lfortran/pass/nested_vars.h>


namespace LFortran {

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
    SymbolTable* current_scope;

public:
    std::vector<uint64_t> needed_globals;

    template<typename T>
    void visit_procedure(const T &x) {
        nesting_depth++;
        if (nesting_depth == 1) {
            for (auto &item : x.m_symtab->scope) {
                if (ASR::is_a<ASR::Subroutine_t>(*item.second)) {
                    ASR::Subroutine_t *s = ASR::down_cast<ASR::Subroutine_t>(
                            item.second);
                    visit_Subroutine(*s);
                }
                if (ASR::is_a<ASR::Function_t>(*item.second)) {
                    ASR::Function_t *s = ASR::down_cast<ASR::Function_t>(
                            item.second);
                    visit_Function(*s);
                }
            }
        } else {
            current_scope = x.m_symtab;
            for (size_t i = 0; i < x.n_body; i++) {
                visit_stmt(*x.m_body[i]);
            }
        }
        nesting_depth--;
    }

    void visit_Subroutine(const ASR::Subroutine_t &x) {
        visit_procedure(x);
    }

    void visit_Function(const ASR::Function_t &x) {
        visit_procedure(x);
    }

    void visit_Var(const ASR::Var_t &x) {
        // Only attempt if we are actually in a nested function
        if (nesting_depth > 1) {
            ASR::Variable_t *v = ASR::down_cast<ASR::Variable_t>(
                    symbol_get_past_external(x.m_v));
            // If the variable is not defined in the current scope, it is a
            // "needed global" since we need to be able to access it from the
            // nested procedure.
            if (current_scope->scope.find(v->m_name) ==
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
