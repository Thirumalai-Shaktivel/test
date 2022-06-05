#ifndef LCOMPILERS_AST_TO_ASR_H
#define LCOMPILERS_AST_TO_ASR_H

#include <lfortran/ast.h>
#include <libasr/asr.h>

namespace LCompilers {

    Result<ASR::TranslationUnit_t*> ast_to_asr(Allocator &al,
        AST::TranslationUnit_t &ast, diag::Diagnostics &diagnostics,
        SymbolTable *symbol_table=nullptr,
        bool symtab_only=false);

} // namespace LCompilers

#endif // LCOMPILERS_AST_TO_ASR_H
