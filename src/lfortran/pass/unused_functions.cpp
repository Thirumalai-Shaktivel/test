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

class UnusedFunctionsVisitor : public ASR::BaseWalkVisitor<UnusedFunctionsVisitor>
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
    }

};

// Returns a list of unused functions by their hash.
// The corresponding std::string is not needed, but for now we included
// it for easier debugging.
std::map<uint64_t, std::string> collect_unused_functions(ASR::TranslationUnit_t &unit) {
    UnusedFunctionsVisitor v;
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

void pass_unused_functions(Allocator &/*al*/, ASR::TranslationUnit_t &unit) {
    std::map<uint64_t, std::string> fn_unused;
    fn_unused = collect_unused_functions(unit);
    std::cout << "Unused functions:" << std::endl;
    for (auto &a : fn_unused) {
        std::cout << a.second << " ";
    }
    std::cout << std::endl;
}


} // namespace LFortran
