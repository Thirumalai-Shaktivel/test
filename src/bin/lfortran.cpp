#include <chrono>
#include <iostream>
#include <stdlib.h>

#include <bin/CLI11.hpp>
#include <bin/tpl/whereami/whereami.h>

#include <lfortran/stacktrace.h>
#include <lfortran/parser/parser.h>
#include <lfortran/pickle.h>
#include <lfortran/semantics/ast_to_asr.h>
#include <lfortran/codegen/asr_to_llvm.h>
#include <lfortran/codegen/evaluator.h>
#include <lfortran/config.h>

void section(const std::string &s)
{
    std::cout << color(LFortran::style::bold) << color(LFortran::fg::blue) << s << color(LFortran::style::reset) << color(LFortran::fg::reset) << std::endl;
}

std::string remove_extension(const std::string& filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot);
}

std::string remove_path(const std::string& filename) {
    size_t lastslash = filename.find_last_of("/");
    if (lastslash == std::string::npos) return filename;
    return filename.substr(lastslash+1);
}

bool ends_with(const std::string &s, const std::string &e) {
    if (s.size() < e.size()) return false;
    return s.substr(s.size()-e.size()) == e;
}

std::string read_file(const std::string &filename)
{
    std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary
            | std::ios::ate);

    std::ifstream::pos_type filesize = ifs.tellg();
    if (filesize < 0) return std::string();

    ifs.seekg(0, std::ios::beg);

    std::vector<char> bytes(filesize);
    ifs.read(&bytes[0], filesize);

    return std::string(&bytes[0], filesize);
}

#ifdef HAVE_LFORTRAN_LLVM
int prompt()
{
    std::cout << "Interactive Fortran. Experimental prototype, not ready for end users." << std::endl;
    std::cout << "  * Use Ctrl-D to exit" << std::endl;
    std::cout << "  * Use Enter to submit" << std::endl;
    std::cout << "Try: function f(); f = 42; end function" << std::endl;
    while (true) {
        std::cout << color(LFortran::style::bold) << color(LFortran::fg::green) << ">>> "
            << color(LFortran::style::reset) << color(LFortran::fg::reset);
        std::string input;
        std::getline(std::cin, input);
        if (std::cin.rdstate() & std::ios_base::eofbit) {
            std::cout << std::endl;
            std::cout << "Exiting." << std::endl;
            return 0;
        }
        if (std::cin.rdstate() & std::ios_base::badbit) {
            std::cout << "Irrecoverable stream error." << std::endl;
            std::cout << "Exiting." << std::endl;
            return 1;
        }
        if (std::cin.rdstate() & std::ios_base::failbit) {
            std::cout << "Input/output operation failed (formatting or extraction error)." << std::endl;
            std::cout << "Exiting." << std::endl;
            return 2;
        }
        if (std::cin.rdstate() != std::ios_base::goodbit) {
            std::cout << "Unknown error." << std::endl;
            std::cout << "Exiting." << std::endl;
            return 3;
        }
        section("Input:");
        std::cout << input << std::endl;

        // Src -> AST
        Allocator al(64*1024*1024);
        LFortran::AST::TranslationUnit_t* ast;
        try {
            ast = LFortran::parse2(al, input);
        } catch (const LFortran::TokenizerError &e) {
            std::cout << "Tokenizing error: " << e.msg() << std::endl;
            continue;
        } catch (const LFortran::ParserError &e) {
            std::cout << "Parsing error: " << e.msg() << std::endl;
            continue;
        } catch (const LFortran::LFortranException &e) {
            std::cout << "Other LFortran exception: " << e.msg() << std::endl;
            continue;
        }
        section("AST:");
        std::cout << LFortran::pickle(*ast, true) << std::endl;


        // AST -> ASR
        LFortran::ASR::asr_t* asr;
        try {
            asr = LFortran::ast_to_asr(al, *ast);
        } catch (const LFortran::LFortranException &e) {
            std::cout << "LFortran exception: " << e.msg() << std::endl;
            continue;
        }
        section("ASR:");
        std::cout << LFortran::pickle(*asr, true) << std::endl;

        // ASR -> LLVM
        LFortran::LLVMEvaluator e;
        std::unique_ptr<LFortran::LLVMModule> m;
        try {
            m = LFortran::asr_to_llvm(*asr, e.get_context(), al);
        } catch (const LFortran::CodeGenError &e) {
            std::cout << "Code generation error: " << e.msg() << std::endl;
            continue;
        }
        section("LLVM IR:");
        std::cout << m->str() << std::endl;

        // LLVM -> Machine code -> Execution
        e.add_module(std::move(m));
        int r = e.intfn("f");
        section("Result:");
        std::cout << r << std::endl;
    }
    return 0;
}
#endif

int emit_tokens(const std::string &infile)
{
    std::string input = read_file(infile);
    // Src -> Tokens
    Allocator al(64*1024*1024);
    std::vector<int> toks;
    std::vector<LFortran::YYSTYPE> stypes;
    try {
        toks = LFortran::tokens(input, &stypes);
    } catch (const LFortran::TokenizerError &e) {
        std::cerr << "Tokenizing error: " << e.msg() << std::endl;
        return 1;
    }

    for (size_t i=0; i < toks.size(); i++) {
        std::cout << LFortran::pickle(toks[i], stypes[i]) << std::endl;
    }
    return 0;
}

int emit_ast(const std::string &infile)
{
    std::string input = read_file(infile);
    // Src -> AST
    Allocator al(64*1024*1024);
    LFortran::AST::TranslationUnit_t* ast;
    try {
        ast = LFortran::parse2(al, input);
    } catch (const LFortran::TokenizerError &e) {
        std::cerr << "Tokenizing error: " << e.msg() << std::endl;
        return 1;
    } catch (const LFortran::ParserError &e) {
        std::cerr << "Parsing error: " << e.msg() << std::endl;
        return 2;
    }

    std::cout << LFortran::pickle(*ast) << std::endl;
    return 0;
}

int emit_asr(const std::string &infile)
{
    std::string input = read_file(infile);

    // Src -> AST
    Allocator al(64*1024*1024);
    LFortran::AST::TranslationUnit_t* ast;
    try {
        ast = LFortran::parse2(al, input);
    } catch (const LFortran::TokenizerError &e) {
        std::cerr << "Tokenizing error: " << e.msg() << std::endl;
        return 1;
    } catch (const LFortran::ParserError &e) {
        std::cerr << "Parsing error: " << e.msg() << std::endl;
        return 2;
    }

    // AST -> ASR
    LFortran::ASR::asr_t* asr;
    try {
        // FIXME: For now we only transform the first node in the list:
        asr = LFortran::ast_to_asr(al, *ast);
    } catch (const LFortran::SemanticError &e) {
        std::cerr << "Semantic error: " << e.msg() << std::endl;
        return 2;
    }

    std::cout << LFortran::pickle(*asr) << std::endl;
    return 0;
}

#ifdef HAVE_LFORTRAN_LLVM
int emit_llvm(const std::string &infile)
{
    std::string input = read_file(infile);

    // Src -> AST
    Allocator al(64*1024*1024);
    LFortran::AST::TranslationUnit_t* ast;
    try {
        ast = LFortran::parse2(al, input);
    } catch (const LFortran::TokenizerError &e) {
        std::cerr << "Tokenizing error: " << e.msg() << std::endl;
        return 1;
    } catch (const LFortran::ParserError &e) {
        std::cerr << "Parsing error: " << e.msg() << std::endl;
        return 2;
    }

    // AST -> ASR
    LFortran::ASR::asr_t* asr = LFortran::ast_to_asr(al, *ast);

    // ASR -> LLVM
    LFortran::LLVMEvaluator e;
    std::unique_ptr<LFortran::LLVMModule> m;
    try {
        m = LFortran::asr_to_llvm(*asr, e.get_context(), al);
    } catch (const LFortran::CodeGenError &e) {
        std::cerr << "Code generation error: " << e.msg() << std::endl;
        return 5;
    }

    std::cout << m->str() << std::endl;
    return 0;
}

int compile_to_object_file(const std::string &infile, const std::string &outfile,
        bool assembly=false)
{
    std::string input = read_file(infile);

    // Src -> AST
    Allocator al(64*1024*1024);
    LFortran::AST::TranslationUnit_t* ast;
    try {
        ast = LFortran::parse2(al, input);
    } catch (const LFortran::TokenizerError &e) {
        std::cerr << "Tokenizing error: " << e.msg() << std::endl;
        return 1;
    } catch (const LFortran::ParserError &e) {
        std::cerr << "Parsing error: " << e.msg() << std::endl;
        return 2;
    }

    // AST -> ASR
    LFortran::ASR::asr_t* asr = LFortran::ast_to_asr(al, *ast);

    // ASR -> LLVM
    LFortran::LLVMEvaluator e;
    std::unique_ptr<LFortran::LLVMModule> m;
    try {
        m = LFortran::asr_to_llvm(*asr, e.get_context(), al);
    } catch (const LFortran::CodeGenError &e) {
        std::cerr << "Code generation error: " << e.msg() << std::endl;
        return 5;
    }

    // LLVM -> Machine code (saves to an object file)
    if (assembly) {
        e.save_asm_file(*(m->m_m), outfile);
    } else {
        e.save_object_file(*(m->m_m), outfile);
    }

    return 0;
}

int compile_to_assembly_file(const std::string &infile, const std::string &outfile)
{
    return compile_to_object_file(infile, outfile, true);
}
#endif

// infile is an object file
// outfile will become the executable
int link_executable(const std::string &infile, const std::string &outfile,
    const std::string &runtime_library_dir, bool static_executable=false)
{
    /*
    The `gcc` line for dynamic linking that is constructed below:

    gcc -o $outfile $infile \
        -Lsrc/runtime -Wl,-rpath=src/runtime -llfortran_runtime

    is equivalent to the following:

    ld -o $outfile $infile \
        -Lsrc/runtime -rpath=src/runtime -llfortran_runtime \
        -dynamic-linker /lib64/ld-linux-x86-64.so.2  \
        /usr/lib/x86_64-linux-gnu/Scrt1.o /usr/lib/x86_64-linux-gnu/libc.so

    and this for static linking:

    gcc -static -o $outfile $infile \
        -Lsrc/runtime -Wl,-rpath=src/runtime -llfortran_runtime_static

    is equivalent to:

    ld -o $outfile $infile \
        -Lsrc/runtime -rpath=src/runtime -llfortran_runtime_static \
        /usr/lib/x86_64-linux-gnu/crt1.o /usr/lib/x86_64-linux-gnu/crti.o \
        /usr/lib/x86_64-linux-gnu/libc.a \
        /usr/lib/gcc/x86_64-linux-gnu/7/libgcc_eh.a \
        /usr/lib/x86_64-linux-gnu/libc.a \
        /usr/lib/gcc/x86_64-linux-gnu/7/libgcc.a \
        /usr/lib/x86_64-linux-gnu/crtn.o

    This was tested on Ubuntu 18.04.

    The `gcc` and `ld` approaches are equivalent except:

    1. The `gcc` command knows how to find and link the `libc` library,
       while in `ld` we must do that manually
    2. For dynamic linking, we must also specify the dynamic linker for `ld`

    Notes:

    * We can use `lld` to do the linking via the `ld` approach, so `ld` is
      preferable if we can mitigate the issues 1. and 2.
    * If we ship our own libc (such as musl), then we know how to find it
      and link it, which mitigates the issue 1.
    * If we link `musl` statically, then issue 2. does not apply.
    * If we link `musl` dynamically, then we have to find the dynamic
      linker (doable), which mitigates the issue 2.

    One way to find the default dynamic linker is by:

        $ readelf -e /bin/bash | grep ld-linux
            [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]

    There are probably simpler ways.


    */
    std::string CC = "gcc";
    std::string base_path = runtime_library_dir;
    std::string options;
    std::string runtime_lib = "lfortran_runtime";
    if (static_executable) {
        options += " -static ";
        runtime_lib = "lfortran_runtime_static";
    }
    std::string cmd = CC + options + " -o " + outfile + " " + infile + " -L"
        + base_path + " -Wl,-rpath," + base_path + " -l" + runtime_lib + " -lm";
    int err = system(cmd.c_str());
    if (err) {
        std::cout << "The command '" + cmd + "' failed." << std::endl;
        return 10;
    }
    return 0;
}

void get_executable_path(std::string &executable_path, int &dirname_length)
{
    int length;

    length = wai_getExecutablePath(NULL, 0, &dirname_length);
    if (length > 0) {
        std::string path(length+1, '\0');
        wai_getExecutablePath(&path[0], length, &dirname_length);
        executable_path = path;
    } else {
        throw LFortran::LFortranException("Cannot determine executable path.");
    }
}

std::string get_runtime_library_dir()
{
    char *env_p = std::getenv("LFORTRAN_RUNTIME_LIBRARY_DIR");
    if (env_p) return env_p;

    std::string path;
    int dirname_length;
    get_executable_path(path, dirname_length);
    std::string dirname = path.substr(0,dirname_length);
    if (ends_with(dirname, "src/bin")) {
        // Development version
        return dirname + "/../runtime";
    } else {
        // Installed version
        return dirname + "/../share/lfortran/lib/";
    }
}

int main(int argc, char *argv[])
{
#if defined(HAVE_LFORTRAN_STACKTRACE)
    LFortran::print_stack_on_segfault();
#endif
    std::string runtime_library_dir = get_runtime_library_dir();

    bool arg_S = false;
    bool arg_c = false;
    bool arg_v = false;
    bool arg_E = false;
    std::string arg_o;
    std::string arg_file;
    bool arg_version = false;
    bool show_tokens = false;
    bool show_ast = false;
    bool show_asr = false;
    bool show_llvm = false;
    bool show_asm = false;
    bool static_link = false;

    CLI::App app{"LFortran: modern interactive LLVM-based Fortran compiler"};
    // Standard options compatible with gfortran, gcc or clang
    // We follow the established conventions
    app.add_option("file", arg_file, "Source file");
    app.add_flag("-S", arg_S, "Emit assembly, do not assemble or link");
    app.add_flag("-c", arg_c, "Compile and assemble, do not link");
    app.add_option("-o", arg_o, "Specify the file to place the output into");
    app.add_flag("-v", arg_v, "Be more verbose");
    app.add_flag("-E", arg_E, "Preprocess only; do not compile, assemble or link");
    app.add_flag("--version", arg_version, "Display compiler version information");

    // LFortran specific options
    app.add_flag("--show-tokens", show_tokens, "Show tokens for the given file and exit");
    app.add_flag("--show-ast", show_ast, "Show AST for the given file and exit");
    app.add_flag("--show-asr", show_asr, "Show ASR for the given file and exit");
    app.add_flag("--show-llvm", show_llvm, "Show LLVM IR for the given file and exit");
    app.add_flag("--show-asm", show_asm, "Show assembly for the given file and exit");
    app.add_flag("--static", static_link, "Create a static executable");
    CLI11_PARSE(app, argc, argv);

    if (arg_version) {
        std::string version = LFORTRAN_VERSION;
        if (version == "0.1.1") version = "git";
        std::cout << "LFortran version: " << version << std::endl;
        return 0;
    }

    if (arg_E) {
        return 1;
    }

    if (arg_file.size() == 0) {
#ifdef HAVE_LFORTRAN_LLVM
        return prompt();
#else
        std::cerr << "Interactive prompt requires the LLVM backend to be enabled. Recompile with `WITH_LLVM=yes`." << std::endl;
        return 1;
#endif
    }

    std::string outfile;
    std::string basename;
    basename = remove_extension(arg_file);
    basename = remove_path(basename);
    if (arg_o.size() > 0) {
        outfile = arg_o;
    } else if (arg_S) {
        outfile = basename + ".s";
    } else if (arg_c) {
        outfile = basename + ".o";
    } else if (show_tokens) {
        outfile = basename + ".tokens";
    } else if (show_ast) {
        outfile = basename + ".ast";
    } else if (show_asr) {
        outfile = basename + ".asr";
    } else if (show_llvm) {
        outfile = basename + ".ll";
    } else {
        outfile = "a.out";
    }

    if (show_tokens) {
        return emit_tokens(arg_file);
    }
    if (show_ast) {
        return emit_ast(arg_file);
    }
    if (show_asr) {
        return emit_asr(arg_file);
    }
    if (show_llvm) {
#ifdef HAVE_LFORTRAN_LLVM
        return emit_llvm(arg_file);
#else
        std::cerr << "The --show-llvm option requires the LLVM backend to be enabled. Recompile with `WITH_LLVM=yes`." << std::endl;
        return 1;
#endif
    }
    if (arg_S) {
#ifdef HAVE_LFORTRAN_LLVM
        return compile_to_assembly_file(arg_file, outfile);
#else
        std::cerr << "The -S option requires the LLVM backend to be enabled. Recompile with `WITH_LLVM=yes`." << std::endl;
        return 1;
#endif
    }
    if (arg_c) {
#ifdef HAVE_LFORTRAN_LLVM
        return compile_to_object_file(arg_file, outfile);
#else
        std::cerr << "The -c option requires the LLVM backend to be enabled. Recompile with `WITH_LLVM=yes`." << std::endl;
        return 1;
#endif
    }

    if (ends_with(arg_file, ".f90")) {
        std::string tmp_o = outfile + ".tmp.o";
#ifdef HAVE_LFORTRAN_LLVM
        int err = compile_to_object_file(arg_file, tmp_o);
        if (err) return err;
#else
        std::cerr << "Compiling Fortran files to object files requires the LLVM backend to be enabled. Recompile with `WITH_LLVM=yes`." << std::endl;
        return 1;
#endif
        return link_executable(tmp_o, outfile, runtime_library_dir, static_link);
    } else {
        return link_executable(arg_file, outfile, runtime_library_dir, static_link);
    }
}
