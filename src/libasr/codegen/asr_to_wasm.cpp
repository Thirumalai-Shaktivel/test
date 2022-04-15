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
