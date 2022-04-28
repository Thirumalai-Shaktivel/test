#include <chrono>
#include <iostream>
#include <stdlib.h>

// #include <emscripten/emscripten.h>

#include <libasr/stacktrace.h>
#include <lfortran/parser/parser.h>
#include <lfortran/parser/preprocessor.h>
#include <lfortran/pickle.h>
#include <lfortran/semantics/ast_to_asr.h>
#include <lfortran/mod_to_asr.h>
#include <libasr/codegen/asr_to_llvm.h>
#include <libasr/codegen/asr_to_cpp.h>
#include <libasr/codegen/asr_to_wasm.h>
#include <libasr/codegen/asr_to_py.h>
#include <libasr/codegen/asr_to_x86.h>
#include <lfortran/ast_to_src.h>
#include <lfortran/fortran_evaluator.h>
#include <libasr/codegen/evaluator.h>
#include <libasr/pass/do_loops.h>
#include <libasr/pass/global_stmts.h>
#include <libasr/pass/implied_do_loops.h>
#include <libasr/pass/array_op.h>
#include <libasr/pass/class_constructor.h>
#include <libasr/pass/arr_slice.h>
#include <libasr/pass/print_arr.h>
#include <libasr/pass/unused_functions.h>
#include <libasr/asr_utils.h>
#include <libasr/asr_verify.h>
#include <libasr/modfile.h>
#include <libasr/config.h>
#include <lfortran/fortran_kernel.h>
#include <libasr/string_utils.h>
#include <lfortran/utils.h>
#include <lfortran/parser/parser.tab.hh>


namespace {

using LFortran::endswith;
using LFortran::CompilerOptions;

enum Backend {
    llvm, cpp, x86
};

enum ASRPass {
    do_loops, global_stmts, implied_do_loops, array_op,
    arr_slice, print_arr, class_constructor, unused_functions,
};

std::string emit_ast(const std::string &input, CompilerOptions &compiler_options)
{
    LFortran::FortranEvaluator fe(compiler_options);
    LFortran::LocationManager lm;
    LFortran::diag::Diagnostics diagnostics;
    lm.in_filename = "input";
    LFortran::Result<std::string> r = fe.get_ast(input, lm, diagnostics);
    std::string out;
    out += diagnostics.render(input, lm, compiler_options);
    if (r.ok) {
        out += r.result;
    }
    return out;
}


std::string emit_asr(const std::string &input, CompilerOptions &compiler_options)
{
    LFortran::FortranEvaluator fe(compiler_options);
    LFortran::LocationManager lm;
    lm.in_filename = "input";
    LFortran::diag::Diagnostics diagnostics;
    LFortran::Result<std::string> r = fe.get_asr(input, lm, diagnostics);
    std::string out;
    out += diagnostics.render(input, lm, compiler_options);
    if (r.ok) {
        out += r.result;
    }
    return out;
}

std::string emit_wasm(const std::string &input, CompilerOptions &compiler_options)
{
    LFortran::FortranEvaluator fe(compiler_options);
    LFortran::LocationManager lm;
    LFortran::diag::Diagnostics diagnostics;
    lm.in_filename = "input";
    LFortran::Result<std::string> wasm = fe.get_wasm(input, lm, diagnostics);
    std::string out;
    out += diagnostics.render(input, lm, compiler_options);
    if (wasm.ok) {
        out += wasm.result;
    } else {
        LFORTRAN_ASSERT(diagnostics.has_error())
    }
    return out;
}




} // anonymous namespace

std::string driver(const std::string &input) {
    LFortran::CompilerOptions compiler_options;
    compiler_options.use_colors = false;
    std::string ast = emit_ast(input, compiler_options);
    std::string asr = emit_asr(input, compiler_options);
    std::string wasm = emit_wasm(input, compiler_options);
    std::string out = ast + "|" + asr + "|" + wasm;
    return out;
}

std::string buffer;

extern "C" {

// EMSCRIPTEN_KEEPALIVE
void sayHi() {
  printf("Hi!\n");
}

// EMSCRIPTEN_KEEPALIVE
void sayHi2(int i) {
    std::cout << "Number:" + std::to_string(i) + "\n";
}

// Call with:
// Module.ccall("sayHi3", null, ["string"], ["some string"])
// EMSCRIPTEN_KEEPALIVE
void sayHi3(char *p) {
    std::string s = p;
    std::cout << "String length: " << std::to_string(s.size()) << "\n";
    std::cout << "String itself: " << s << "\n";
}

// Call with:
// Module.ccall("sayHi4", "string", ["string"], ["some string"])
// Or:
// f = Module.cwrap("sayHi4", "string", ["string"])
// f("some string")
// EMSCRIPTEN_KEEPALIVE
char* sayHi4(char *p) {
    std::string s = p;
    std::cout << "String length: " << std::to_string(s.size()) << "\n";
    std::cout << "String itself: " << s << "\n";
    std::string out = "(" + s + ")";
    buffer = out;
    return &buffer[0];
}

// Call with:
// driver = Module.cwrap("cdriver", "string", ["string"])
// driver("5+6")
// driver("integer :: x\nx = 6+8\nx")
// EMSCRIPTEN_KEEPALIVE
char* cdriver(char *p) {
    std::string input = p;
    buffer = driver(input);
    return &buffer[0];
}

}

int main()
{
    /*
    std::cout << "Start" << std::endl;
    std::string input = R"(
program expr2
implicit none
integer :: x
x = (2+3)*5
print *, x
end program
    )";
    std::cout << driver(input);
    std::cout << "End" << std::endl;
    */
    return 0;
}
