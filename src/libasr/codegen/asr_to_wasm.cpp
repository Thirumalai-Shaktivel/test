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

using LFortran::ASRUtils::EXPR2VAR;


class ASRToWASMVisitor : public ASR::BaseVisitor<ASRToWASMVisitor> {
   public:
    diag::Diagnostics &diag;

    std::map<std::string, int> get_var_index;
    std::map<std::string, BinaryenType> get_func_return_type;
    ASR::Variable_t *return_var;
    BinaryenType return_type;
    BinaryenExpressionRef expr_src;

    BinaryenModuleRef binaryen_module;

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

    void visit_Program(const ASR::Program_t &x) {
        for (auto &item : x.m_symtab->scope) {
            if (ASR::is_a<ASR::Subroutine_t>(*item.second)) {
                throw CodeGenError("Sub Routine not yet supported");
            }
            if (ASR::is_a<ASR::Function_t>(*item.second)) {
                ASR::Function_t *s =
                    ASR::down_cast<ASR::Function_t>(item.second);
                visit_Function(*s);  // function themselves add to module and it
                                     // also sets expr_src = NULL
            }
        }
    }

    void visit_Function(const ASR::Function_t &x) {
        return_var = LFortran::ASRUtils::EXPR2VAR(x.m_return_var);
        if (ASRUtils::is_integer(*return_var->m_type)) {
            bool is_int =
                ASR::down_cast<ASR::Integer_t>(return_var->m_type)->m_kind == 4;
            if (is_int) {
                return_type = BinaryenTypeInt32();
            } else {
                return_type = BinaryenTypeInt64();
            }
        } else {
            throw CodeGenError("Return type not supported");
        }

        std::vector<BinaryenType> func_params;
        int curIdx = 0;
        for (size_t i = 0; i < x.n_args; i++) {
            ASR::Variable_t *arg = LFortran::ASRUtils::EXPR2VAR(x.m_args[i]);
            LFORTRAN_ASSERT(LFortran::ASRUtils::is_arg_dummy(arg->m_intent));
            if (arg->m_type->type == ASR::ttypeType::Integer) {
                // checking for array is currently omitted
                func_params.push_back(BinaryenTypeInt32());
                get_var_index[arg->m_name] = curIdx++;
            } else {
                throw CodeGenError(
                    "Parameters other than integer not yet supported");
            }
        }

        std::vector<BinaryenType> func_local_vars;

        for (auto &item : x.m_symtab->scope) {
            if (ASR::is_a<ASR::Variable_t>(*item.second)) {
                ASR::Variable_t *v =
                    ASR::down_cast<ASR::Variable_t>(item.second);
                if (v->m_intent == LFortran::ASRUtils::intent_local ||
                    v->m_intent == LFortran::ASRUtils::intent_return_var) {
                    if (v->m_type->type == ASR::ttypeType::Integer) {
                        func_local_vars.push_back(BinaryenTypeInt32());
                        get_var_index[v->m_name] = curIdx++;
                    } else {
                        throw CodeGenError(
                            "Variables other than integer not yet supported");
                    }
                }
            }
        }

        std::vector<BinaryenExpressionRef> body_children;

        for (size_t i = 0; i < x.n_body; i++) {
            this->visit_stmt(*x.m_body[i]);
            body_children.push_back(expr_src);
        }

        // check if body_child.size() > 0 or not
        BinaryenExpressionRef func_body_block =
            BinaryenBlock(binaryen_module, "body_block", body_children.data(),
                          body_children.size(), return_type);

        BinaryenFunctionRef func = BinaryenAddFunction(
            binaryen_module, x.m_name,
            BinaryenTypeCreate(func_params.data(), func_params.size()),
            return_type, func_local_vars.data(), func_local_vars.size(),
            func_body_block);

        get_func_return_type[x.m_name] = return_type;

        BinaryenAddFunctionExport(binaryen_module, x.m_name, x.m_name);

        expr_src = nullptr;
    }

    void visit_Assignment(const ASR::Assignment_t &x) {
        this->visit_expr(*x.m_value);
        BinaryenExpressionRef value = expr_src;
        // this->visit_expr(*x.m_target);
        // BinaryenExpressionRef target = expr_src;
        if (ASR::is_a<ASR::Var_t>(*x.m_target)) {
            ASR::Variable_t *asr_target = EXPR2VAR(x.m_target);
            expr_src = BinaryenLocalSet(binaryen_module,
                                        get_var_index[asr_target->m_name], value);
        } else if (ASR::is_a<ASR::ArrayRef_t>(*x.m_target)) {
            throw CodeGenError("Assignment: Arrays not yet supported");
        } else {
            LFORTRAN_ASSERT(false)
        }
    }

    void visit_BinOp(const ASR::BinOp_t &x) {
        this->visit_expr(*x.m_left);
        BinaryenExpressionRef left_val = expr_src;
        this->visit_expr(*x.m_right);
        BinaryenExpressionRef right_val = expr_src;
        if (ASRUtils::is_integer(*x.m_type)) {
            switch (x.m_op) {
                case ASR::binopType::Add: {
                    expr_src =
                        BinaryenBinary(binaryen_module, BinaryenAddInt32(),
                                       left_val, right_val);
                    break;
                };
                case ASR::binopType::Sub: {
                    expr_src =
                        BinaryenBinary(binaryen_module, BinaryenSubInt32(),
                                       left_val, right_val);
                    break;
                };
                case ASR::binopType::Mul: {
                    expr_src =
                        BinaryenBinary(binaryen_module, BinaryenMulInt32(),
                                       left_val, right_val);
                    break;
                };
                case ASR::binopType::Div: {
                    expr_src =
                        BinaryenBinary(binaryen_module, BinaryenDivUInt32(),
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

    void visit_Var(const ASR::Var_t &x) {
        const ASR::symbol_t *s = ASRUtils::symbol_get_past_external(x.m_v);
        auto v = ASR::down_cast<ASR::Variable_t>(s);
        BinaryenType var_type;
        switch (v->m_type->type) {
            case ASR::ttypeType::Integer:
                // currently omitting for 64 bit integers
                var_type = BinaryenTypeInt32();
                break;

            default:
                throw CodeGenError("Variable type not supported");
        }
        expr_src =
            BinaryenLocalGet(binaryen_module, get_var_index[v->m_name], var_type);
    }

    void visit_Return(const ASR::Return_t & /* x */) {
        expr_src = BinaryenLocalGet(binaryen_module,
                                    get_var_index[return_var->m_name], return_type);
    }

    void visit_ConstantInteger(const ASR::ConstantInteger_t &x) {
        int64_t val = x.m_n;
        int a_kind = ((ASR::Integer_t *)(&(x.m_type->base)))->m_kind;
        switch (a_kind) {
            case 4: {
                expr_src =
                    BinaryenConst(binaryen_module, BinaryenLiteralInt32(val));
                break;
            }
            default: {
                throw CodeGenError(
                    "Constant Integer: Only kind 4 currently supported");
            }
        }
    }

    void visit_FunctionCall(const ASR::FunctionCall_t &x) {
        ASR::Function_t *fn = ASR::down_cast<ASR::Function_t>(
            LFortran::ASRUtils::symbol_get_past_external(x.m_name));

        std::vector<BinaryenExpressionRef> func_arguments;
        int curIdx = 0;
        for (size_t i = 0; i < x.n_args; i++) {
            visit_expr(*x.m_args[i].m_value);
            func_arguments.push_back(expr_src);
        }

        expr_src = BinaryenCall(binaryen_module, fn->m_name, func_arguments.data(), func_arguments.size(), get_func_return_type[std::string(fn->m_name)]);
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
