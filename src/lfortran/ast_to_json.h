#ifndef LCOMPILERS_AST_TO_JSON_H
#define LCOMPILERS_AST_TO_JSON_H

#include <lfortran/ast.h>

namespace LCompilers {

    std::string ast_to_json(LCompilers::AST::ast_t &ast);

}

#endif // LCOMPILERS_AST_TO_JSON_H
