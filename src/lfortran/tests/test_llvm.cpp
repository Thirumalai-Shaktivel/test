#include <iostream>

#include <llvm/IR/LLVMContext.h>


int main() {
    std::cout << "LLVM Version:" << std::endl;
    llvm::cl::PrintVersionMessage();

    /*
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
    */
    /*
    auto context = std::make_shared<llvm::LLVMContext>();
    std::unique_ptr<llvm::Module> module
        = llvm::make_unique<llvm::Module>("SymEngine", *context.get());
    module->setDataLayout("");
    auto mod = module.get();
    */
    return 0;
}
