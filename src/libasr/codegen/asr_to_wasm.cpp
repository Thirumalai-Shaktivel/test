// #include <libasr/codegen/wasmblr.h>
#include <libasr/codegen/binaryen-c.h>
#include <iostream>
#include <memory>

#include <libasr/asr.h>
#include <libasr/containers.h>
#include <libasr/codegen/asr_to_cpp.h>
#include <libasr/exception.h>
#include <libasr/asr_utils.h>
#include <libasr/string_utils.h>
#include <libasr/pass/unused_functions.h>

namespace LFortran {

namespace {

// Local exception that is only used in this file to exit the visitor
// pattern and caught later (not propagated outside)
class CodeGenError {
   public:
    diag::Diagnostic d;

   public:
    CodeGenError(const std::string &msg)
        : d{diag::Diagnostic(msg, diag::Level::Error, diag::Stage::CodeGen)} {}
};

class Abort {};

}  // namespace

using ASR::down_cast;
using ASR::down_cast2;
using ASR::is_a;

using LFortran::ASRUtils::determine_module_dependencies;
using LFortran::ASRUtils::EXPR2FUN;
using LFortran::ASRUtils::EXPR2SUB;
using LFortran::ASRUtils::EXPR2VAR;
using LFortran::ASRUtils::expr_type;
using LFortran::ASRUtils::intent_local;
using LFortran::ASRUtils::intent_return_var;
using LFortran::ASRUtils::is_arg_dummy;
using LFortran::ASRUtils::symbol_get_past_external;

// Platform dependent fast unique hash:
uint64_t static get_hash(ASR::asr_t *node) { return (uint64_t)node; }

struct SymbolInfo {
    bool needs_declaration = true;
    bool intrinsic_function = false;
};

class ASRToWASMVisitor : public ASR::BaseVisitor<ASRToWASMVisitor> {
   public:
    diag::Diagnostics &diag;

    std::map<uint64_t, BinaryenExpressionRef *>
        binaryen_symtab;  // binaryen_symtab_value
    // std::map<char*, BinaryenExpressionRef *>
    //     binaryen_symtab;  // binaryen_symtab_value

    BinaryenExpressionRef src;

    BinaryenModuleRef binaryen_module;

    bool intrinsic_module = false;
    const ASR::Function_t *current_function = nullptr;
    const ASR::intentType intent_local =
        ASR::intentType::Local;  // local variable (not a dummy argument)
    const ASR::intentType intent_in =
        ASR::intentType::In;  // dummy argument, intent(in)
    const ASR::intentType intent_out =
        ASR::intentType::Out;  // dummy argument, intent(out)
    const ASR::intentType intent_inout =
        ASR::intentType::InOut;  // dummy argument, intent(inout)
    const ASR::intentType intent_return_var =
        ASR::intentType::ReturnVar;  // return variable of a function
    const ASR::intentType intent_unspecified =
        ASR::intentType::Unspecified;  // dummy argument, ambiguous intent

    ASRToWASMVisitor(diag::Diagnostics &diag) : diag{diag} {}

    void visit_TranslationUnit(const ASR::TranslationUnit_t &x) {
        // create the binaryen_module
        binaryen_module = BinaryenModuleCreate();

        // the main program:
        for (auto &item : x.m_global_scope->scope) {
            if (ASR::is_a<ASR::Program_t>(*item.second)) {
                visit_symbol(*item.second);
            }
        }
    }

    void declare_var(const ASR::Variable_t &x) {
        const ASR::Variable_t *v = &x;
        if (v->m_intent == intent_local || v->m_intent == intent_return_var ||
            !v->m_intent) {
            switch (v->m_type->type) {
                case (ASR::ttypeType::Integer): {
                    ASR::Integer_t *v_type =
                        LFortran::ASR::down_cast<ASR::Integer_t>(v->m_type);
                    if (v_type->n_dims > 0) {
                        throw CodeGenError(
                            "Declare Var: Array types not yet supported");
                    } else {
                        src = BinaryenGlobalGet(binaryen_module, x.m_name, BinaryenInt32());
                        std::cout << "name = " << x.m_name << "\n";
                    }
                    break;
                }
                default:
                    throw CodeGenError(
                        "Declare Var: Only Integer types currently supported");
            }
        }
        else{
            src = nullptr;
            throw CodeGenError("Declare Var: Not a local variable or return variable");
        }
    }

    void visit_Program(const ASR::Program_t &x) {
        std::vector<BinaryenExpressionRef> childrens;
        for (auto &item : x.m_symtab->scope) {
            if (ASR::is_a<ASR::Variable_t>(*item.second)) {
                ASR::Variable_t *v =
                    ASR::down_cast<ASR::Variable_t>(item.second);
                declare_var(*v);
                childrens.push_back(src);
            }
        }

        // inner statements of main program
        for (size_t i = 0; i < x.n_body; i++) {
            std::cout << "Statement " << i + 1 << "\n";
            this->visit_stmt(*x.m_body[i]);
            childrens.push_back(src);
        }

        BinaryenExpressionRef *body_children =
            new BinaryenExpressionRef[childrens.size()];
        for (size_t i = 0; i < childrens.size(); i++) {
            body_children[i] = childrens[i];
        }

        BinaryenExpressionRef main_body_block = BinaryenBlock(
            binaryen_module, "main_body_block", body_children, x.n_body, NULL);

        BinaryenFunctionRef binaryen_main = BinaryenAddFunction(
            binaryen_module, "binaryen_main", {}, {}, NULL, 0, main_body_block);
    }

    void visit_ConstantInteger(const ASR::ConstantInteger_t &x) {
        int64_t val = x.m_n;
        int a_kind = ((ASR::Integer_t *)(&(x.m_type->base)))->m_kind;
        switch (a_kind) {
            case 4: {
                src = BinaryenConst(binaryen_module, BinaryenLiteralInt32(val));
                std::cout << "This is constant integer = " << val << "\n";
                break;
            }
            default: {
                throw CodeGenError(
                    "Constant Integer: Only kind 4 currently supported");
            }
        }
    }

     void visit_Var(const ASR::Var_t &x) {
        const ASR::symbol_t *s = ASRUtils::symbol_get_past_external(x.m_v);
        std::string name = ASR::down_cast<ASR::Variable_t>(s)->m_name;
        src = BinaryenGlobalGet(binaryen_module, name.c_str(), BinaryenInt32());
    }

    void visit_Assignment(const ASR::Assignment_t &x) {
        this->visit_expr(*x.m_value);
        if (ASR::is_a<ASR::Var_t>(*x.m_target)) {
            ASR::Variable_t *asr_target = EXPR2VAR(x.m_target);
            src = BinaryenGlobalSet(binaryen_module, asr_target->m_name, src);
            std::cout << "name is " << asr_target->m_name << " and " << x.m_target << "\n";
        } else if (ASR::is_a<ASR::ArrayRef_t>(*x.m_target)) {
            throw CodeGenError("Assignment: Arrays not yet supported");
        } else {
            LFORTRAN_ASSERT(false)
        }
    }

    void visit_BinOp(const ASR::BinOp_t &x) {
        this->visit_expr(*x.m_left);
        BinaryenExpressionRef left_val = src;
        this->visit_expr(*x.m_right);
        BinaryenExpressionRef right_val = src;
        if (ASRUtils::is_integer(*x.m_type)) {
            switch (x.m_op) {
                case ASR::binopType::Add: {
                    src = BinaryenBinary(binaryen_module, BinaryenAddInt32(),
                                         left_val, right_val);
                    std::cout << "I returned this\n";
                    break;
                };
                case ASR::binopType::Sub: {
                    src = BinaryenBinary(binaryen_module, BinaryenSubInt32(),
                                         left_val, right_val);
                    break;
                };
                case ASR::binopType::Mul: {
                    src = BinaryenBinary(binaryen_module, BinaryenMulInt32(),
                                         left_val, right_val);
                    break;
                };
                case ASR::binopType::Div: {
                    src = BinaryenBinary(binaryen_module, BinaryenDivUInt32(),
                                         left_val, right_val);
                    break;
                };
                default:
                    throw CodeGenError(
                        "Binop: Pow Operation not yet implemented");
            }
        } else {
            throw CodeGenError("Binop: Only Integer type implemented");
        }
    }
};

Result<std::string> asr_to_wasm(Allocator &al, ASR::TranslationUnit_t &asr,
                                diag::Diagnostics &diagnostics) {
    // pass_unused_functions(al, asr); // do not remove unused functions

    ASRToWASMVisitor v(diagnostics);
    try {
        v.visit_asr((ASR::asr_t &)asr);
    } catch (const CodeGenError &e) {
        diagnostics.diagnostics.push_back(e.d);
        return Error();
    } catch (const Abort &) {
        return Error();
    }
    return std::string(BinaryenModuleEmitText(v.binaryen_module));
}

}  // namespace LFortran
