#ifndef LFORTRAN_PARSER_STYPE_H
#define LFORTRAN_PARSER_STYPE_H

#include <vector>
#include <lfortran/ast.h>
#include <lfortran/parser/location.h>

namespace LFortran
{

struct YYSTYPE {
    LFortran::AST::ast_t* ast;
    std::vector<LFortran::AST::ast_t*> vec_ast;
    unsigned long n;
    std::string string;
    // Constructor
    YYSTYPE() = default;
    // Destructor
    ~YYSTYPE() = default;
    // Copy constructor and assignment
    YYSTYPE(const YYSTYPE &) = default;
    YYSTYPE &operator=(const YYSTYPE &) = default;
    // Move constructor and assignment
    YYSTYPE(YYSTYPE &&) = default;
    YYSTYPE &operator=(YYSTYPE &&) = default;
};

} // namespace LFortran


typedef struct LFortran::Location YYLTYPE;
#define YYLTYPE_IS_DECLARED 1
#define YYLTYPE_IS_TRIVIAL 1


#endif
