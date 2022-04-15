#include <libasr/codegen/wasmblr.h>
#include <iostream>
#include <memory>

#include <libasr/asr.h>
#include <libasr/containers.h>
#include <libasr/codegen/asr_to_cpp.h>
#include <libasr/exception.h>
#include <libasr/asr_utils.h>
#include <libasr/string_utils.h>
#include <libasr/pass/unused_functions.h>

namespace LFortran{

namespace {

    // Local exception that is only used in this file to exit the visitor
    // pattern and caught later (not propagated outside)
    class CodeGenError
    {
    public:
        diag::Diagnostic d;
    public:
        CodeGenError(const std::string &msg)
            : d{diag::Diagnostic(msg, diag::Level::Error, diag::Stage::CodeGen)}
        { }
    };

    class Abort {};

}

// several util functions here
struct SymbolInfo
{
    bool needs_declaration = true;
    bool intrinsic_function = false;
};

class ASRToWASMVisitor : public ASR::BaseVisitor<ASRToWASMVisitor>
{
public:
    diag::Diagnostics &diag;
    std::map<uint64_t, SymbolInfo> sym_info;
    std::string src;
    int indentation_level;
    int indentation_spaces;
    // The precedence of the last expression, using the table:
    // https://en.cppreference.com/w/cpp/language/operator_precedence
    int last_expr_precedence;
    bool intrinsic_module = false;
    const ASR::Function_t *current_function = nullptr;

    ASRToWASMVisitor(diag::Diagnostics &diag) : diag{diag} {}

    void visit_TranslationUnit(const ASR::TranslationUnit_t &x) {
//         // All loose statements must be converted to a function, so the items
//         // must be empty:
//         LFORTRAN_ASSERT(x.n_items == 0);
//         std::string unit_src = "";
//         indentation_level = 0;
//         indentation_spaces = 4;

//         std::string headers =
// R"(#include <iostream>
// #include <string>
// #include <vector>
// #include <cassert>
// #include <cmath>
// #include <Kokkos_Core.hpp>
// #include <lfortran_intrinsics.h>

// template <typename T>
// Kokkos::View<T*> from_std_vector(const std::vector<T> &v)
// {
//     Kokkos::View<T*> r("r", v.size());
//     for (size_t i=0; i < v.size(); i++) {
//         r(i) = v[i];
//     }
//     return r;
// }

// )";
//         unit_src += headers;


//         // TODO: We need to pre-declare all functions first, then generate code
//         // Otherwise some function might not be found.

//         {
//             // Process intrinsic modules in the right order
//             std::vector<std::string> build_order
//                 = LFortran::ASRUtils::determine_module_dependencies(x);
//             for (auto &item : build_order) {
//                 LFORTRAN_ASSERT(x.m_global_scope->scope.find(item)
//                     != x.m_global_scope->scope.end());
//                 if (startswith(item, "lfortran_intrinsic")) {
//                     ASR::symbol_t *mod = x.m_global_scope->scope[item];
//                     visit_symbol(*mod);
//                     unit_src += src;
//                 }
//             }
//         }

//         // Process procedures first:
//         for (auto &item : x.m_global_scope->scope) {
//             if (ASR::is_a<ASR::Function_t>(*item.second)
//                 || ASR::is_a<ASR::Subroutine_t>(*item.second)) {
//                 visit_symbol(*item.second);
//                 unit_src += src;
//             }
//         }

//         // Then do all the modules in the right order
//         std::vector<std::string> build_order
//             = LFortran::ASRUtils::determine_module_dependencies(x);
//         for (auto &item : build_order) {
//             LFORTRAN_ASSERT(x.m_global_scope->scope.find(item)
//                 != x.m_global_scope->scope.end());
//             if (!startswith(item, "lfortran_intrinsic")) {
//                 ASR::symbol_t *mod = x.m_global_scope->scope[item];
//                 visit_symbol(*mod);
//                 unit_src += src;
//             }
//         }

//         // Then the main program:
//         for (auto &item : x.m_global_scope->scope) {
//             if (ASR::is_a<ASR::Program_t>(*item.second)) {
//                 visit_symbol(*item.second);
//                 unit_src += src;
//             }
//         }
// 
        // src = unit_src;
        src = "This is temporary wasm code\n";
    }

};

Result<std::string> asr_to_wasm(Allocator &al, ASR::TranslationUnit_t &asr,
    diag::Diagnostics &diagnostics){
    pass_unused_functions(al, asr);
    ASRToWASMVisitor v(diagnostics);
    try {
        v.visit_asr((ASR::asr_t &)asr);
    } catch (const CodeGenError &e) {
        diagnostics.diagnostics.push_back(e.d);
        return Error();
    } catch (const Abort &) {
        return Error();
    }
    return v.src;
}

} // namespace LFortran
