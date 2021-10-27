#include <chrono>
#include <iostream>
#include <stdlib.h>

#include <emscripten.h>

#include <lfortran/stacktrace.h>
#include <lfortran/parser/parser.h>
#include <lfortran/parser/preprocessor.h>
#include <lfortran/pickle.h>
#include <lfortran/semantics/ast_to_asr.h>
#include <lfortran/mod_to_asr.h>
#include <lfortran/codegen/asr_to_llvm.h>
#include <lfortran/codegen/asr_to_cpp.h>
#include <lfortran/codegen/asr_to_py.h>
#include <lfortran/codegen/asr_to_x86.h>
#include <lfortran/ast_to_src.h>
#include <lfortran/codegen/fortran_evaluator.h>
#include <lfortran/codegen/evaluator.h>
#include <lfortran/pass/do_loops.h>
#include <lfortran/pass/global_stmts.h>
#include <lfortran/pass/implied_do_loops.h>
#include <lfortran/pass/array_op.h>
#include <lfortran/pass/class_constructor.h>
#include <lfortran/pass/arr_slice.h>
#include <lfortran/pass/print_arr.h>
#include <lfortran/pass/unused_functions.h>
#include <lfortran/asr_utils.h>
#include <lfortran/asr_verify.h>
#include <lfortran/modfile.h>
#include <lfortran/config.h>
#include <lfortran/fortran_kernel.h>
#include <lfortran/string_utils.h>
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




} // anonymous namespace

std::string driver(const std::string &input) {
    LFortran::CompilerOptions compiler_options;
    compiler_options.use_colors = false;
    std::string ast = emit_ast(input, compiler_options);
    std::string asr = emit_asr(input, compiler_options);
    std::string out;
    out += "input:\n";
    out += input + "\n";
    out += "AST:\n";
    out += ast + "\n";
    out += "ASR:\n";
    out += asr + "\n";
    return out;
}

extern "C" {

EMSCRIPTEN_KEEPALIVE
void sayHi() {
  printf("Hi!\n");
}

EMSCRIPTEN_KEEPALIVE
void sayHi2(int i) {
    std::cout << "Number:" + std::to_string(i) + "\n";
}

}

int main(int argc, char *argv[])
{
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
    return 0;
}
