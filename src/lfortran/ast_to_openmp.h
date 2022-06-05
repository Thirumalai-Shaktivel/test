#ifndef LFORTRAN_AST_TO_OPENMP_H
#define LFORTRAN_AST_TO_OPENMP_H

#include <lfortran/ast.h>

namespace LCompilers {

    // Converts AST to Fortran source code with OpenMP parallel pragmas
    std::string ast_to_openmp(LCompilers::AST::ast_t &ast);

}

#endif // LFORTRAN_AST_TO_OPENMP_H
