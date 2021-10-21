#ifndef LFORTRAN_AST_TO_ASR_H
#define LFORTRAN_AST_TO_ASR_H

#include <lfortran/ast.h>
#include <lfortran/asr.h>

namespace LFortran {

    Result<ASR::TranslationUnit_t*> ast_to_asr(Allocator &al,
        AST::TranslationUnit_t &ast, diag::Diagnostics &diagnostics,
        SymbolTable *symbol_table=nullptr,
        bool symtab_only=false);

} // namespace LFortran

#endif // LFORTRAN_AST_TO_ASR_H
