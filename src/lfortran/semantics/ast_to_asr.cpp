#include <iostream>
#include <map>
#include <memory>

#include <lfortran/ast.h>
#include <lfortran/asr.h>
#include <lfortran/asr_utils.h>
#include <lfortran/pickle.h>
#include <lfortran/semantics/ast_to_asr.h>
#include <lfortran/parser/parser_stype.h>
#include <string>

#define num_types 6

namespace LFortran {

    class ImplicitCastRules {

        private:

            //! Default case when no conversion is needed.
            static const int defaultCase = -1;
            //! Error case when conversion is not possible or is illegal.
            static const int errorCase = -2;
            static const int integerToReal = ASR::cast_kindType::IntegerToReal;
            static const int realToInteger = ASR::cast_kindType::RealToInteger;
            static const int realToComplex = ASR::cast_kindType::RealToComplex;

            //! Stores the variable part of error messages to be passed to SemanticError.
            static constexpr const char* type_names[num_types][2] = {
                {"Integer", ""}, 
                {"Real", "Integer or Real"}, 
                {"Complex", ""},
                {"Character", ""},
                {"Logical", ""},
                {"Derived", ""}
            }; 

            /*
            * Rule map for performing implicit cast represented by a 2D integer array.
            * 
            * Key is the pair of indices with row index denoting the source type
            * and column index denoting the destination type.
            */
            static constexpr const int ruleMap[num_types][num_types] = {

                {defaultCase, integerToReal, errorCase, errorCase, errorCase, errorCase},
                {realToInteger, defaultCase, realToComplex, defaultCase, defaultCase, defaultCase},
                {defaultCase, defaultCase, defaultCase, defaultCase, defaultCase, defaultCase},
                {defaultCase, defaultCase, defaultCase, defaultCase, defaultCase, defaultCase},
                {defaultCase, defaultCase, defaultCase, defaultCase, defaultCase, defaultCase},
                {defaultCase, defaultCase, defaultCase, defaultCase, defaultCase, defaultCase}

            };

            /*
            * Priority of different types to be used in conversion
            * when source and destination are directly not deducible.
            */
            static constexpr const int typePriority[num_types] = {
                4, // Integer
                5, // Real
                6, // Complex
                -1, // Character
                -1, // Logical
                -1 // Derived
            };

        public:

            /*
            * Adds ImplicitCast node if necessary.
            * 
            * @param al Allocator&
            * @param a_loc Location&
            * @param convertCan ASR::expr_t** Address of the pointer to 
            *                                 conversion candidate.
            * @param sourceType ASR::ttype_t* Source type.
            * @param destType AST::ttype_t* Destination type.
            */
            static void setConvertedValue
            (Allocator &al, const Location &a_loc, 
             ASR::expr_t** convertCan, ASR::ttype_t* sourceType, ASR::ttype_t* destType) {
                int cast_kind = ruleMap[sourceType->type][destType->type];
                if( cast_kind == errorCase )
                {
                    std::string allowed_types_str = type_names[destType->type][1];
                    std::string dest_type_str = type_names[destType->type][0];
                    std::string error_msg = "Only " + allowed_types_str + 
                                            " can be assigned to " + dest_type_str;
                    throw SemanticError(error_msg, a_loc);
                }
                else if( cast_kind != defaultCase )
                {
                    *convertCan = (ASR::expr_t*) ASR::make_ImplicitCast_t(
                        al, a_loc, *convertCan, (ASR::cast_kindType) cast_kind, 
                        destType
                    );
                }
            }

            /*
            * Deduces the candidate which is to be casted
            * based on the priority of types.
            * 
            * @param left ASR::expr_t** Address of the pointer to left 
            *                           element in the operation.
            * @param right ASR::expr_t** Address of the pointer to right
            *                            element in the operation.
            * @param left_type ASR::ttype_t* Pointer to the type of left element.
            * @param right_type ASR::ttype_t* Pointer to the type of right element.
            * @param conversionCand ASR::expr_t**& Reference to the address of
            *                                      the pointer of conversion
            *                                      candidate.
            * @param sourceType ASR::ttype_t** For storing the address of pointer
            *                                  to source type.
            * @param destType ASR::ttype_t** For stroing the address of pointer to
            *                                destination type.
            * 
            * Note
            * ====
            * 
            * Address of pointer have been used so that the contents 
            * of the pointer variables are modified which are then 
            * used in making the node of different operations. If directly
            * the pointer values are used, then no effect on left or right
            * is observed and ASR construction fails.
            */
            static void findConversionCandidate
            (ASR::expr_t** left, ASR::expr_t** right,
             ASR::ttype_t* left_type, ASR::ttype_t* right_type,
             ASR::expr_t** &conversionCand, 
             ASR::ttype_t** sourceType, ASR::ttype_t** destType) {

                int left_type_p = typePriority[left_type->type];
                int right_type_p = typePriority[right_type->type];
                if( left_type_p >= right_type_p ) {
                    conversionCand = right;
                    *sourceType = right_type;
                    *destType = left_type;
                }
                else {
                    conversionCand = left;
                    *sourceType = left_type;
                    *destType = right_type;
                }
            }

    };

class SymbolTableVisitor : public AST::BaseVisitor<SymbolTableVisitor>
{
public:
    ASR::asr_t *asr;
    Allocator &al;
    SymbolTable *current_scope;

    SymbolTableVisitor(Allocator &al, SymbolTable *symbol_table)
        : al{al}, current_scope{symbol_table} { }

    void visit_TranslationUnit(const AST::TranslationUnit_t &x) {
        if (!current_scope) {
            current_scope = al.make_new<SymbolTable>(nullptr);
        }
        LFORTRAN_ASSERT(current_scope != nullptr);
        for (size_t i=0; i<x.n_items; i++) {
            AST::astType t = x.m_items[i]->type;
            if (t != AST::astType::expr && t != AST::astType::stmt) {
                visit_ast(*x.m_items[i]);
            }
        }
        asr = ASR::make_TranslationUnit_t(al, x.base.base.loc,
            current_scope, nullptr, 0);
    }

    void visit_Module(const AST::Module_t &x) {
        SymbolTable *parent_scope = current_scope;
        current_scope = al.make_new<SymbolTable>(parent_scope);
        for (size_t i=0; i<x.n_use; i++) {
            visit_unit_decl1(*x.m_use[i]);
        }
        for (size_t i=0; i<x.n_decl; i++) {
            visit_unit_decl2(*x.m_decl[i]);
        }
        for (size_t i=0; i<x.n_contains; i++) {
            visit_program_unit(*x.m_contains[i]);
        }
        asr = ASR::make_Module_t(
            al, x.base.base.loc,
            /* a_symtab */ current_scope,
            /* a_name */ x.m_name);
        std::string sym_name = x.m_name;
        if (parent_scope->scope.find(sym_name) != parent_scope->scope.end()) {
            throw SemanticError("Module already defined", asr->loc);
        }
        parent_scope->scope[sym_name] = ASR::down_cast<ASR::symbol_t>(asr);
        current_scope = parent_scope;
    }

    void visit_Program(const AST::Program_t &x) {
        SymbolTable *parent_scope = current_scope;
        current_scope = al.make_new<SymbolTable>(parent_scope);
        for (size_t i=0; i<x.n_use; i++) {
            visit_unit_decl1(*x.m_use[i]);
        }
        for (size_t i=0; i<x.n_decl; i++) {
            visit_unit_decl2(*x.m_decl[i]);
        }
        for (size_t i=0; i<x.n_contains; i++) {
            visit_program_unit(*x.m_contains[i]);
        }
        asr = ASR::make_Program_t(
            al, x.base.base.loc,
            /* a_symtab */ current_scope,
            /* a_name */ x.m_name,
            /* a_body */ nullptr,
            /* n_body */ 0);
        std::string sym_name = x.m_name;
        if (parent_scope->scope.find(sym_name) != parent_scope->scope.end()) {
            throw SemanticError("Program already defined", asr->loc);
        }
        parent_scope->scope[sym_name] = ASR::down_cast<ASR::symbol_t>(asr);
        current_scope = parent_scope;
    }

    void visit_Subroutine(const AST::Subroutine_t &x) {
        SymbolTable *parent_scope = current_scope;
        current_scope = al.make_new<SymbolTable>(parent_scope);
        for (size_t i=0; i<x.n_decl; i++) {
            visit_unit_decl2(*x.m_decl[i]);
        }
        Vec<ASR::expr_t*> args;
        args.reserve(al, x.n_args);
        for (size_t i=0; i<x.n_args; i++) {
            char *arg=x.m_args[i].m_arg;
            std::string arg_s = arg;
            if (current_scope->scope.find(arg_s) == current_scope->scope.end()) {
                throw SemanticError("Dummy argument '" + arg_s + "' not defined", x.base.base.loc);
            }
            ASR::symbol_t *var = current_scope->scope[arg_s];
            args.push_back(al, EXPR(ASR::make_Var_t(al, x.base.base.loc,
                var)));
        }
        asr = ASR::make_Subroutine_t(
            al, x.base.base.loc,
            /* a_symtab */ current_scope,
            /* a_name */ x.m_name,
            /* a_args */ args.p,
            /* n_args */ args.size(),
            /* a_body */ nullptr,
            /* n_body */ 0,
            /* a_bind */ nullptr,
            nullptr);
        std::string sym_name = x.m_name;
        if (parent_scope->scope.find(sym_name) != parent_scope->scope.end()) {
            ASR::symbol_t *f1 = parent_scope->scope[sym_name];
            ASR::Subroutine_t *f2 = ASR::down_cast<ASR::Subroutine_t>(f1);
            if (f2->m_external && f2->m_external->m_type == ASR::proc_external_typeType::Interactive) {
                // Previous declaration will be shadowed
            } else {
                throw SemanticError("Subroutine already defined", asr->loc);
            }
        }
        parent_scope->scope[sym_name] = ASR::down_cast<ASR::symbol_t>(asr);
        current_scope = parent_scope;
    }

    void visit_Function(const AST::Function_t &x) {
        // Extract local (including dummy) variables first
        SymbolTable *parent_scope = current_scope;
        current_scope = al.make_new<SymbolTable>(parent_scope);
        for (size_t i=0; i<x.n_decl; i++) {
            visit_unit_decl2(*x.m_decl[i]);
        }

        // Convert and check arguments
        Vec<ASR::expr_t*> args;
        args.reserve(al, x.n_args);
        for (size_t i=0; i<x.n_args; i++) {
            char *arg=x.m_args[i].m_arg;
            std::string arg_s = arg;
            if (current_scope->scope.find(arg_s) == current_scope->scope.end()) {
                throw SemanticError("Dummy argument '" + arg_s + "' not defined", x.base.base.loc);
            }
            ASR::symbol_t *var = current_scope->scope[arg_s];
            args.push_back(al, EXPR(ASR::make_Var_t(al, x.base.base.loc,
                var)));
        }

        // Handle the return variable and type
        // First determine the name of the variable: either the function name
        // or result(...)
        char *return_var_name;
        if (x.m_return_var) {
            if (x.m_return_var->type == AST::exprType::Name) {
                return_var_name = ((AST::Name_t*)(x.m_return_var))->m_id;
            } else {
                throw SemanticError("Return variable must be an identifier",
                    x.m_return_var->base.loc);
            }
        } else {
            return_var_name = x.m_name;
        }

        // Determine the type of the variable, the type is either specified as
        //     integer function f()
        // or in local variables as
        //     integer :: f
        ASR::asr_t *return_var;
        if (current_scope->scope.find(std::string(return_var_name)) == current_scope->scope.end()) {
            // The variable is not defined among local variables, extract the
            // type from "integer function f()" and add the variable.
            ASR::ttype_t *type;
            if (!x.m_return_type) {
                throw SemanticError("Return type not specified",
                        x.base.base.loc);
            }
            std::string return_type = x.m_return_type;
            if (return_type == "integer") {
                type = TYPE(ASR::make_Integer_t(al, x.base.base.loc, 4, nullptr, 0));
            } else if (return_type == "real") {
                type = TYPE(ASR::make_Real_t(al, x.base.base.loc, 4, nullptr, 0));
            } else if (return_type == "logical") {
                type = TYPE(ASR::make_Logical_t(al, x.base.base.loc, 4, nullptr, 0));
            } else {
                throw SemanticError("Return type not supported",
                        x.base.base.loc);
            }
            // Add it as a local variable:
            return_var = ASR::make_Variable_t(al, x.base.base.loc,
                current_scope, return_var_name, intent_return_var, type);
            current_scope->scope[std::string(return_var_name)]
                = ASR::down_cast<ASR::symbol_t>(return_var);
        } else {
            if (x.m_return_type) {
                throw SemanticError("Cannot specify the return type twice",
                    x.base.base.loc);
            }
            // Extract the variable from the local scope
            return_var = (ASR::asr_t*) current_scope->scope[std::string(return_var_name)];
            ASR::down_cast2<ASR::Variable_t>(return_var)->m_intent = intent_return_var;
        }

        ASR::asr_t *return_var_ref = ASR::make_Var_t(al, x.base.base.loc,
            ASR::down_cast<ASR::symbol_t>(return_var));

        // Create and register the function
        asr = ASR::make_Function_t(
            al, x.base.base.loc,
            /* a_symtab */ current_scope,
            /* a_name */ x.m_name,
            /* a_args */ args.p,
            /* n_args */ args.size(),
            /* a_body */ nullptr,
            /* n_body */ 0,
            /* a_bind */ nullptr,
            /* a_return_var */ EXPR(return_var_ref),
            /* a_module */ nullptr);
        std::string sym_name = x.m_name;
        if (parent_scope->scope.find(sym_name) != parent_scope->scope.end()) {
            ASR::symbol_t *f1 = parent_scope->scope[sym_name];
            ASR::Function_t *f2 = ASR::down_cast<ASR::Function_t>(f1);
            if (f2->m_external && f2->m_external->m_type == ASR::proc_external_typeType::Interactive) {
                // Previous declaration will be shadowed
            } else {
                throw SemanticError("Function already defined", asr->loc);
            }
        }
        parent_scope->scope[sym_name] = ASR::down_cast<ASR::symbol_t>(asr);
        current_scope = parent_scope;
    }

    void visit_Declaration(const AST::Declaration_t &x) {
        for (size_t i=0; i<x.n_vars; i++) {
            this->visit_decl(x.m_vars[i]);
        }
    }

    void visit_Use(const AST::Use_t &x) {
        std::string msym = x.m_module;
        ASR::symbol_t *t = current_scope->parent->resolve_symbol(msym);
        if (!t) {
            throw SemanticError("Module '" + msym + "' not declared",
                x.base.base.loc);
        }
        if (!ASR::is_a<ASR::Module_t>(*t)) {
            throw SemanticError("The symbol '" + msym + "' must be a module",
                x.base.base.loc);
        }
        ASR::Module_t *m = ASR::down_cast<ASR::Module_t>(t);
        if (x.n_symbols == 0) {
            // Import all symbols from the module, e.g.:
            //     use a
            for (auto &item : m->m_symtab->scope) {
                // TODO: only import "public" symbols from the module
                if (ASR::is_a<ASR::Subroutine_t>(*item.second)) {
                    ASR::Subroutine_t *msub = ASR::down_cast<ASR::Subroutine_t>(item.second);
                    ASR::proc_external_t *external = al.make_new<ASR::proc_external_t>();
                    external->m_type = ASR::proc_external_typeType::LFortranModule;
                    external->m_module_proc = (ASR::symbol_t*)msub;
                    ASR::asr_t *sub = ASR::make_Subroutine_t(
                        al, msub->base.base.loc,
                        /* a_symtab */ msub->m_symtab,
                        /* a_name */ msub->m_name,
                        /* a_args */ msub->m_args,
                        /* n_args */ msub->n_args,
                        /* a_body */ nullptr,
                        /* n_body */ 0,
                        /* a_bind */ msub->m_bind,
                        /* a_external */ external
                        );
                    std::string sym = msub->m_name;
                    current_scope->scope[sym] = ASR::down_cast<ASR::symbol_t>(sub);
                } else if (ASR::is_a<ASR::Function_t>(*item.second)) {
                    ASR::Function_t *mfn = ASR::down_cast<ASR::Function_t>(item.second);
                    ASR::proc_external_t *external = al.make_new<ASR::proc_external_t>();
                    external->m_type = ASR::proc_external_typeType::LFortranModule;
                    external->m_module_proc = (ASR::symbol_t*)mfn;
                    ASR::asr_t *fn = ASR::make_Function_t(
                        al, mfn->base.base.loc,
                        /* a_symtab */ mfn->m_symtab,
                        /* a_name */ mfn->m_name,
                        /* a_args */ mfn->m_args,
                        /* n_args */ mfn->n_args,
                        /* a_body */ nullptr,
                        /* n_body */ 0,
                        /* a_bind */ mfn->m_bind,
                        /* a_return_var */ mfn->m_return_var,
                        /* a_external */ external
                        );
                    std::string sym = mfn->m_name;
                    current_scope->scope[sym] = ASR::down_cast<ASR::symbol_t>(fn);
                } else {
                    throw LFortranException("Only function / subroutine implemented");
                }
            }
        } else {
            // Only import individual symbols from the module, e.g.:
            //     use a, only: x, y, z
            for (size_t i = 0; i < x.n_symbols; i++) {
                std::string remote_sym = AST::down_cast<AST::UseSymbol_t>(x.m_symbols[i])->m_sym;
                std::string local_sym;
                if (AST::down_cast<AST::UseSymbol_t>(x.m_symbols[i])->m_rename) {
                    local_sym = AST::down_cast<AST::UseSymbol_t>(x.m_symbols[i])->m_rename;
                } else {
                    local_sym = remote_sym;
                }
                ASR::symbol_t *t = m->m_symtab->resolve_symbol(remote_sym);
                if (!t) {
                    throw SemanticError("The symbol '" + remote_sym + "' not found in the module '" + msym + "'",
                        x.base.base.loc);
                }
                if (ASR::is_a<ASR::Subroutine_t>(*t)) {
                    if (current_scope->scope.find(local_sym) != current_scope->scope.end()) {
                        throw SemanticError("Subroutine already defined",
                            x.base.base.loc);
                    }
                    ASR::Subroutine_t *msub = ASR::down_cast<ASR::Subroutine_t>(t);
                    // `msub` is the Subroutine in a module. Now we construct
                    // a new Subroutine that is just the prototype, and that links to
                    // `msub` via the `external` field.
                    ASR::proc_external_t *external = al.make_new<ASR::proc_external_t>();
                    external->m_type = ASR::proc_external_typeType::LFortranModule;
                    external->m_module_proc = (ASR::symbol_t*)msub;
                    Str name;
                    name.from_str(al, local_sym);
                    ASR::asr_t *sub = ASR::make_Subroutine_t(
                        al, msub->base.base.loc,
                        /* a_symtab */ msub->m_symtab,
                        /* a_name */ name.c_str(al),
                        /* a_args */ msub->m_args,
                        /* n_args */ msub->n_args,
                        /* a_body */ nullptr,
                        /* n_body */ 0,
                        /* a_bind */ msub->m_bind,
                        /* a_external */ external
                        );
                    current_scope->scope[local_sym] = ASR::down_cast<ASR::symbol_t>(sub);
                } else if (ASR::is_a<ASR::Function_t>(*t)) {
                    if (current_scope->scope.find(local_sym) != current_scope->scope.end()) {
                        throw SemanticError("Function already defined",
                            x.base.base.loc);
                    }
                    ASR::Function_t *mfn = ASR::down_cast<ASR::Function_t>(t);
                    // `msub` is the Function in a module. Now we construct
                    // a new Function that is just the prototype, and that links to
                    // `mfn` via the `external` field.
                    ASR::proc_external_t *external = al.make_new<ASR::proc_external_t>();
                    external->m_type = ASR::proc_external_typeType::LFortranModule;
                    external->m_module_proc = (ASR::symbol_t*)mfn;
                    Str name;
                    name.from_str(al, local_sym);
                    ASR::asr_t *fn = ASR::make_Function_t(
                        al, mfn->base.base.loc,
                        /* a_symtab */ mfn->m_symtab,
                        /* a_name */ name.c_str(al),
                        /* a_args */ mfn->m_args,
                        /* n_args */ mfn->n_args,
                        /* a_body */ nullptr,
                        /* n_body */ 0,
                        /* a_bind */ mfn->m_bind,
                        /* a_return_var */ mfn->m_return_var,
                        /* a_external */ external
                        );
                    current_scope->scope[local_sym] = ASR::down_cast<ASR::symbol_t>(fn);
                } else {
                    throw LFortranException("Only Subroutines and Functions supported in 'use'");
                }
            }
        }
    }

    void visit_decl(const AST::decl_t &x) {
        std::string sym = x.m_sym;
        std::string sym_type = x.m_sym_type;
        if (current_scope->scope.find(sym) == current_scope->scope.end()) {
            ASR::intentType s_intent=intent_local;
            Vec<ASR::dimension_t> dims;
            dims.reserve(al, x.n_dims);
            if (x.n_attrs > 0) {
                AST::Attribute_t *a = (AST::Attribute_t*)(x.m_attrs[0]);
                if (std::string(a->m_name) == "intent") {
                    if (a->n_args > 0) {
                        std::string intent = std::string(a->m_args[0].m_arg);
                        if (intent == "in") {
                            s_intent = intent_in;
                        } else if (intent == "out") {
                            s_intent = intent_out;
                        } else if (intent == "inout") {
                            s_intent = intent_inout;
                        } else {
                            throw SemanticError("Incorrect intent specifier", x.loc);
                        }
                    } else {
                        throw SemanticError("intent() is empty. Must specify intent", x.loc);
                    }
                }
                if (std::string(a->m_name) == "dimension") {
                    if (x.n_dims > 0) {
                        throw SemanticError("Cannot specify dimensions both ways", x.loc);
                    }
                    dims.reserve(al, a->n_dims);
                    for (size_t i=0; i<a->n_dims; i++) {
                        ASR::dimension_t dim;
                        if (a->m_dims[i].m_start) {
                            this->visit_expr(*a->m_dims[i].m_start);
                            dim.m_start = EXPR(asr);
                        } else {
                            dim.m_start = nullptr;
                        }
                        if (a->m_dims[i].m_end) {
                            this->visit_expr(*a->m_dims[i].m_end);
                            dim.m_end = EXPR(asr);
                        } else {
                            dim.m_end = nullptr;
                        }
                        dims.push_back(al, dim);
                    }
                }
            }
            for (size_t i=0; i<x.n_dims; i++) {
                ASR::dimension_t dim;
                if (x.m_dims[i].m_start) {
                    this->visit_expr(*x.m_dims[i].m_start);
                    dim.m_start = EXPR(asr);
                } else {
                    dim.m_start = nullptr;
                }
                if (x.m_dims[i].m_end) {
                    this->visit_expr(*x.m_dims[i].m_end);
                    dim.m_end = EXPR(asr);
                } else {
                    dim.m_end = nullptr;
                }
                dims.push_back(al, dim);
            }
            ASR::ttype_t *type;
            if (sym_type == "real") {
                type = TYPE(ASR::make_Real_t(al, x.loc, 4, dims.p, dims.size()));
            } else if (sym_type == "integer") {
                type = TYPE(ASR::make_Integer_t(al, x.loc, 4, dims.p, dims.size()));
            } else if (sym_type == "logical") {
                type = TYPE(ASR::make_Logical_t(al, x.loc, 4, dims.p, dims.size()));
            } else if (sym_type == "complex") {
                type = TYPE(ASR::make_Complex_t(al, x.loc, 4, dims.p, dims.size()));
            } else if (sym_type == "character") {
                type = TYPE(ASR::make_Character_t(al, x.loc, 4, dims.p, dims.size()));
            } else {
                throw SemanticError("Unsupported type: " + sym_type, x.loc);
            }
            ASR::asr_t *v = ASR::make_Variable_t(al, x.loc, current_scope,
                x.m_sym, s_intent, type);
            current_scope->scope[sym] = ASR::down_cast<ASR::symbol_t>(v);

        }
    }

    void visit_Num(const AST::Num_t &x) {
        ASR::ttype_t *type = TYPE(ASR::make_Integer_t(al, x.base.base.loc,
                8, nullptr, 0));
        asr = ASR::make_ConstantInteger_t(al, x.base.base.loc, x.m_n, type);
    }

};

class BodyVisitor : public AST::BaseVisitor<BodyVisitor>
{
public:
    Allocator &al;
    ASR::asr_t *asr, *tmp;
    SymbolTable *current_scope;
    BodyVisitor(Allocator &al, ASR::asr_t *unit) : al{al}, asr{unit} {}

    void visit_TranslationUnit(const AST::TranslationUnit_t &x) {
        ASR::TranslationUnit_t *unit = ASR::down_cast2<ASR::TranslationUnit_t>(asr);
        current_scope = unit->m_global_scope;
        Vec<ASR::asr_t*> items;
        items.reserve(al, x.n_items);
        for (size_t i=0; i<x.n_items; i++) {
            tmp = nullptr;
            visit_ast(*x.m_items[i]);
            if (tmp) {
                items.push_back(al, tmp);
            }
        }
        unit->m_items = items.p;
        unit->n_items = items.size();
    }

    void visit_Declaration(const AST::Declaration_t & /* x */) {
        // This AST node was already visited in SymbolTableVisitor
    }

    void visit_Module(const AST::Module_t &x) {
        SymbolTable *old_scope = current_scope;
        ASR::symbol_t *t = current_scope->scope[std::string(x.m_name)];
        ASR::Module_t *v = ASR::down_cast<ASR::Module_t>(t);
        current_scope = v->m_symtab;

        for (size_t i=0; i<x.n_contains; i++) {
            visit_program_unit(*x.m_contains[i]);
        }

        current_scope = old_scope;
        tmp = nullptr;
    }

    void visit_Program(const AST::Program_t &x) {
        SymbolTable *old_scope = current_scope;
        ASR::symbol_t *t = current_scope->scope[std::string(x.m_name)];
        ASR::Program_t *v = ASR::down_cast<ASR::Program_t>(t);
        current_scope = v->m_symtab;

        Vec<ASR::stmt_t*> body;
        body.reserve(al, x.n_body);
        for (size_t i=0; i<x.n_body; i++) {
            this->visit_stmt(*x.m_body[i]);
            ASR::stmt_t *stmt = STMT(tmp);
            body.push_back(al, stmt);
        }
        v->m_body = body.p;
        v->n_body = body.size();

        for (size_t i=0; i<x.n_contains; i++) {
            visit_program_unit(*x.m_contains[i]);
        }

        current_scope = old_scope;
        tmp = nullptr;
    }

    void visit_Subroutine(const AST::Subroutine_t &x) {
    // TODO: add SymbolTable::lookup_symbol(), which will automatically return
    // an error
    // TODO: add SymbolTable::get_symbol(), which will only check in Debug mode
        SymbolTable *old_scope = current_scope;
        ASR::symbol_t *t = current_scope->scope[std::string(x.m_name)];
        ASR::Subroutine_t *v = ASR::down_cast<ASR::Subroutine_t>(t);
        current_scope = v->m_symtab;
        Vec<ASR::stmt_t*> body;
        body.reserve(al, x.n_body);
        for (size_t i=0; i<x.n_body; i++) {
            this->visit_stmt(*x.m_body[i]);
            ASR::stmt_t *stmt = STMT(tmp);
            body.push_back(al, stmt);
        }
        v->m_body = body.p;
        v->n_body = body.size();
        current_scope = old_scope;
        tmp = nullptr;
    }

    void visit_Function(const AST::Function_t &x) {
        SymbolTable *old_scope = current_scope;
        ASR::symbol_t *t = current_scope->scope[std::string(x.m_name)];
        ASR::Function_t *v = ASR::down_cast<ASR::Function_t>(t);
        current_scope = v->m_symtab;
        Vec<ASR::stmt_t*> body;
        body.reserve(al, x.n_body);
        for (size_t i=0; i<x.n_body; i++) {
            this->visit_stmt(*x.m_body[i]);
            ASR::stmt_t *stmt = STMT(tmp);
            body.push_back(al, stmt);
        }
        v->m_body = body.p;
        v->n_body = body.size();
        current_scope = old_scope;
        tmp = nullptr;
    }

    void visit_Assignment(const AST::Assignment_t &x) {
        this->visit_expr(*x.m_target);
        ASR::expr_t *target = EXPR(tmp);
        ASR::ttype_t *target_type = expr_type(target);
        if( target->type != ASR::exprType::Var && 
            target->type != ASR::exprType::ArrayRef )
        {
            throw SemanticError(
                "The LHS of assignment can only be a variable or an array reference",
                x.base.base.loc
            );
        }

        this->visit_expr(*x.m_value);
        ASR::expr_t *value = EXPR(tmp);
        ASR::ttype_t *value_type = expr_type(value);
        if (target->type == ASR::exprType::Var) {

            ImplicitCastRules::setConvertedValue(al, x.base.base.loc, &value, 
                                                 value_type, target_type);

        }
        tmp = ASR::make_Assignment_t(al, x.base.base.loc, target, value);
    }

    Vec<ASR::expr_t*> visit_expr_list(AST::fnarg_t *ast_list, size_t n) {
        Vec<ASR::expr_t*> asr_list;
        asr_list.reserve(al, n);
        for (size_t i=0; i<n; i++) {
            visit_expr(*ast_list[i].m_end);
            ASR::expr_t *expr = EXPR(tmp);
            asr_list.push_back(al, expr);
        }
        return asr_list;
    }

    void visit_SubroutineCall(const AST::SubroutineCall_t &x) {
        ASR::Subroutine_t *sub = resolve_subroutine(x.base.base.loc, x.m_name);
        Vec<ASR::expr_t*> args = visit_expr_list(x.m_args, x.n_args);
        tmp = ASR::make_SubroutineCall_t(al, x.base.base.loc,
                (ASR::symbol_t*)sub, args.p, args.size());
    }

    void visit_Compare(const AST::Compare_t &x) {
        this->visit_expr(*x.m_left);
        ASR::expr_t *left = EXPR(tmp);
        this->visit_expr(*x.m_right);
        ASR::expr_t *right = EXPR(tmp);
        // Cast LHS or RHS if necessary
        ASR::ttype_t *left_type = expr_type(left);
        ASR::ttype_t *right_type = expr_type(right);
        if( (left_type->type != ASR::ttypeType::Real && 
            left_type->type != ASR::ttypeType::Integer) &&
            (right_type->type != ASR::ttypeType::Real &&
             right_type->type != ASR::ttypeType::Integer) ) {
            throw SemanticError(
                "Compare: only Integer or Real can be on the LHS and RHS", 
            x.base.base.loc);
        }
        else
        {
            ASR::expr_t **conversionCand = &left;
            ASR::ttype_t *destType = right_type;
            ASR::ttype_t *sourceType = left_type;
            ImplicitCastRules::findConversionCandidate
            (&left, &right, left_type, right_type, 
             conversionCand, &sourceType, &destType);

            ImplicitCastRules::setConvertedValue
            (al, x.base.base.loc, conversionCand, 
             sourceType, destType);
        }

        LFORTRAN_ASSERT(expr_type(left)->type == expr_type(right)->type);
        ASR::ttype_t *type = TYPE(ASR::make_Logical_t(al, x.base.base.loc,
                4, nullptr, 0));
        ASR::cmpopType asr_op;
        switch (x.m_op) {
            case (AST::cmpopType::Eq) : { asr_op = ASR::cmpopType::Eq; break;}
            case (AST::cmpopType::Gt) : { asr_op = ASR::cmpopType::Gt; break;}
            case (AST::cmpopType::GtE) : { asr_op = ASR::cmpopType::GtE; break;}
            case (AST::cmpopType::Lt) : { asr_op = ASR::cmpopType::Lt; break;}
            case (AST::cmpopType::LtE) : { asr_op = ASR::cmpopType::LtE; break;}
            case (AST::cmpopType::NotEq) : { asr_op = ASR::cmpopType::NotEq; break;}
            default : {
                throw SemanticError("Comparison operator not implemented",
                        x.base.base.loc);
            }
        }
        tmp = ASR::make_Compare_t(al, x.base.base.loc,
            left, asr_op, right, type);
    }

    void visit_BinOp(const AST::BinOp_t &x) {
        this->visit_expr(*x.m_left);
        ASR::expr_t *left = EXPR(tmp);
        this->visit_expr(*x.m_right);
        ASR::expr_t *right = EXPR(tmp);
        ASR::binopType op;
        switch (x.m_op) {
            case (AST::Add) :
                op = ASR::Add;
                break;
            case (AST::Sub) :
                op = ASR::Sub;
                break;
            case (AST::Mul) :
                op = ASR::Mul;
                break;
            case (AST::Div) :
                op = ASR::Div;
                break;
            case (AST::Pow) :
                op = ASR::Pow;
                break;
            // Fix compiler warning:
            default : { LFORTRAN_ASSERT(false); op = ASR::binopType::Pow; }
        }

        // Cast LHS or RHS if necessary
        ASR::ttype_t *left_type = expr_type(left);
        ASR::ttype_t *right_type = expr_type(right);
        ASR::expr_t **conversionCand = &left;
        ASR::ttype_t *sourceType = left_type;
        ASR::ttype_t *destType = right_type;

        ImplicitCastRules::findConversionCandidate(
            &left, &right, left_type, right_type, 
            conversionCand, &sourceType, &destType);
        ImplicitCastRules::setConvertedValue(
            al, x.base.base.loc, conversionCand,
            sourceType, destType);

        LFORTRAN_ASSERT(expr_type(left)->type == expr_type(right)->type);
        tmp = ASR::make_BinOp_t(al, x.base.base.loc,
                left, op, right, destType);
    }

    void visit_UnaryOp(const AST::UnaryOp_t &x) {
        this->visit_expr(*x.m_operand);
        ASR::expr_t *operand = EXPR(tmp);
        ASR::unaryopType op;
        switch (x.m_op) {
            case (AST::unaryopType::Invert) :
                op = ASR::unaryopType::Invert;
                break;
            case (AST::unaryopType::Not) :
                op = ASR::unaryopType::Not;
                break;
            case (AST::unaryopType::UAdd) :
                op = ASR::unaryopType::UAdd;
                break;
            case (AST::unaryopType::USub) :
                op = ASR::unaryopType::USub;
                break;
            // Fix compiler warning:
            default : { LFORTRAN_ASSERT(false); op = ASR::unaryopType::Invert; }
        }
        ASR::ttype_t *operand_type = expr_type(operand);
        tmp = ASR::make_UnaryOp_t(al, x.base.base.loc,
                op, operand, operand_type);
    }

    ASR::asr_t* resolve_variable(const Location &loc, const char* id) {
        SymbolTable *scope = current_scope;
        std::string var_name = id;
        ASR::symbol_t *v = scope->resolve_symbol(var_name);
        if (!v) {
            throw SemanticError("Variable '" + var_name + "' not declared", loc);
        }
        return ASR::make_Var_t(al, loc, v);
    }

    ASR::Subroutine_t* resolve_subroutine(const Location &loc, const char* id) {
        SymbolTable *scope = current_scope;
        std::string sub_name = id;
        ASR::symbol_t *sub = scope->resolve_symbol(sub_name);
        if (!sub) {
            throw SemanticError("Subroutine '" + sub_name + "' not declared", loc);
        }
        return ASR::down_cast<ASR::Subroutine_t>(sub);
    }

    void visit_Name(const AST::Name_t &x) {
        tmp = resolve_variable(x.base.base.loc, x.m_id);
    }

    void visit_FuncCallOrArray(const AST::FuncCallOrArray_t &x) {
        SymbolTable *scope = current_scope;
        std::string var_name = x.m_func;
        ASR::symbol_t *v = scope->resolve_symbol(var_name);
        if (!v) {
            // TODO: add these to global scope by default ahead of time
            if (var_name == "size") {
                // Intrinsic function size(), add it to the global scope
                ASR::TranslationUnit_t *unit = (ASR::TranslationUnit_t*)asr;
                const char* fn_name_orig = "size";
                char *fn_name = (char*)fn_name_orig;
                SymbolTable *fn_scope = al.make_new<SymbolTable>(unit->m_global_scope);
                ASR::ttype_t *type;
                type = TYPE(ASR::make_Integer_t(al, x.base.base.loc, 4, nullptr, 0));
                ASR::asr_t *return_var = ASR::make_Variable_t(al, x.base.base.loc,
                    fn_scope, fn_name, intent_return_var, type);
                fn_scope->scope[std::string(fn_name)] = ASR::down_cast<ASR::symbol_t>(return_var);
                ASR::asr_t *return_var_ref = ASR::make_Var_t(al, x.base.base.loc,
                    ASR::down_cast<ASR::symbol_t>(return_var));
                ASR::asr_t *fn = ASR::make_Function_t(
                    al, x.base.base.loc,
                    /* a_symtab */ fn_scope,
                    /* a_name */ fn_name,
                    /* a_args */ nullptr,
                    /* n_args */ 0,
                    /* a_body */ nullptr,
                    /* n_body */ 0,
                    /* a_bind */ nullptr,
                    /* a_return_var */ EXPR(return_var_ref),
                    /* a_module */ nullptr);
                std::string sym_name = fn_name;
                unit->m_global_scope->scope[sym_name] = ASR::down_cast<ASR::symbol_t>(fn);
                v = ASR::down_cast<ASR::symbol_t>(fn);
            } else if (var_name == "present") {
                // Intrinsic function present(), add it to the global scope
                ASR::TranslationUnit_t *unit = (ASR::TranslationUnit_t*)asr;
                const char* fn_name_orig = "present";
                char *fn_name = (char*)fn_name_orig;
                SymbolTable *fn_scope = al.make_new<SymbolTable>(unit->m_global_scope);
                ASR::ttype_t *type;
                type = TYPE(ASR::make_Logical_t(al, x.base.base.loc, 4, nullptr, 0));
                ASR::asr_t *return_var = ASR::make_Variable_t(al, x.base.base.loc,
                    fn_scope, fn_name, intent_return_var, type);
                fn_scope->scope[std::string(fn_name)] = ASR::down_cast<ASR::symbol_t>(return_var);
                ASR::asr_t *return_var_ref = ASR::make_Var_t(al, x.base.base.loc,
                    ASR::down_cast<ASR::symbol_t>(return_var));
                ASR::asr_t *fn = ASR::make_Function_t(
                    al, x.base.base.loc,
                    /* a_symtab */ fn_scope,
                    /* a_name */ fn_name,
                    /* a_args */ nullptr,
                    /* n_args */ 0,
                    /* a_body */ nullptr,
                    /* n_body */ 0,
                    /* a_bind */ nullptr,
                    /* a_return_var */ EXPR(return_var_ref),
                    /* a_module */ nullptr);
                std::string sym_name = fn_name;
                unit->m_global_scope->scope[sym_name] = ASR::down_cast<ASR::symbol_t>(fn);
                v = ASR::down_cast<ASR::symbol_t>(fn);
            } else if (var_name == "sin") {
                // Intrinsic function sin(x), add it to the global scope
                ASR::TranslationUnit_t *unit = (ASR::TranslationUnit_t*)asr;
                const char* fn_name_orig = "sin";
                char *fn_name = (char*)fn_name_orig;
                SymbolTable *fn_scope = al.make_new<SymbolTable>(unit->m_global_scope);
                ASR::ttype_t *type;

                // Arguments
                Vec<ASR::expr_t*> args;
                args.reserve(al, 1);
                type = TYPE(ASR::make_Real_t(al, x.base.base.loc, 4, nullptr, 0));
                const char* arg0_s_orig = "x";
                char *arg0_s = (char*)arg0_s_orig;
                ASR::asr_t *arg0 = ASR::make_Variable_t(al, x.base.base.loc,
                    fn_scope, arg0_s, intent_in, type);
                ASR::symbol_t *var = ASR::down_cast<ASR::symbol_t>(arg0);
                fn_scope->scope[std::string(arg0_s)] = var;
                args.push_back(al, EXPR(ASR::make_Var_t(al, x.base.base.loc,
                    var)));

                // Return value
                type = TYPE(ASR::make_Real_t(al, x.base.base.loc, 4, nullptr, 0));
                ASR::asr_t *return_var = ASR::make_Variable_t(al, x.base.base.loc,
                    fn_scope, fn_name, intent_return_var, type);
                fn_scope->scope[std::string(fn_name)] = ASR::down_cast<ASR::symbol_t>(return_var);
                ASR::asr_t *return_var_ref = ASR::make_Var_t(al, x.base.base.loc,
                    ASR::down_cast<ASR::symbol_t>(return_var));

                ASR::proc_external_t *external = al.make_new<ASR::proc_external_t>();
                external->m_type = ASR::proc_external_typeType::Intrinsic;
                external->m_module_proc = nullptr;

                ASR::asr_t *fn = ASR::make_Function_t(
                    al, x.base.base.loc,
                    /* a_symtab */ fn_scope,
                    /* a_name */ fn_name,
                    /* a_args */ args.p,
                    /* n_args */ args.n,
                    /* a_body */ nullptr,
                    /* n_body */ 0,
                    /* a_bind */ nullptr,
                    /* a_return_var */ EXPR(return_var_ref),
                    /* a_external */ external);
                std::string sym_name = fn_name;
                unit->m_global_scope->scope[sym_name] = ASR::down_cast<ASR::symbol_t>(fn);
                v = ASR::down_cast<ASR::symbol_t>(fn);
            } else {
                throw SemanticError("Function or array '" + var_name
                    + "' not declared", x.base.base.loc);
            }
        }
        switch (v->type) {
            case (ASR::symbolType::Function) : {
                Vec<ASR::expr_t*> args = visit_expr_list(x.m_args, x.n_args);
                ASR::ttype_t *type;
                type = EXPR2VAR(ASR::down_cast<ASR::Function_t>(v)->m_return_var)->m_type;
                tmp = ASR::make_FuncCall_t(al, x.base.base.loc,
                    v, args.p, args.size(), nullptr, 0, type);
                break;
            }
            case (ASR::symbolType::Variable) : {
                Vec<ASR::array_index_t> args;
                args.reserve(al, x.n_args);
                for (size_t i=0; i<x.n_args; i++) {
                    visit_expr(*x.m_args[i].m_end);
                    ASR::array_index_t ai;
                    ai.m_left = nullptr;
                    ai.m_right = EXPR(tmp);
                    ai.m_step = nullptr;
                    args.push_back(al, ai);
                }

                ASR::ttype_t *type;
                type = ASR::down_cast<ASR::Variable_t>(v)->m_type;
                tmp = ASR::make_ArrayRef_t(al, x.base.base.loc,
                    v, args.p, args.size(), type);
                break;
            }
            default : throw SemanticError("Symbol '" + var_name
                    + "' is not a function or an array", x.base.base.loc);
        }
    }

    void visit_Num(const AST::Num_t &x) {
        ASR::ttype_t *type = TYPE(ASR::make_Integer_t(al, x.base.base.loc,
                4, nullptr, 0));
        tmp = ASR::make_ConstantInteger_t(al, x.base.base.loc, x.m_n, type);
    }

    void visit_Constant(const AST::Constant_t &x) {
        ASR::ttype_t *type = TYPE(ASR::make_Logical_t(al, x.base.base.loc,
                4, nullptr, 0));
        tmp = ASR::make_ConstantLogical_t(al, x.base.base.loc, x.m_value, type);
    }

    void visit_Str(const AST::Str_t &x) {
        ASR::ttype_t *type = TYPE(ASR::make_Character_t(al, x.base.base.loc,
                8, nullptr, 0));
        tmp = ASR::make_Str_t(al, x.base.base.loc, x.m_s, type);
    }

    void visit_Real(const AST::Real_t &x) {
        ASR::ttype_t *type = TYPE(ASR::make_Real_t(al, x.base.base.loc,
                4, nullptr, 0));
        tmp = ASR::make_ConstantReal_t(al, x.base.base.loc, x.m_n, type);
    }

    void visit_Complex(const AST::Complex_t &x) {
        ASR::ttype_t *type = TYPE(ASR::make_Complex_t(al, x.base.base.loc,
                4, nullptr, 0));
        this->visit_expr(*x.m_re);
        ASR::expr_t *re = EXPR(tmp);
        this->visit_expr(*x.m_im);
        ASR::expr_t *im = EXPR(tmp);
        tmp = ASR::make_ConstantComplex_t(al, x.base.base.loc,
                re, im, type);
    }


    void visit_ArrayInitializer(const AST::ArrayInitializer_t &x) {
        Vec<ASR::expr_t*> body;
        body.reserve(al, x.n_args);
        ASR::ttype_t *type = nullptr;
        for (size_t i=0; i<x.n_args; i++) {
            visit_expr(*x.m_args[i]);
            ASR::expr_t *expr = EXPR(tmp);
            if (type == nullptr) {
                type = expr_type(expr);
            } else {
                if (expr_type(expr)->type != type->type) {
                    throw SemanticError("Type mismatch in array initializer",
                        x.base.base.loc);
                }
            }
            body.push_back(al, expr);
        }
        tmp = ASR::make_ArrayInitializer_t(al, x.base.base.loc, body.p,
            body.size(), type);
    }

    void visit_Print(const AST::Print_t &x) {
        Vec<ASR::expr_t*> body;
        body.reserve(al, x.n_values);
        for (size_t i=0; i<x.n_values; i++) {
            visit_expr(*x.m_values[i]);
            ASR::expr_t *expr = EXPR(tmp);
            body.push_back(al, expr);
        }
        tmp = ASR::make_Print_t(al, x.base.base.loc, nullptr,
            body.p, body.size());
    }

    void visit_If(const AST::If_t &x) {
        visit_expr(*x.m_test);
        ASR::expr_t *test = EXPR(tmp);
        Vec<ASR::stmt_t*> body;
        body.reserve(al, x.n_body);
        for (size_t i=0; i<x.n_body; i++) {
            visit_stmt(*x.m_body[i]);
            body.push_back(al, STMT(tmp));
        }
        Vec<ASR::stmt_t*> orelse;
        orelse.reserve(al, x.n_orelse);
        for (size_t i=0; i<x.n_orelse; i++) {
            visit_stmt(*x.m_orelse[i]);
            orelse.push_back(al, STMT(tmp));
        }
        tmp = ASR::make_If_t(al, x.base.base.loc, test, body.p,
                body.size(), orelse.p, orelse.size());
    }

    void visit_WhileLoop(const AST::WhileLoop_t &x) {
        visit_expr(*x.m_test);
        ASR::expr_t *test = EXPR(tmp);
        Vec<ASR::stmt_t*> body;
        body.reserve(al, x.n_body);
        for (size_t i=0; i<x.n_body; i++) {
            visit_stmt(*x.m_body[i]);
            body.push_back(al, STMT(tmp));
        }
        tmp = ASR::make_WhileLoop_t(al, x.base.base.loc, test, body.p,
                body.size());
    }

    void visit_DoLoop(const AST::DoLoop_t &x) {
        if (! x.m_var) {
            throw SemanticError("Do loop: loop variable is required for now",
                x.base.base.loc);
        }
        if (! x.m_start) {
            throw SemanticError("Do loop: start condition required for now",
                x.base.base.loc);
        }
        if (! x.m_end) {
            throw SemanticError("Do loop: end condition required for now",
                x.base.base.loc);
        }
        ASR::expr_t *var = EXPR(resolve_variable(x.base.base.loc, x.m_var));
        visit_expr(*x.m_start);
        ASR::expr_t *start = EXPR(tmp);
        visit_expr(*x.m_end);
        ASR::expr_t *end = EXPR(tmp);
        ASR::expr_t *increment;
        if (x.m_increment) {
            visit_expr(*x.m_increment);
            increment = EXPR(tmp);
        } else {
            increment = nullptr;
        }

        Vec<ASR::stmt_t*> body;
        body.reserve(al, x.n_body);
        for (size_t i=0; i<x.n_body; i++) {
            visit_stmt(*x.m_body[i]);
            body.push_back(al, STMT(tmp));
        }
        ASR::do_loop_head_t head;
        head.m_v = var;
        head.m_start = start;
        head.m_end = end;
        head.m_increment = increment;
        tmp = ASR::make_DoLoop_t(al, x.base.base.loc, head, body.p,
                body.size());
    }

    void visit_DoConcurrentLoop(const AST::DoConcurrentLoop_t &x) {
        if (x.n_control != 1) {
            throw SemanticError("Do concurrent: exactly one control statement is required for now",
            x.base.base.loc);
        }
        AST::ConcurrentControl_t &h = *(AST::ConcurrentControl_t*) x.m_control[0];
        if (! h.m_var) {
            throw SemanticError("Do loop: loop variable is required for now",
                x.base.base.loc);
        }
        if (! h.m_start) {
            throw SemanticError("Do loop: start condition required for now",
                x.base.base.loc);
        }
        if (! h.m_end) {
            throw SemanticError("Do loop: end condition required for now",
                x.base.base.loc);
        }
        ASR::expr_t *var = EXPR(resolve_variable(x.base.base.loc, h.m_var));
        visit_expr(*h.m_start);
        ASR::expr_t *start = EXPR(tmp);
        visit_expr(*h.m_end);
        ASR::expr_t *end = EXPR(tmp);
        ASR::expr_t *increment;
        if (h.m_increment) {
            visit_expr(*h.m_increment);
            increment = EXPR(tmp);
        } else {
            increment = nullptr;
        }

        Vec<ASR::stmt_t*> body;
        body.reserve(al, x.n_body);
        for (size_t i=0; i<x.n_body; i++) {
            visit_stmt(*x.m_body[i]);
            body.push_back(al, STMT(tmp));
        }
        ASR::do_loop_head_t head;
        head.m_v = var;
        head.m_start = start;
        head.m_end = end;
        head.m_increment = increment;
        tmp = ASR::make_DoConcurrentLoop_t(al, x.base.base.loc, head, body.p,
                body.size());
    }

    void visit_Exit(const AST::Exit_t &x) {
        // TODO: add a check here that we are inside a While loop
        tmp = ASR::make_Exit_t(al, x.base.base.loc);
    }

    void visit_Cycle(const AST::Cycle_t &x) {
        // TODO: add a check here that we are inside a While loop
        tmp = ASR::make_Cycle_t(al, x.base.base.loc);
    }

    void visit_Stop(const AST::Stop_t &x) {
        ASR::expr_t *code;
        if (x.m_code) {
            visit_expr(*x.m_code);
            code = EXPR(tmp);
        } else {
            code = nullptr;
        }
        tmp = ASR::make_Stop_t(al, x.base.base.loc, code);
    }

    void visit_ErrorStop(const AST::ErrorStop_t &x) {
        ASR::expr_t *code;
        if (x.m_code) {
            visit_expr(*x.m_code);
            code = EXPR(tmp);
        } else {
            code = nullptr;
        }
        tmp = ASR::make_ErrorStop_t(al, x.base.base.loc, code);
    }
};

ASR::TranslationUnit_t *ast_to_asr(Allocator &al, AST::TranslationUnit_t &ast,
        SymbolTable *symbol_table)
{
    SymbolTableVisitor v(al, symbol_table);
    v.visit_TranslationUnit(ast);
    ASR::asr_t *unit = v.asr;

    // Uncomment for debugging the ASR after SymbolTable building:
    // std::cout << pickle(*unit) << std::endl;

    BodyVisitor b(al, unit);
    b.visit_TranslationUnit(ast);
    return ASR::down_cast2<ASR::TranslationUnit_t>(unit);
}

} // namespace LFortran
