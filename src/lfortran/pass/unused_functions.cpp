#include <lfortran/asr.h>
#include <lfortran/containers.h>
#include <lfortran/exception.h>
#include <lfortran/asr_utils.h>
#include <lfortran/asr_verify.h>
#include <lfortran/pass/unused_functions.h>

#include <cstring>


namespace LFortran {

// Platform dependent fast unique hash:
uint64_t static get_hash(ASR::asr_t *node)
{
    return (uint64_t)node;
}

class CollectUnusedFunctionsVisitor :
    public ASR::BaseWalkVisitor<CollectUnusedFunctionsVisitor>
{
public:
    std::map<uint64_t, std::string> fn_declarations;
    std::map<uint64_t, std::string> fn_used;

    // TODO: Do subroutines just like Functions:

    void visit_Function(const ASR::Function_t &x) {
        uint64_t h = get_hash((ASR::asr_t*)&x);
        fn_declarations[h] = x.m_name;
        for (auto &a : x.m_symtab->scope) {
            this->visit_symbol(*a.second);
        }
        for (size_t i=0; i<x.n_body; i++) {
            visit_stmt(*x.m_body[i]);
        }
    }

    void visit_FunctionCall(const ASR::FunctionCall_t &x) {
        const ASR::symbol_t *s = ASRUtils::symbol_get_past_external(x.m_name);
        if (ASR::is_a<ASR::Function_t>(*s)) {
            ASR::Function_t *f = ASR::down_cast<ASR::Function_t>(s);
            std::string name = f->m_name;
            uint64_t h = get_hash((ASR::asr_t*)f);
            fn_used[h] = name;
        }
        if (ASR::is_a<ASR::GenericProcedure_t>(*s)) {
            ASR::GenericProcedure_t *g = ASR::down_cast<ASR::GenericProcedure_t>(s);
            std::string name = g->m_name;
            uint64_t h = get_hash((ASR::asr_t*)g);
            fn_used[h] = name;
        }
    }

    void visit_GenericProcedure(const ASR::GenericProcedure_t &x) {
        for (size_t i=0; i<x.n_procs; i++) {
            const ASR::symbol_t *s = ASRUtils::symbol_get_past_external(x.m_procs[i]);
            if (ASR::is_a<ASR::Function_t>(*s)) {
                ASR::Function_t *f = ASR::down_cast<ASR::Function_t>(s);
                std::string name = f->m_name;
                uint64_t h = get_hash((ASR::asr_t*)f);
                fn_used[h] = name;
            }
        }
        uint64_t h = get_hash((ASR::asr_t*)&x);
        fn_declarations[h] = x.m_name;
    }

};

// Returns a list of unused functions by their hash.
// The corresponding std::string is not needed, but for now we included
// it for easier debugging.
std::map<uint64_t, std::string> collect_unused_functions(ASR::TranslationUnit_t &unit) {
    CollectUnusedFunctionsVisitor v;
    v.visit_TranslationUnit(unit);
    std::map<uint64_t, std::string> fn_unused;
    for (auto &a : v.fn_declarations) {
        uint64_t h = a.first;
        if (v.fn_used.find(h) == v.fn_used.end()) {
            fn_unused[h] = a.second;
        }
    }
    return fn_unused;
}

class UnusedFunctionsVisitor : public ASR::BaseWalkVisitor<UnusedFunctionsVisitor>
{
public:
    std::map<uint64_t, std::string> fn_unused;

    void remove_unused_fn(std::map<std::string, ASR::symbol_t*> &scope) {
        for (auto it = scope.begin(); it != scope.end(); ) {
            this->visit_symbol(*it->second);
            uint64_t h = get_hash((ASR::asr_t*)it->second);
            if (fn_unused.find(h) != fn_unused.end()) {
//                std::cout << "Erasing: " << fn_unused[h] << std::endl;
                it = scope.erase(it);
            } else {
                ++it;
            }
        }
    }

    void visit_TranslationUnit(const ASR::TranslationUnit_t &x) {
        remove_unused_fn(x.m_global_scope->scope);
    }
    void visit_Program(const ASR::Program_t &x) {
        remove_unused_fn(x.m_symtab->scope);
    }
    void visit_Module(const ASR::Module_t &x) {
        remove_unused_fn(x.m_symtab->scope);
    }
    void visit_Subroutine(const ASR::Subroutine_t &x) {
        remove_unused_fn(x.m_symtab->scope);
    }
    void visit_Function(const ASR::Function_t &x) {
        remove_unused_fn(x.m_symtab->scope);
    }

};

void pass_unused_functions(ASR::TranslationUnit_t &unit) {
    for (int i=0; i < 4; i++)
    {
        std::map<uint64_t, std::string> fn_unused;
        fn_unused = collect_unused_functions(unit);
/*
        std::cout << "Unused functions:" << std::endl;
        for (auto &a : fn_unused) {
            std::cout << a.second << " ";
        }
        std::cout << std::endl;
*/
        UnusedFunctionsVisitor v;
        v.fn_unused = fn_unused;
        v.visit_TranslationUnit(unit);
        LFORTRAN_ASSERT(asr_verify(unit));
    }
}


} // namespace LFortran
