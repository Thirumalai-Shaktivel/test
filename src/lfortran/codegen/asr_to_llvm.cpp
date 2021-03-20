#include <iostream>
#include <memory>
#include <unordered_map>
#include <functional>
#include <string_view>

#include <llvm/ADT/STLExtras.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/Argument.h>
#include <llvm/IR/Attributes.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Vectorize.h>
#include <llvm/ExecutionEngine/ObjectCache.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Path.h>

#include <lfortran/asr.h>
#include <lfortran/containers.h>
#include <lfortran/codegen/asr_to_llvm.h>
#include <lfortran/pass/do_loops.h>
#include <lfortran/pass/select_case.h>
#include <lfortran/pass/global_stmts.h>
#include <lfortran/exception.h>
#include <lfortran/asr_utils.h>
#include <lfortran/pickle.h>


namespace LFortran {

using ASR::is_a;
using ASR::down_cast;
using ASR::down_cast2;

// Platform dependent fast unique hash:
uint64_t static get_hash(ASR::asr_t *node)
{
    return (uint64_t)node;
}

void printf(llvm::LLVMContext &context, llvm::Module &module,
    llvm::IRBuilder<> &builder, const std::vector<llvm::Value*> &args)
{
    llvm::Function *fn_printf = module.getFunction("_lfortran_printf");
    if (!fn_printf) {
        llvm::FunctionType *function_type = llvm::FunctionType::get(
                llvm::Type::getVoidTy(context), {llvm::Type::getInt8PtrTy(context)}, true);
        fn_printf = llvm::Function::Create(function_type,
                llvm::Function::ExternalLinkage, "_lfortran_printf", &module);
    }
    builder.CreateCall(fn_printf, args);
}

void exit(llvm::LLVMContext &context, llvm::Module &module,
    llvm::IRBuilder<> &builder, llvm::Value* exit_code)
{
    llvm::Function *fn_exit = module.getFunction("exit");
    if (!fn_exit) {
        llvm::FunctionType *function_type = llvm::FunctionType::get(
                llvm::Type::getVoidTy(context), {llvm::Type::getInt64Ty(context)},
                false);
        fn_exit = llvm::Function::Create(function_type,
                llvm::Function::ExternalLinkage, "exit", &module);
    }
    builder.CreateCall(fn_exit, {exit_code});
}

class ASRToLLVMVisitor : public ASR::BaseVisitor<ASRToLLVMVisitor>
{
private:
  //!< A map from sin, cos, etc. to the corresponding functions
  std::unordered_map<std::string, llvm::Function *> all_intrinsics;
public:
    llvm::LLVMContext &context;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<llvm::IRBuilder<>> builder;

    llvm::Value *tmp;
    llvm::BasicBlock *current_loophead, *current_loopend;
    std::string mangle_prefix;
    bool prototype_only;
    llvm::StructType *complex_type;
    llvm::PointerType *character_type;

    std::map<uint64_t, llvm::Value*> llvm_symtab; // llvm_symtab_value
    std::map<uint64_t, llvm::Function*> llvm_symtab_fn;

    ASRToLLVMVisitor(llvm::LLVMContext &context) : context{context},
        prototype_only{false} {}

    /*
    * Dispatches the required function from runtime library to 
    * perform the specified binary operation.
    * 
    * @param left_arg llvm::Value* The left argument of the binary operator.
    * @param right_arg llvm::Value* The right argument of the binary operator.
    * @param runtime_func_name std::string The name of the function to be dispatched
    *                                      from runtime library.
    * @returns llvm::Value* The result of the operation.
    * 
    * Note
    * ====
    * 
    * Internally the call to this function gets transformed into a runtime call:
    * void _lfortran_complex_add(complex* a, complex* b, complex *result)
    * 
    * As of now the following values for func_name are supported,
    * 
    * _lfortran_complex_add
    * _lfortran_complex_sub
    * _lfortran_complex_div
    * _lfortran_complex_mul
    */
    llvm::Value* lfortran_complex_bin_op(llvm::Value* left_arg, llvm::Value* right_arg, 
                                         std::string runtime_func_name)
    {
        llvm::Function *fn = module->getFunction(runtime_func_name);
        if (!fn) {
            llvm::FunctionType *function_type = llvm::FunctionType::get(
                    llvm::Type::getVoidTy(context), {
                        complex_type->getPointerTo(),
                        complex_type->getPointerTo(),
                        complex_type->getPointerTo()
                    }, true);
            fn = llvm::Function::Create(function_type,
                    llvm::Function::ExternalLinkage, runtime_func_name, *module);
        }

        llvm::AllocaInst *pleft_arg = builder->CreateAlloca(complex_type,
            nullptr);
        builder->CreateStore(left_arg, pleft_arg);
        llvm::AllocaInst *pright_arg = builder->CreateAlloca(complex_type,
            nullptr);
        builder->CreateStore(right_arg, pright_arg);
        llvm::AllocaInst *presult = builder->CreateAlloca(complex_type,
            nullptr);
        std::vector<llvm::Value*> args = {pleft_arg, pright_arg, presult};
        builder->CreateCall(fn, args);
        return builder->CreateLoad(presult);
    }


    llvm::Value* lfortran_strop(llvm::Value* left_arg, llvm::Value* right_arg, 
                                         std::string runtime_func_name)
    {
        llvm::Function *fn = module->getFunction(runtime_func_name);
        if (!fn) {
            llvm::FunctionType *function_type = llvm::FunctionType::get(
                    llvm::Type::getVoidTy(context), {
                        character_type->getPointerTo(),
                        character_type->getPointerTo(),
                        character_type->getPointerTo()
                    }, false);
            fn = llvm::Function::Create(function_type,
                    llvm::Function::ExternalLinkage, runtime_func_name, *module);
        }
        llvm::AllocaInst *pleft_arg = builder->CreateAlloca(character_type,
            nullptr);
        builder->CreateStore(left_arg, pleft_arg);
        llvm::AllocaInst *pright_arg = builder->CreateAlloca(character_type,
            nullptr);
        builder->CreateStore(right_arg, pright_arg);
        llvm::AllocaInst *presult = builder->CreateAlloca(character_type,
            nullptr);
        std::vector<llvm::Value*> args = {pleft_arg, pright_arg, presult};
        builder->CreateCall(fn, args);
        return builder->CreateLoad(presult);
    }


    // This function is called as:
    // float complex_re(complex a)
    // And it extracts the real part of the complex number
    llvm::Value *complex_re(llvm::Value *c) {
        llvm::AllocaInst *pc = builder->CreateAlloca(complex_type, nullptr);
        builder->CreateStore(c, pc);
        std::vector<llvm::Value *> idx = {
            llvm::ConstantInt::get(context, llvm::APInt(32, 0)),
            llvm::ConstantInt::get(context, llvm::APInt(32, 0))};
        llvm::Value *pim = builder->CreateGEP(pc, idx);
        return builder->CreateLoad(pim);
    }

    llvm::Value *complex_im(llvm::Value *c) {
        llvm::AllocaInst *pc = builder->CreateAlloca(complex_type, nullptr);
        builder->CreateStore(c, pc);
        std::vector<llvm::Value *> idx = {
            llvm::ConstantInt::get(context, llvm::APInt(32, 0)),
            llvm::ConstantInt::get(context, llvm::APInt(32, 1))};
        llvm::Value *pim = builder->CreateGEP(pc, idx);
        return builder->CreateLoad(pim);
    }

    llvm::Value *complex_from_floats(llvm::Value *re, llvm::Value *im) {
        llvm::AllocaInst *pres = builder->CreateAlloca(complex_type, nullptr);
        std::vector<llvm::Value *> idx1 = {
            llvm::ConstantInt::get(context, llvm::APInt(32, 0)),
            llvm::ConstantInt::get(context, llvm::APInt(32, 0))};
        std::vector<llvm::Value *> idx2 = {
            llvm::ConstantInt::get(context, llvm::APInt(32, 0)),
            llvm::ConstantInt::get(context, llvm::APInt(32, 1))};
        llvm::Value *pre = builder->CreateGEP(pres, idx1);
        llvm::Value *pim = builder->CreateGEP(pres, idx2);
        builder->CreateStore(re, pre);
        builder->CreateStore(im, pim);
        return builder->CreateLoad(pres);
    }

    /**
     * @brief This function generates the
     * @detail This is converted to
     *
     *     float lfortran_KEY(float *x)
     *
     *   Where KEY can be any of the supported intrinsics; this is then
     *   transformed into a runtime call:
     *
     *     void _lfortran_KEY(float x, float *result)
     */
    llvm::Value* lfortran_intrinsic(llvm::Function *fn, llvm::Value* pa)
    {
        llvm::AllocaInst *presult = builder->CreateAlloca(
            llvm::Type::getFloatTy(context),
            nullptr);
        llvm::Value *a = builder->CreateLoad(pa);
        std::vector<llvm::Value*> args = {a, presult};
        builder->CreateCall(fn, args);
        return builder->CreateLoad(presult);
    }

    void visit_TranslationUnit(const ASR::TranslationUnit_t &x) {
        module = std::make_unique<llvm::Module>("LFortran", context);
        module->setDataLayout("");
        builder = std::make_unique<llvm::IRBuilder<>>(context);


        // All loose statements must be converted to a function, so the items
        // must be empty:
        LFORTRAN_ASSERT(x.n_items == 0);

        // Define LLVM types that we might need
        // Complex type is represented as an identified struct in LLVM
        // %complex = type { float, float }
        std::vector<llvm::Type*> els = {
            llvm::Type::getFloatTy(context),
            llvm::Type::getFloatTy(context)};
        complex_type = llvm::StructType::create(context, els, "complex");
        character_type = llvm::Type::getInt8PtrTy(context);

        // Process Variables first:
        for (auto &item : x.m_global_scope->scope) {
            if (is_a<ASR::Variable_t>(*item.second)) {
                visit_symbol(*item.second);
            }
        }

        prototype_only = true;
        // Generate function prototypes
        for (auto &item : x.m_global_scope->scope) {
            if (is_a<ASR::Function_t>(*item.second)) {
                visit_Function(*ASR::down_cast<ASR::Function_t>(item.second));
            }
            if (is_a<ASR::Subroutine_t>(*item.second)) {
                visit_Subroutine(*ASR::down_cast<ASR::Subroutine_t>(item.second));
            }
        }
        prototype_only = false;

        // Then the rest:
        for (auto &item : x.m_global_scope->scope) {
            if (!is_a<ASR::Variable_t>(*item.second)) {
                visit_symbol(*item.second);
            }
        }
    }

    void visit_Variable(const ASR::Variable_t &x) {
        uint32_t h = get_hash((ASR::asr_t*)&x);
        // This happens at global scope, so the intent can only be either local
        // (global variable declared/initialized in this translation unit), or
        // external (global variable not declared/initialized in this
        // translation unit, just referenced).
        LFORTRAN_ASSERT(x.m_intent == intent_local
            || x.m_abi == ASR::abiType::Interactive);
        bool external = (x.m_abi != ASR::abiType::Source);
        llvm::Constant* init_value = nullptr;
        if (x.m_value != nullptr){
            this->visit_expr(*x.m_value);
            init_value = llvm::dyn_cast<llvm::Constant>(tmp);
        }
        if (x.m_type->type == ASR::ttypeType::Integer) {
            llvm::Constant *ptr = module->getOrInsertGlobal(x.m_name,
                llvm::Type::getInt64Ty(context));
            if (!external) {
                if (init_value) {
                    module->getNamedGlobal(x.m_name)->setInitializer(
                            init_value);
                } else {
                    module->getNamedGlobal(x.m_name)->setInitializer(
                            llvm::ConstantInt::get(context, 
                                llvm::APInt(64, 0)));
                }
            }
            llvm_symtab[h] = ptr;
        } else if (x.m_type->type == ASR::ttypeType::Real) {
            llvm::Constant *ptr = module->getOrInsertGlobal(x.m_name,
                llvm::Type::getFloatTy(context));
            if (!external) {
                if (init_value) {
                    module->getNamedGlobal(x.m_name)->setInitializer(
                            init_value);
                } else {
                    module->getNamedGlobal(x.m_name)->setInitializer(
                            llvm::ConstantFP::get(context, 
                                llvm::APFloat((float)0)));
                }
            }
            llvm_symtab[h] = ptr;
        } else if (x.m_type->type == ASR::ttypeType::Logical) {
            llvm::Constant *ptr = module->getOrInsertGlobal(x.m_name,
                llvm::Type::getInt1Ty(context));
            if (!external) {
                if (init_value) {
                    module->getNamedGlobal(x.m_name)->setInitializer(
                            init_value);
                } else {
                    module->getNamedGlobal(x.m_name)->setInitializer(
                            llvm::ConstantInt::get(context, 
                                llvm::APInt(1, 0)));
                }
            }
            llvm_symtab[h] = ptr;
        } else {
            throw CodeGenError("Variable type not supported");
        }
    }

    void visit_Module(const ASR::Module_t &x) {
        mangle_prefix = "__module_" + std::string(x.m_name) + "_";
        for (auto &item : x.m_symtab->scope) {
            if (is_a<ASR::Subroutine_t>(*item.second)) {
                ASR::Subroutine_t *s = ASR::down_cast<ASR::Subroutine_t>(item.second);
                visit_Subroutine(*s);
            }
            if (is_a<ASR::Function_t>(*item.second)) {
                ASR::Function_t *s = ASR::down_cast<ASR::Function_t>(item.second);
                visit_Function(*s);
            }
        }
        mangle_prefix = "";
    }

    void visit_Program(const ASR::Program_t &x) {
        // Generate code for nested subroutines and functions first:
        for (auto &item : x.m_symtab->scope) {
            if (is_a<ASR::Subroutine_t>(*item.second)) {
                ASR::Subroutine_t *s = down_cast<ASR::Subroutine_t>(item.second);
                visit_Subroutine(*s);
            }
            if (is_a<ASR::Function_t>(*item.second)) {
                ASR::Function_t *s = down_cast<ASR::Function_t>(item.second);
                visit_Function(*s);
            }
        }

        // Generate code for the main program
        llvm::FunctionType *function_type = llvm::FunctionType::get(
                llvm::Type::getInt64Ty(context), {}, false);
        llvm::Function *F = llvm::Function::Create(function_type,
                llvm::Function::ExternalLinkage, "main", module.get());
        llvm::BasicBlock *BB = llvm::BasicBlock::Create(context,
                ".entry", F);
        builder->SetInsertPoint(BB);

        for (auto &item : x.m_symtab->scope) {
            if (is_a<ASR::Variable_t>(*item.second)) {
                ASR::Variable_t *v = down_cast<ASR::Variable_t>(item.second);
                uint32_t h = get_hash((ASR::asr_t*)v);
                llvm::AllocaInst *ptr;
                switch (v->m_type->type) {
                    case (ASR::ttypeType::Integer) :
                        ptr = builder->CreateAlloca(llvm::Type::getInt64Ty(context), nullptr, v->m_name);
                        break;
                    case (ASR::ttypeType::Real) : {
                        llvm::Type* typeByKind = nullptr;
                        int a_kind = ((ASR::Real_t*)(&(v->m_type->base)))->m_kind;
                        switch( a_kind )
                        {
                            case 4:
                                typeByKind = llvm::Type::getFloatTy(context);
                                break;
                            case 8:
                                typeByKind = llvm::Type::getDoubleTy(context);
                                break;
                            default:
                                throw SemanticError("Only 32 and 64 bits real kinds are supported.", 
                                                    x.base.base.loc);
                        }
                        ptr = builder->CreateAlloca(typeByKind, nullptr, v->m_name);
                        break;
                    }
                    case (ASR::ttypeType::Complex) :
                        // TODO: Assuming single precision
                        ptr = builder->CreateAlloca(complex_type, nullptr, v->m_name);
                        break;
                    case (ASR::ttypeType::Character) :
                        ptr = builder->CreateAlloca(character_type, nullptr, v->m_name);
                        break;
                    case (ASR::ttypeType::Logical) :
                        ptr = builder->CreateAlloca(llvm::Type::getInt1Ty(context), nullptr, v->m_name);
                        break;
                    case (ASR::ttypeType::Derived) :
                        throw CodeGenError("Derived type argument not implemented yet in conversion");
                        break;
                    default :
                        LFORTRAN_ASSERT(false);
                }
                llvm_symtab[h] = ptr;
                if( v->m_value != nullptr ) {
                    llvm::Value *target_var = ptr;
                    this->visit_expr(*v->m_value);
                    llvm::Value *init_value = tmp;
                    builder->CreateStore(init_value, target_var);
                }
            }
        }

        for (size_t i=0; i<x.n_body; i++) {
            this->visit_stmt(*x.m_body[i]);
        }
        llvm::Value *ret_val2 = llvm::ConstantInt::get(context,
            llvm::APInt(64, 0));
        builder->CreateRet(ret_val2);
    }

    template <typename T>
    std::vector<llvm::Type*> convert_args(const T &x) {
        std::vector<llvm::Type*> args;
        for (size_t i=0; i<x.n_args; i++) {
            ASR::Variable_t *arg = EXPR2VAR(x.m_args[i]);
            LFORTRAN_ASSERT(is_arg_dummy(arg->m_intent));
            // We pass all arguments as pointers for now
            switch (arg->m_type->type) {
                case (ASR::ttypeType::Integer) :
                    args.push_back(llvm::Type::getInt64PtrTy(context));
                    break;
                case (ASR::ttypeType::Real) :
                    args.push_back(llvm::Type::getFloatPtrTy(context));
                    break;
                case (ASR::ttypeType::Complex) :
                    throw CodeGenError("Complex argument type not implemented yet in conversion");
                    break;
                case (ASR::ttypeType::Character) :
                    throw CodeGenError("Character argument type not implemented yet in conversion");
                    break;
                case (ASR::ttypeType::Logical) :
                    args.push_back(llvm::Type::getInt1PtrTy(context));
                    break;
                case (ASR::ttypeType::Derived) :
                    throw CodeGenError("Derived type argument not implemented yet in conversion");
                    break;
                default :
                    LFORTRAN_ASSERT(false);
            }
        }
        return args;
    }

    template <typename T>
    void declare_args(const T &x, llvm::Function &F) {
        size_t i = 0;
        for (llvm::Argument &llvm_arg : F.args()) {
            ASR::Variable_t *arg = EXPR2VAR(x.m_args[i]);
            LFORTRAN_ASSERT(is_arg_dummy(arg->m_intent));
            uint32_t h = get_hash((ASR::asr_t*)arg);
            std::string arg_s = arg->m_name;
            llvm_arg.setName(arg_s);
            llvm_symtab[h] = &llvm_arg;
            i++;
        }
    }

    template <typename T>
    void declare_local_vars(const T &x) {
        for (auto &item : x.m_symtab->scope) {
            if (is_a<ASR::Variable_t>(*item.second)) {
                ASR::Variable_t *v = down_cast<ASR::Variable_t>(item.second);
                uint32_t h = get_hash((ASR::asr_t*)v);

                llvm::Type *type;
                if (v->m_intent == intent_local || v->m_intent == intent_return_var) {
                    switch (v->m_type->type) {
                        case (ASR::ttypeType::Integer) :
                            type = llvm::Type::getInt64Ty(context);
                            break;
                        case (ASR::ttypeType::Real) :
                            type = llvm::Type::getFloatTy(context);
                            break;
                        case (ASR::ttypeType::Complex) :
                            throw CodeGenError("Complex type not implemented yet");
                            break;
                        case (ASR::ttypeType::Character) :
                            throw CodeGenError("Character type not implemented yet");
                            break;
                        case (ASR::ttypeType::Logical) :
                            type = llvm::Type::getInt1Ty(context);
                            break;
                        case (ASR::ttypeType::Derived) :
                            throw CodeGenError("Derived type not implemented yet");
                            break;
                        default :
                            LFORTRAN_ASSERT(false);
                    }
                    llvm::AllocaInst *ptr = builder->CreateAlloca(
                        type, nullptr, v->m_name);
                    llvm_symtab[h] = ptr;
                }
            }
        }
    }

    void visit_Function(const ASR::Function_t &x) {
        if (x.m_abi != ASR::abiType::Source &&
            x.m_abi != ASR::abiType::Interactive) {
                return;
        }
        bool interactive = (x.m_abi == ASR::abiType::Interactive);

        uint32_t h = get_hash((ASR::asr_t*)&x);
        llvm::Function *F = nullptr;
        if (llvm_symtab_fn.find(h) != llvm_symtab_fn.end()) {
            /*
            throw CodeGenError("Function code already generated for '"
                + std::string(x.m_name) + "'");
            */
            F = llvm_symtab_fn[h];
        } else {
            ASR::ttypeType return_var_type = EXPR2VAR(x.m_return_var)->m_type->type;
            llvm::Type *return_type;
            switch (return_var_type) {
                case (ASR::ttypeType::Integer) :
                    return_type = llvm::Type::getInt64Ty(context);
                    break;
                case (ASR::ttypeType::Real) :
                    return_type = llvm::Type::getFloatTy(context);
                    break;
                case (ASR::ttypeType::Complex) :
                    throw CodeGenError("Complex return type not implemented yet");
                    break;
                case (ASR::ttypeType::Character) :
                    throw CodeGenError("Character return type not implemented yet");
                    break;
                case (ASR::ttypeType::Logical) :
                    return_type = llvm::Type::getInt1Ty(context);
                    break;
                case (ASR::ttypeType::Derived) :
                    throw CodeGenError("Derived return type not implemented yet");
                    break;
                default :
                    LFORTRAN_ASSERT(false);
            }
            std::vector<llvm::Type*> args = convert_args(x);
            llvm::FunctionType *function_type = llvm::FunctionType::get(
                    return_type, args, false);
            F = llvm::Function::Create(function_type,
                    llvm::Function::ExternalLinkage, mangle_prefix + x.m_name, module.get());
            llvm_symtab_fn[h] = F;
        }

        if (interactive) return;

        if (!prototype_only) {
            llvm::BasicBlock *BB = llvm::BasicBlock::Create(context,
                    ".entry", F);
            builder->SetInsertPoint(BB);

            declare_args(x, *F);

            declare_local_vars(x);

            for (size_t i=0; i<x.n_body; i++) {
                this->visit_stmt(*x.m_body[i]);
            }

            ASR::Variable_t *asr_retval = EXPR2VAR(x.m_return_var);
            uint32_t h = get_hash((ASR::asr_t*)asr_retval);
            llvm::Value *ret_val = llvm_symtab[h];
            llvm::Value *ret_val2 = builder->CreateLoad(ret_val);
            builder->CreateRet(ret_val2);
        }

    }

    void visit_Subroutine(const ASR::Subroutine_t &x) {
        if (x.m_abi != ASR::abiType::Source &&
            x.m_abi != ASR::abiType::Interactive) {
                return;
        }
        bool interactive = (x.m_abi == ASR::abiType::Interactive);

        uint32_t h = get_hash((ASR::asr_t*)&x);
        llvm::Function *F = nullptr;
        if (llvm_symtab_fn.find(h) != llvm_symtab_fn.end()) {
            /*
            throw CodeGenError("Subroutine code already generated for '"
                + std::string(x.m_name) + "'");
            */
            F = llvm_symtab_fn[h];
        } else {
            std::vector<llvm::Type*> args = convert_args(x);
            llvm::FunctionType *function_type = llvm::FunctionType::get(
                    llvm::Type::getVoidTy(context), args, false);
            F = llvm::Function::Create(function_type,
                    llvm::Function::ExternalLinkage, mangle_prefix + x.m_name, module.get());
            llvm_symtab_fn[h] = F;
        }

        if (interactive) return;

        if (!prototype_only) {
            llvm::BasicBlock *BB = llvm::BasicBlock::Create(context,
                    ".entry", F);
            builder->SetInsertPoint(BB);

            declare_args(x, *F);

            declare_local_vars(x);

            for (size_t i=0; i<x.n_body; i++) {
                this->visit_stmt(*x.m_body[i]);
            }

            builder->CreateRetVoid();
        }

    }

    void visit_Assignment(const ASR::Assignment_t &x) {
        //this->visit_expr(*x.m_target);
        ASR::Variable_t *asr_target = EXPR2VAR(x.m_target);
        uint32_t h = get_hash((ASR::asr_t*)asr_target);
        llvm::Value *target = llvm_symtab[h];
        this->visit_expr(*x.m_value);
        llvm::Value *value=tmp;
        builder->CreateStore(value, target);

    }

    void visit_Compare(const ASR::Compare_t &x) {
        this->visit_expr(*x.m_left);
        llvm::Value *left = tmp;
        this->visit_expr(*x.m_right);
        llvm::Value *right = tmp;
        LFORTRAN_ASSERT(expr_type(x.m_left)->type == expr_type(x.m_right)->type);
        ASR::ttypeType optype = expr_type(x.m_left)->type;
        if (optype == ASR::ttypeType::Integer) {
            switch (x.m_op) {
                case (ASR::cmpopType::Eq) : {
                    tmp = builder->CreateICmpEQ(left, right);
                    break;
                }
                case (ASR::cmpopType::Gt) : {
                    tmp = builder->CreateICmpSGT(left, right);
                    break;
                }
                case (ASR::cmpopType::GtE) : {
                    tmp = builder->CreateICmpSGE(left, right);
                    break;
                }
                case (ASR::cmpopType::Lt) : {
                    tmp = builder->CreateICmpSLT(left, right);
                    break;
                }
                case (ASR::cmpopType::LtE) : {
                    tmp = builder->CreateICmpSLE(left, right);
                    break;
                }
                case (ASR::cmpopType::NotEq) : {
                    tmp = builder->CreateICmpNE(left, right);
                    break;
                }
                default : {
                    throw SemanticError("Comparison operator not implemented",
                            x.base.base.loc);
                }
            }
        } else if (optype == ASR::ttypeType::Real) {
            switch (x.m_op) {
                case (ASR::cmpopType::Eq) : {
                    tmp = builder->CreateFCmpUEQ(left, right);
                    break;
                }
                case (ASR::cmpopType::Gt) : {
                    tmp = builder->CreateFCmpUGT(left, right);
                    break;
                }
                case (ASR::cmpopType::GtE) : {
                    tmp = builder->CreateFCmpUGE(left, right);
                    break;
                }
                case (ASR::cmpopType::Lt) : {
                    tmp = builder->CreateFCmpULT(left, right);
                    break;
                }
                case (ASR::cmpopType::LtE) : {
                    tmp = builder->CreateFCmpULE(left, right);
                    break;
                }
                case (ASR::cmpopType::NotEq) : {
                    tmp = builder->CreateFCmpUNE(left, right);
                    break;
                }
                default : {
                    throw SemanticError("Comparison operator not implemented",
                            x.base.base.loc);
                }
            }
        } else {
            throw CodeGenError("Only Integer and Real implemented in Compare");
        }
    }

    void visit_If(const ASR::If_t &x) {
        this->visit_expr(*x.m_test);
        llvm::Value *cond=tmp;
        llvm::Function *fn = builder->GetInsertBlock()->getParent();
        llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(context, "then", fn);
        llvm::BasicBlock *elseBB = llvm::BasicBlock::Create(context, "else");
        llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(context, "ifcont");
        builder->CreateCondBr(cond, thenBB, elseBB);
        builder->SetInsertPoint(thenBB);
        for (size_t i=0; i<x.n_body; i++) {
            this->visit_stmt(*x.m_body[i]);
        }
        llvm::Value *thenV = llvm::ConstantInt::get(context, llvm::APInt(64, 1));
        builder->CreateBr(mergeBB);
        thenBB = builder->GetInsertBlock();
        fn->getBasicBlockList().push_back(elseBB);
        builder->SetInsertPoint(elseBB);
        for (size_t i=0; i<x.n_orelse; i++) {
            this->visit_stmt(*x.m_orelse[i]);
        }
        llvm::Value *elseV = llvm::ConstantInt::get(context, llvm::APInt(64, 2));
        builder->CreateBr(mergeBB);
        elseBB = builder->GetInsertBlock();
        fn->getBasicBlockList().push_back(mergeBB);
        builder->SetInsertPoint(mergeBB);
        llvm::PHINode *PN = builder->CreatePHI(llvm::Type::getInt64Ty(context), 2,
                                        "iftmp");
        PN->addIncoming(thenV, thenBB);
        PN->addIncoming(elseV, elseBB);
    }

    void visit_WhileLoop(const ASR::WhileLoop_t &x) {
        llvm::Function *fn = builder->GetInsertBlock()->getParent();
        llvm::BasicBlock *loophead = llvm::BasicBlock::Create(context, "loop.head", fn);
        llvm::BasicBlock *loopbody = llvm::BasicBlock::Create(context, "loop.body");
        llvm::BasicBlock *loopend = llvm::BasicBlock::Create(context, "loop.end");
        this->current_loophead = loophead;
        this->current_loopend = loopend;

        // head
        builder->CreateBr(loophead);
        builder->SetInsertPoint(loophead);
        this->visit_expr(*x.m_test);
        llvm::Value *cond = tmp;
        builder->CreateCondBr(cond, loopbody, loopend);

        // body
        fn->getBasicBlockList().push_back(loopbody);
        builder->SetInsertPoint(loopbody);
        for (size_t i=0; i<x.n_body; i++) {
            this->visit_stmt(*x.m_body[i]);
        }
        builder->CreateBr(loophead);

        // end
        fn->getBasicBlockList().push_back(loopend);
        builder->SetInsertPoint(loopend);
    }

    void visit_Exit(const ASR::Exit_t & /* x */) {
        llvm::Function *fn = builder->GetInsertBlock()->getParent();
        llvm::BasicBlock *after = llvm::BasicBlock::Create(context, "after", fn);
        builder->CreateBr(current_loopend);
        builder->SetInsertPoint(after);
    }

    void visit_Cycle(const ASR::Cycle_t & /* x */) {
        llvm::Function *fn = builder->GetInsertBlock()->getParent();
        llvm::BasicBlock *after = llvm::BasicBlock::Create(context, "after", fn);
        builder->CreateBr(current_loophead);
        builder->SetInsertPoint(after);
    }

    void visit_BoolOp(const ASR::BoolOp_t &x) {
        this->visit_expr(*x.m_left);
        llvm::Value *left_val = tmp;
        this->visit_expr(*x.m_right);
        llvm::Value *right_val = tmp;
        if (x.m_type->type == ASR::ttypeType::Logical) {
            switch (x.m_op) {
                case ASR::boolopType::And: {
                    tmp = builder->CreateAnd(left_val, right_val);
                    break;
                };
                case ASR::boolopType::Or: {
                    tmp = builder->CreateOr(left_val, right_val);
                    break;
                };
                case ASR::boolopType::NEqv: {
                    tmp = builder->CreateXor(left_val, right_val);
                    break;
                };
                case ASR::boolopType::Eqv: {
                    tmp = builder->CreateXor(left_val, right_val);
                    tmp = builder->CreateNot(tmp);
                };
            } 
        } else {
            throw CodeGenError("Boolop: Only Logical types can be used with logical operators.");
        }
    }

    void visit_StrOp(const ASR::StrOp_t &x) {
        this->visit_expr(*x.m_left);
        llvm::Value *left_val = tmp;
        this->visit_expr(*x.m_right);
        llvm::Value *right_val = tmp;
        switch (x.m_op) {
            case ASR::stropType::Concat: {
                tmp = lfortran_strop(left_val, right_val, "_lfortran_strcat");
                break;
            };
        }
    }

    void visit_BinOp(const ASR::BinOp_t &x) {
        this->visit_expr(*x.m_left);
        llvm::Value *left_val = tmp;
        this->visit_expr(*x.m_right);
        llvm::Value *right_val = tmp;
        if (x.m_type->type == ASR::ttypeType::Integer) {
            switch (x.m_op) {
                case ASR::binopType::Add: {
                    tmp = builder->CreateAdd(left_val, right_val);
                    break;
                };
                case ASR::binopType::Sub: {
                    tmp = builder->CreateSub(left_val, right_val);
                    break;
                };
                case ASR::binopType::Mul: {
                    tmp = builder->CreateMul(left_val, right_val);
                    break;
                };
                case ASR::binopType::Div: {
                    tmp = builder->CreateUDiv(left_val, right_val);
                    break;
                };
                case ASR::binopType::Pow: {
                    llvm::Value *fleft = builder->CreateSIToFP(left_val,
                            llvm::Type::getFloatTy(context));
                    llvm::Value *fright = builder->CreateSIToFP(right_val,
                            llvm::Type::getFloatTy(context));

                    llvm::Function *fn_pow = module->getFunction("llvm.pow.f32");
                    if (!fn_pow) {
                        llvm::FunctionType *function_type = llvm::FunctionType::get(
                                llvm::Type::getFloatTy(context), {
                                    llvm::Type::getFloatTy(context),
                                    llvm::Type::getFloatTy(context)
                                }, false);
                        fn_pow = llvm::Function::Create(function_type,
                                llvm::Function::ExternalLinkage, "llvm.pow.f32",
                                module.get());
                    }
                    tmp = builder->CreateCall(fn_pow, {fleft, fright});
                    tmp = builder->CreateFPToSI(tmp, llvm::Type::getInt64Ty(context));
                    break;
                };
            }
        } else if (x.m_type->type == ASR::ttypeType::Real) {
            switch (x.m_op) {
                case ASR::binopType::Add: {
                    tmp = builder->CreateFAdd(left_val, right_val);
                    break;
                };
                case ASR::binopType::Sub: {
                    tmp = builder->CreateFSub(left_val, right_val);
                    break;
                };
                case ASR::binopType::Mul: {
                    tmp = builder->CreateFMul(left_val, right_val);
                    break;
                };
                case ASR::binopType::Div: {
                    tmp = builder->CreateFDiv(left_val, right_val);
                    break;
                };
                case ASR::binopType::Pow: {
                    llvm::Function *fn_pow = module->getFunction("llvm.pow.f32");
                    if (!fn_pow) {
                        llvm::FunctionType *function_type = llvm::FunctionType::get(
                                llvm::Type::getFloatTy(context), {
                                    llvm::Type::getFloatTy(context),
                                    llvm::Type::getFloatTy(context)
                                }, false);
                        fn_pow = llvm::Function::Create(function_type,
                                llvm::Function::ExternalLinkage, "llvm.pow.f32",
                                module.get());
                    }
                    tmp = builder->CreateCall(fn_pow, {left_val, right_val});
                    break;
                };
            }
        } else if (x.m_type->type == ASR::ttypeType::Complex) {
            switch (x.m_op) {
                case ASR::binopType::Add: {
                    tmp = lfortran_complex_bin_op(left_val, right_val, "_lfortran_complex_add");
                    break;
                };
                case ASR::binopType::Sub: {
                    tmp = lfortran_complex_bin_op(left_val, right_val, "_lfortran_complex_sub");
                    break;
                };
                case ASR::binopType::Mul: {
                    tmp = lfortran_complex_bin_op(left_val, right_val, "_lfortran_complex_mul");
                    break;
                };
                case ASR::binopType::Div: {
                    tmp = lfortran_complex_bin_op(left_val, right_val, "_lfortran_complex_div");
                    break;
                };
                case ASR::binopType::Pow: {
                    tmp = lfortran_complex_bin_op(left_val, right_val, "_lfortran_complex_pow");
                    break;
                };
            }
        } else {
            throw CodeGenError("Binop: Only Real, Integer and Complex types implemented");
        }
    }

    void visit_UnaryOp(const ASR::UnaryOp_t &x) {
        this->visit_expr(*x.m_operand);
        if (x.m_type->type == ASR::ttypeType::Integer) {
            if (x.m_op == ASR::unaryopType::UAdd) {
                // tmp = tmp;
                return;
            } else if (x.m_op == ASR::unaryopType::USub) {
                llvm::Value *zero = llvm::ConstantInt::get(context,
                    llvm::APInt(64, 0));
                tmp = builder ->CreateSub(zero, tmp);
                return;
            } else {
                throw CodeGenError("Unary type not implemented yet");
            }
        } else if (x.m_type->type == ASR::ttypeType::Real) {
            if (x.m_op == ASR::unaryopType::UAdd) {
                // tmp = tmp;
                return;
            } else if (x.m_op == ASR::unaryopType::USub) {
                llvm::Value *zero = llvm::ConstantFP::get(context,
                        llvm::APFloat((float)0.0));
                tmp = builder ->CreateFSub(zero, tmp);
                return;
            } else {
                throw CodeGenError("Unary type not implemented yet");
            }
        } else if (x.m_type->type == ASR::ttypeType::Logical) {
            if (x.m_op == ASR::unaryopType::Not) {
                tmp = builder ->CreateNot(tmp);
                return;
            } else {
                throw CodeGenError("Unary type not implemented yet in Logical");
            }
        } else {
            throw CodeGenError("UnaryOp: type not supported yet");
        }
    }

    void visit_ConstantInteger(const ASR::ConstantInteger_t &x) {
        tmp = llvm::ConstantInt::get(context, llvm::APInt(64, x.m_n));
    }

    void visit_ConstantReal(const ASR::ConstantReal_t &x) {
        double val = std::atof(x.m_r);
        int a_kind = ((ASR::Real_t*)(&(x.m_type->base)))->m_kind;
        switch( a_kind ) {
            
            case 4 : {
                tmp = llvm::ConstantFP::get(context, llvm::APFloat((float)val));
                break;
            }
            case 8 : {
                tmp = llvm::ConstantFP::get(context, llvm::APFloat(val));
                break;
            }
            default : {
                break;
            }

        }
        
    }

    void visit_ConstantComplex(const ASR::ConstantComplex_t &x) {
        LFORTRAN_ASSERT(ASR::is_a<ASR::ConstantReal_t>(*x.m_re));
        LFORTRAN_ASSERT(ASR::is_a<ASR::ConstantReal_t>(*x.m_im));
        double re = std::atof(
            ASR::down_cast<ASR::ConstantReal_t>(x.m_re)->m_r);
        double im = std::atof(
            ASR::down_cast<ASR::ConstantReal_t>(x.m_im)->m_r);
        // TODO: assuming single precision
        llvm::Value *re2 = llvm::ConstantFP::get(context, llvm::APFloat((float)re));
        llvm::Value *im2 = llvm::ConstantFP::get(context, llvm::APFloat((float)im));
        tmp = complex_from_floats(re2, im2);
    }

    void visit_ConstantLogical(const ASR::ConstantLogical_t &x) {
        int val;
        if (x.m_value == true) {
            val = 1;
        } else {
            val = 0;
        }
        tmp = llvm::ConstantInt::get(context, llvm::APInt(1, val));
    }


    void visit_Str(const ASR::Str_t &x) {
        tmp = builder->CreateGlobalStringPtr(x.m_s);
    }

    void visit_Var(const ASR::Var_t &x) {
        ASR::Variable_t *v = ASR::down_cast<ASR::Variable_t>(x.m_v);
        uint32_t h = get_hash((ASR::asr_t*)v);
        LFORTRAN_ASSERT(llvm_symtab.find(h) != llvm_symtab.end());
        llvm::Value *ptr = llvm_symtab[h];
        tmp = builder->CreateLoad(ptr);
    }

    inline int extract_kind_from_ttype_t(const ASR::ttype_t* curr_type) {
        if( curr_type == nullptr ) {
            return -1;
        }
        switch (curr_type->type) {
            case ASR::ttypeType::Real : {
                return ((ASR::Real_t*)(&(curr_type->base)))->m_kind;
            }
            default : {
                return -1;
            }
        }
    }

    inline ASR::ttype_t* extract_ttype_t_from_expr(ASR::expr_t* expr) {
        ASR::asr_t* base = &(expr->base);
        switch( expr->type ) {
            case ASR::exprType::ConstantReal : {
                return ((ASR::ConstantReal_t*)base)->m_type;
            }
            case ASR::exprType::BinOp : {
                return ((ASR::BinOp_t*)base)->m_type;
            }
            default : {
                return nullptr;
            }
        }
    }

    void extract_kinds(const ASR::ImplicitCast_t& x, 
                       int& arg_kind, int& dest_kind)
    {   
        dest_kind = extract_kind_from_ttype_t(x.m_type);
        ASR::ttype_t* curr_type = extract_ttype_t_from_expr(x.m_arg);
        arg_kind = extract_kind_from_ttype_t(curr_type);
    }

    void visit_ImplicitCast(const ASR::ImplicitCast_t &x) {
        visit_expr(*x.m_arg);
        switch (x.m_kind) {
            case (ASR::cast_kindType::IntegerToReal) : {
                int a_kind = extract_kind_from_ttype_t(x.m_type);
                switch (a_kind) {
                    case 4 : {
                        tmp = builder->CreateSIToFP(tmp, llvm::Type::getFloatTy(context));
                        break;
                    }
                    case 8 : {
                        tmp = builder->CreateSIToFP(tmp, llvm::Type::getDoubleTy(context));
                        break;
                    }
                    default : {
                        throw SemanticError(R"""(Only 32 and 64 bit real kinds are implemented)""", 
                                            x.base.base.loc);
                    }
                }
                break;
            }
            case (ASR::cast_kindType::RealToInteger) : {
                tmp = builder->CreateFPToSI(tmp, llvm::Type::getInt64Ty(context));
                break;
            }
            case (ASR::cast_kindType::RealToComplex) : {
                llvm::Value *zero = llvm::ConstantFP::get(context,
                        llvm::APFloat((float)0.0));
                tmp = complex_from_floats(tmp, zero);
                break;
            }
            case (ASR::cast_kindType::IntegerToComplex) : {
                tmp = builder->CreateSIToFP(tmp, llvm::Type::getFloatTy(context));
                llvm::Value *zero = llvm::ConstantFP::get(context,
                        llvm::APFloat((float)0.0));
                tmp = complex_from_floats(tmp, zero);
                break;
            }
            case (ASR::cast_kindType::IntegerToLogical) : {
                tmp = builder->CreateICmpNE(tmp, builder->getInt64(0));
                break;
            }
            case (ASR::cast_kindType::RealToReal) : {
                int arg_kind = -1, dest_kind = -1;
                extract_kinds(x, arg_kind, dest_kind);
                if( arg_kind > 0 && dest_kind > 0 )
                {
                    if( arg_kind == 4 && dest_kind == 8 ) {
                        tmp = builder->CreateFPExt(tmp, llvm::Type::getDoubleTy(context));
                    } else if( arg_kind == 8 && dest_kind == 4 ) {
                        tmp = builder->CreateFPTrunc(tmp, llvm::Type::getFloatTy(context));
                    } else {
                        std::string msg = "Conversion from " + std::to_string(arg_kind) + 
                                          " to " + std::to_string(dest_kind) + " not implemented yet.";
                        throw CodeGenError(msg);
                    }
                }
                break;
            }
            default : throw CodeGenError("Cast kind not implemented");
        }
    }

    void visit_Print(const ASR::Print_t &x) {
        std::vector<llvm::Value *> args;
        std::vector<std::string> fmt;
        for (size_t i=0; i<x.n_values; i++) {
            this->visit_expr(*x.m_values[i]);
            ASR::expr_t *v = x.m_values[i];
            ASR::ttype_t *t = expr_type(v);
            if (t->type == ASR::ttypeType::Integer || 
                t->type == ASR::ttypeType::Logical) {
                fmt.push_back("%d");
                args.push_back(tmp);
            } else if (t->type == ASR::ttypeType::Real) {
                int a_kind = ((ASR::Real_t*)(&(t->base)))->m_kind;
                llvm::Value *d;
                switch( a_kind ) {
                    case 4 : {
                        // Cast float to double as a workaround for the fact that
                        // vprintf() seems to cast to double even for %f, which
                        // causes it to print 0.000000.
                        fmt.push_back("%f");
                        d = builder->CreateFPExt(tmp,
                        llvm::Type::getDoubleTy(context));
                        break;
                    }
                    case 8 : {
                        fmt.push_back("%lf");
                        d = builder->CreateFPExt(tmp,
                        llvm::Type::getDoubleTy(context));
                        break;
                    }
                    default: {
                        throw SemanticError(R"""(Printing support is available only 
                                            for 32, and 64 bit real kinds.)""", 
                                            x.base.base.loc);
                    }
                }
                args.push_back(d);
            } else if (t->type == ASR::ttypeType::Character) {
                fmt.push_back("%s");
                args.push_back(tmp);
            } else if (t->type == ASR::ttypeType::Complex) {
                fmt.push_back("(%f,%f)");
                llvm::Value *d = builder->CreateFPExt(complex_re(tmp),
                        llvm::Type::getDoubleTy(context));
                args.push_back(d);
                d = builder->CreateFPExt(complex_im(tmp),
                        llvm::Type::getDoubleTy(context));
                args.push_back(d);
            } else {
                throw LFortranException("Type not implemented");
            }
        }
        std::string fmt_str;
        for (size_t i=0; i<fmt.size(); i++) {
            fmt_str += fmt[i];
            if (i < fmt.size()-1) fmt_str += " ";
        }
        fmt_str += "\n";
        llvm::Value *fmt_ptr = builder->CreateGlobalStringPtr(fmt_str);
        std::vector<llvm::Value *> printf_args;
        printf_args.push_back(fmt_ptr);
        printf_args.insert(printf_args.end(), args.begin(), args.end());
        printf(context, *module, *builder, printf_args);
    }

    void visit_Stop(const ASR::Stop_t & /* x */) {
        llvm::Value *fmt_ptr = builder->CreateGlobalStringPtr("STOP\n");
        printf(context, *module, *builder, {fmt_ptr});
        int exit_code_int = 0;
        llvm::Value *exit_code = llvm::ConstantInt::get(context,
                llvm::APInt(64, exit_code_int));
        exit(context, *module, *builder, exit_code);
    }

    void visit_ErrorStop(const ASR::ErrorStop_t & /* x */) {
        llvm::Value *fmt_ptr = builder->CreateGlobalStringPtr("ERROR STOP\n");
        printf(context, *module, *builder, {fmt_ptr});
        int exit_code_int = 1;
        llvm::Value *exit_code = llvm::ConstantInt::get(context,
                llvm::APInt(64, exit_code_int));
        exit(context, *module, *builder, exit_code);
    }

    template <typename T>
    std::vector<llvm::Value*> convert_call_args(const T &x) {
        std::vector<llvm::Value *> args;
        for (size_t i=0; i<x.n_args; i++) {
            if (x.m_args[i]->type == ASR::exprType::Var) {
                ASR::Variable_t *arg = EXPR2VAR(x.m_args[i]);
                uint32_t h = get_hash((ASR::asr_t*)arg);
                tmp = llvm_symtab[h];
            } else {
                this->visit_expr(*x.m_args[i]);
                llvm::Value *value=tmp;
                llvm::Type *target_type;
                switch (expr_type(x.m_args[i])->type) {
                    case (ASR::ttypeType::Integer) :
                        target_type = llvm::Type::getInt64Ty(context);
                        break;
                    case (ASR::ttypeType::Real) :
                        target_type = llvm::Type::getFloatTy(context);
                        break;
                    case (ASR::ttypeType::Complex) :
                        target_type = complex_type;
                        break;
                    case (ASR::ttypeType::Character) :
                        throw CodeGenError("Character argument type not implemented yet in conversion");
                        break;
                    case (ASR::ttypeType::Logical) :
                        target_type = llvm::Type::getInt1Ty(context);
                        break;
                    case (ASR::ttypeType::Derived) :
                        throw CodeGenError("Derived type argument not implemented yet in conversion");
                        break;
                    default :
                        LFORTRAN_ASSERT(false);
                }
                llvm::AllocaInst *target = builder->CreateAlloca(
                    target_type, nullptr);
                builder->CreateStore(value, target);
                tmp = target;
            }
            args.push_back(tmp);
        }
        return args;
    }

    void visit_SubroutineCall(const ASR::SubroutineCall_t &x) {
        ASR::Subroutine_t *s = ASR::down_cast<ASR::Subroutine_t>(x.m_name);
        uint32_t h;
        if (s->m_abi == ASR::abiType::LFortranModule) {
            throw CodeGenError("Subroutine LFortran interfaces not implemented yet");
        } else if (s->m_abi == ASR::abiType::Interactive) {
            h = get_hash((ASR::asr_t*)s);
        } else if (s->m_abi == ASR::abiType::Source) {
            h = get_hash((ASR::asr_t*)s);
        } else {
            throw CodeGenError("External type not implemented yet.");
        }
        if (llvm_symtab_fn.find(h) == llvm_symtab_fn.end()) {
            throw CodeGenError("Subroutine code not generated for '"
                + std::string(s->m_name) + "'");
        }
        llvm::Function *fn = llvm_symtab_fn[h];
        std::vector<llvm::Value *> args = convert_call_args(x);
        builder->CreateCall(fn, args);
    }

    void visit_FuncCall(const ASR::FuncCall_t &x) {
        ASR::Function_t *s = ASR::down_cast<ASR::Function_t>(x.m_name);
        uint32_t h;
        if (s->m_abi == ASR::abiType::Source) {
            h = get_hash((ASR::asr_t*)s);
        } else if (s->m_abi == ASR::abiType::LFortranModule) {
            throw CodeGenError("Function LFortran interfaces not implemented yet");
        } else if (s->m_abi == ASR::abiType::Interactive) {
            h = get_hash((ASR::asr_t*)s);
        } else if (s->m_abi == ASR::abiType::Intrinsic) {
            if (all_intrinsics.empty()) {
                populate_intrinsics();
            }
            // We use an unordered map to get the O(n) operation time
            std::unordered_map<std::string, llvm::Function *>::const_iterator
                find_intrinsic = all_intrinsics.find(s->m_name);
            if (find_intrinsic == all_intrinsics.end()) {
                throw CodeGenError("Intrinsic not implemented yet.");
            } else {
                std::vector<llvm::Value *> args = convert_call_args(x);
                LFORTRAN_ASSERT(args.size() == 1);
                tmp = lfortran_intrinsic(find_intrinsic->second, args[0]);
                return;
            }
            h = get_hash((ASR::asr_t *)s);
        } else {
            throw CodeGenError("External type not implemented yet.");
        }
        if (llvm_symtab_fn.find(h) == llvm_symtab_fn.end()) {
            throw CodeGenError("Function code not generated for '"
                + std::string(s->m_name) + "'");
        }
        llvm::Function *fn = llvm_symtab_fn[h];
        std::vector<llvm::Value *> args = convert_call_args(x);
        tmp = builder->CreateCall(fn, args);
    }

    //!< Meant to be called only once
    void populate_intrinsics() {
        std::vector<std::string> supported = {
            "sin",  "cos",  "tan",  "sinh",  "cosh",  "tanh",
            "asin", "acos", "atan", "asinh", "acosh", "atanh"};

        for (auto sv : supported) {
            auto fname = "_lfortran_" + sv;
            llvm::Function *fn = module->getFunction(fname);
            if (!fn) {
              llvm::FunctionType *function_type =
                  llvm::FunctionType::get(llvm::Type::getVoidTy(context),
                                          {llvm::Type::getFloatTy(context),
                                           llvm::Type::getFloatPtrTy(context)},
                                          false);
              fn = llvm::Function::Create(
                  function_type, llvm::Function::ExternalLinkage, fname, *module);
            }
            all_intrinsics[sv] = fn;
        }
        return;
    }
};



std::unique_ptr<LLVMModule> asr_to_llvm(ASR::TranslationUnit_t &asr,
        llvm::LLVMContext &context, Allocator &al, std::string run_fn)
{
    ASRToLLVMVisitor v(context);
    pass_wrap_global_stmts_into_function(al, asr, run_fn);

    // Uncomment for debugging the ASR after the transformation
    // std::cout << pickle(asr) << std::endl;

    pass_replace_do_loops(al, asr);
    pass_replace_select_case(al, asr);
    v.visit_asr((ASR::asr_t&)asr);
    std::string msg;
    llvm::raw_string_ostream err(msg);
    if (llvm::verifyModule(*v.module, &err)) {
        std::string buf;
        llvm::raw_string_ostream os(buf);
        v.module->print(os, nullptr);
        std::cout << os.str();
        throw CodeGenError("asr_to_llvm: module failed verification. Error:\n"
            + err.str());
    };
    return std::make_unique<LLVMModule>(std::move(v.module));
}

} // namespace LFortran
