#ifndef LFORTRAN_AST_TO_ASR_H
#define LFORTRAN_AST_TO_ASR_H

#include <lfortran/ast.h>
#include <lfortran/asr.h>

#define num_types 6

namespace LFortran {

    // class ImplicitCastRules {

    //     private:

    //         static const int defaultArgs;
    //         static const int errorArgs;
    //         static const int integerToReal;
    //         static const int realToInteger;
    //         static const int realToComplex;

    //         static const std::string type_names[num_types][2]; 

    //         static const int ruleMap[num_types][num_types];

    //     public:

    //         static ASR::expr_t* getConvertedValue
    //         (Allocator &al, const Location &a_loc, 
    //          ASR::expr_t* value, ASR::expr_t* target);

    // };

    ASR::TranslationUnit_t *ast_to_asr(Allocator &al,
        AST::TranslationUnit_t &ast, SymbolTable *symbol_table=nullptr);

} // namespace LFortran

#endif // LFORTRAN_AST_TO_ASR_H
