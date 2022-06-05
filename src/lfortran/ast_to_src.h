#ifndef LCOMPILERS_AST_TO_SRC_H
#define LCOMPILERS_AST_TO_SRC_H

#include <lfortran/ast.h>

namespace LCompilers {

    // Converts AST to Fortran source code
    std::string ast_to_src(AST::TranslationUnit_t &ast, bool color=false,
        int indent=4, bool indent_unit=false);

}

#endif // LCOMPILERS_AST_TO_SRC_H
