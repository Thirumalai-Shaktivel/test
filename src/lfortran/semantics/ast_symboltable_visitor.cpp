#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <cmath>
#include <limits>

#include <lfortran/ast.h>
#include <lfortran/asr.h>
#include <lfortran/asr_utils.h>
#include <lfortran/asr_verify.h>
#include <lfortran/exception.h>
#include <lfortran/semantics/asr_implicit_cast_rules.h>
#include <lfortran/semantics/ast_common_visitor.h>
#include <lfortran/semantics/ast_to_asr.h>
#include <lfortran/parser/parser_stype.h>
#include <lfortran/string_utils.h>
#include <lfortran/utils.h>

namespace LFortran {

template <typename T>
void extract_bind(T &x, ASR::abiType &abi_type, char *&bindc_name) {
    if (x.m_bind) {
        AST::Bind_t *bind = AST::down_cast<AST::Bind_t>(x.m_bind);
        if (bind->n_args == 1) {
            if (AST::is_a<AST::Name_t>(*bind->m_args[0])) {
                AST::Name_t *name = AST::down_cast<AST::Name_t>(
                    bind->m_args[0]);
                if (to_lower(std::string(name->m_id)) == "c") {
                    abi_type=ASR::abiType::BindC;
                } else {
                    throw SemanticError("Unsupported language in bind()",
                        x.base.base.loc);
                }
            } else {
                    throw SemanticError("Language name must be specified in bind() as plain text",
                        x.base.base.loc);
            }
        } else {
            throw SemanticError("At least one argument needed in bind()",
                x.base.base.loc);
        }
        if (bind->n_kwargs == 1) {
            char *arg = bind->m_kwargs[0].m_arg;
            AST::expr_t *value = bind->m_kwargs[0].m_value;
            if (to_lower(std::string(arg)) == "name") {
                if (AST::is_a<AST::String_t>(*value)) {
                    AST::String_t *name = AST::down_cast<AST::String_t>(value);
                    bindc_name = name->m_s;
                } else {
                    throw SemanticError("The value of the 'name' keyword argument in bind(c) must be a string",
                        x.base.base.loc);
                }
            } else {
                throw SemanticError("Unsupported keyword argument in bind()",
                    x.base.base.loc);
            }
        }
    }
}


class SymbolTableVisitor : public AST::BaseVisitor<SymbolTableVisitor> {
private:
    std::map<std::string, std::string> intrinsic_procedures = {
        {"kind", "lfortran_intrinsic_kind"},
        {"selected_int_kind", "lfortran_intrinsic_kind"},
        {"selected_real_kind", "lfortran_intrinsic_kind"},
        {"size", "lfortran_intrinsic_array"},
        {"lbound", "lfortran_intrinsic_array"},
        {"ubound", "lfortran_intrinsic_array"},
        {"min", "lfortran_intrinsic_array"},
        {"max", "lfortran_intrinsic_array"},
        {"allocated", "lfortran_intrinsic_array"},
        {"minval", "lfortran_intrinsic_array"},
        {"maxval", "lfortran_intrinsic_array"},
        {"real", "lfortran_intrinsic_array"},
        {"floor", "lfortran_intrinsic_array"},
        {"int", "lfortran_intrinsic_array"},
        {"sum", "lfortran_intrinsic_array"},
        {"abs", "lfortran_intrinsic_array"},
        {"tiny", "lfortran_intrinsic_array"},
        {"char", "lfortran_intrinsic_array"},
        {"present", "lfortran_intrinsic_util"},
        {"bit_size", "lfortran_intrinsic_util"},
        {"not", "lfortran_intrinsic_util"},
        {"iachar", "lfortran_intrinsic_util"},
        {"achar", "lfortran_intrinsic_util"},
        {"len", "lfortran_intrinsic_util"},
        {"index", "lfortran_intrinsic_string"}
};

public:
    ASR::asr_t *asr;
    Allocator &al;
    SymbolTable *current_scope;
    SymbolTable *global_scope;
    std::map<std::string, std::vector<std::string>> generic_procedures;
    std::map<AST::intrinsicopType, std::vector<std::string>> overloaded_op_procs;
    std::map<std::string, std::map<std::string, std::string>> class_procedures;
    std::string dt_name;
    ASR::accessType dflt_access = ASR::Public;
    ASR::presenceType dflt_presence = ASR::presenceType::Required;
    std::map<std::string, ASR::accessType> assgnd_access;
    std::map<std::string, ASR::presenceType> assgnd_presence;
    Vec<char *> current_module_dependencies;
    bool in_module = false;
    bool is_interface = false;
    bool is_derived_type = false;
    Vec<char*> data_member_names;
    std::vector<std::string> current_procedure_args;
    ASR::abiType current_procedure_abi_type = ASR::abiType::Source;
    std::map<SymbolTable*, std::map<AST::decl_attribute_t*, AST::simple_attributeType>> overloaded_ops;

    std::map<AST::intrinsicopType, std::string> intrinsic2str = {
        {AST::intrinsicopType::STAR, "~mul"},
        {AST::intrinsicopType::PLUS, "~add"},
    };

    std::map<AST::operatorType, std::string> binop2str = {
        {AST::operatorType::Mul, "~mul"},
        {AST::operatorType::Add, "~add"},
    };

    SymbolTableVisitor(Allocator &al, SymbolTable *symbol_table)
      : al{al}, current_scope{symbol_table}, is_derived_type{false} {}


    ASR::symbol_t* resolve_symbol(const Location &loc, const std::string &sub_name) {
        SymbolTable *scope = current_scope;
        ASR::symbol_t *sub = scope->resolve_symbol(sub_name);
        if (!sub) {
            throw SemanticError("Symbol '" + sub_name + "' not declared", loc);
        }
        return sub;
    }

    void visit_TranslationUnit(const AST::TranslationUnit_t &x) {
        if (!current_scope) {
            current_scope = al.make_new<SymbolTable>(nullptr);
        }
        LFORTRAN_ASSERT(current_scope != nullptr);
        global_scope = current_scope;
        for (size_t i=0; i<x.n_items; i++) {
            AST::astType t = x.m_items[i]->type;
            if (t != AST::astType::expr && t != AST::astType::stmt) {
                visit_ast(*x.m_items[i]);
            }
        }
        global_scope = nullptr;
        asr = ASR::make_TranslationUnit_t(al, x.base.base.loc,
            current_scope, nullptr, 0);
    }

    void visit_Module(const AST::Module_t &x) {
        SymbolTable *parent_scope = current_scope;
        current_scope = al.make_new<SymbolTable>(parent_scope);
        current_module_dependencies.reserve(al, 4);
        generic_procedures.clear();
        in_module = true;
        for (size_t i=0; i<x.n_use; i++) {
            visit_unit_decl1(*x.m_use[i]);
        }
        for (size_t i=0; i<x.n_decl; i++) {
            visit_unit_decl2(*x.m_decl[i]);
        }
        for (size_t i=0; i<x.n_contains; i++) {
            visit_program_unit(*x.m_contains[i]);
        }
        add_generic_procedures();
        add_overloaded_procedures();
        add_class_procedures();
        asr = ASR::make_Module_t(
            al, x.base.base.loc,
            /* a_symtab */ current_scope,
            /* a_name */ s2c(al, to_lower(x.m_name)),
            current_module_dependencies.p,
            current_module_dependencies.n,
            false);
        std::string sym_name = to_lower(x.m_name);
        if (parent_scope->scope.find(sym_name) != parent_scope->scope.end()) {
            throw SemanticError("Module already defined", asr->loc);
        }
        parent_scope->scope[sym_name] = ASR::down_cast<ASR::symbol_t>(asr);
        current_scope = parent_scope;
        in_module = false;
    }

    void visit_Program(const AST::Program_t &x) {
        SymbolTable *parent_scope = current_scope;
        current_scope = al.make_new<SymbolTable>(parent_scope);
        current_module_dependencies.reserve(al, 4);
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
            /* a_name */ s2c(al, to_lower(x.m_name)),
            current_module_dependencies.p,
            current_module_dependencies.n,
            /* a_body */ nullptr,
            /* n_body */ 0);
        std::string sym_name = to_lower(x.m_name);
        if (parent_scope->scope.find(sym_name) != parent_scope->scope.end()) {
            throw SemanticError("Program already defined", asr->loc);
        }
        parent_scope->scope[sym_name] = ASR::down_cast<ASR::symbol_t>(asr);
        current_scope = parent_scope;
    }

    void visit_Subroutine(const AST::Subroutine_t &x) {
        ASR::accessType s_access = dflt_access;
        ASR::deftypeType deftype = ASR::deftypeType::Implementation;
        SymbolTable *parent_scope = current_scope;
        current_scope = al.make_new<SymbolTable>(parent_scope);
        for (size_t i=0; i<x.n_args; i++) {
            char *arg=x.m_args[i].m_arg;
            std::string arg_s = arg;
            current_procedure_args.push_back(arg);
        }
        current_procedure_abi_type = ASR::abiType::Source;
        char *bindc_name=nullptr;
        extract_bind(x, current_procedure_abi_type, bindc_name);

        for (size_t i=0; i<x.n_decl; i++) {
            visit_unit_decl2(*x.m_decl[i]);
        }
        for (size_t i=0; i<x.n_contains; i++) {
            visit_program_unit(*x.m_contains[i]);
        }
        Vec<ASR::expr_t*> args;
        args.reserve(al, x.n_args);
        for (size_t i=0; i<x.n_args; i++) {
            char *arg=x.m_args[i].m_arg;
            std::string arg_s = to_lower(arg);
            if (current_scope->scope.find(arg_s) == current_scope->scope.end()) {
                throw SemanticError("Dummy argument '" + arg_s + "' not defined", x.base.base.loc);
            }
            ASR::symbol_t *var = current_scope->scope[arg_s];
            args.push_back(al, LFortran::ASRUtils::EXPR(ASR::make_Var_t(al, x.base.base.loc,
                var)));
        }
        std::string sym_name = to_lower(x.m_name);
        if (assgnd_access.count(sym_name)) {
            s_access = assgnd_access[sym_name];
        }
        if (is_interface){
            deftype = ASR::deftypeType::Interface;
        }
        asr = ASR::make_Subroutine_t(
            al, x.base.base.loc,
            /* a_symtab */ current_scope,
            /* a_name */ s2c(al, to_lower(x.m_name)),
            /* a_args */ args.p,
            /* n_args */ args.size(),
            /* a_body */ nullptr,
            /* n_body */ 0,
            current_procedure_abi_type,
            s_access, deftype, bindc_name);
        if (parent_scope->scope.find(sym_name) != parent_scope->scope.end()) {
            ASR::symbol_t *f1 = parent_scope->scope[sym_name];
            ASR::Subroutine_t *f2 = ASR::down_cast<ASR::Subroutine_t>(f1);
            if (f2->m_abi == ASR::abiType::Interactive) {
                // Previous declaration will be shadowed
            } else {
                throw SemanticError("Subroutine already defined", asr->loc);
            }
        }
        parent_scope->scope[sym_name] = ASR::down_cast<ASR::symbol_t>(asr);
        current_scope = parent_scope;
        /* FIXME: This can become incorrect/get cleared prematurely, perhaps
           in nested functions, and also in callback.f90 test, but it may not
           matter since we would have already checked the intent */
        current_procedure_args.clear();
        current_procedure_abi_type = ASR::abiType::Source;
    }

    AST::AttrType_t* find_return_type(AST::decl_attribute_t** attributes,
            size_t n, const Location &loc) {
        AST::AttrType_t* r = nullptr;
        bool found = false;
        for (size_t i=0; i<n; i++) {
            if (AST::is_a<AST::AttrType_t>(*attributes[i])) {
                if (found) {
                    throw SemanticError("Return type declared twice", loc);
                } else {
                    r = AST::down_cast<AST::AttrType_t>(attributes[i]);
                    found = true;
                }
            }
        }
        return r;
    }

    void visit_Function(const AST::Function_t &x) {
        // Extract local (including dummy) variables first
        ASR::accessType s_access = dflt_access;
        ASR::deftypeType deftype = ASR::deftypeType::Implementation;
        SymbolTable *parent_scope = current_scope;
        current_scope = al.make_new<SymbolTable>(parent_scope);
        for (size_t i=0; i<x.n_args; i++) {
            char *arg=x.m_args[i].m_arg;
            std::string arg_s = arg;
            current_procedure_args.push_back(arg);
        }

        // Determine the ABI (Source or BindC for now)
        current_procedure_abi_type = ASR::abiType::Source;
        char *bindc_name=nullptr;
        extract_bind(x, current_procedure_abi_type, bindc_name);

        for (size_t i=0; i<x.n_decl; i++) {
            visit_unit_decl2(*x.m_decl[i]);
        }
        for (size_t i=0; i<x.n_contains; i++) {
            visit_program_unit(*x.m_contains[i]);
        }
        // Convert and check arguments
        Vec<ASR::expr_t*> args;
        args.reserve(al, x.n_args);
        for (size_t i=0; i<x.n_args; i++) {
            char *arg=x.m_args[i].m_arg;
            std::string arg_s = to_lower(arg);
            if (current_scope->scope.find(arg_s) == current_scope->scope.end()) {
                throw SemanticError("Dummy argument '" + arg_s + "' not defined", x.base.base.loc);
            }
            ASR::symbol_t *var = current_scope->scope[arg_s];
            args.push_back(al, LFortran::ASRUtils::EXPR(ASR::make_Var_t(al, x.base.base.loc,
                var)));
        }

        // Handle the return variable and type
        // First determine the name of the variable: either the function name
        // or result(...)
        std::string return_var_name;
        if (x.m_return_var) {
            if (x.m_return_var->type == AST::exprType::Name) {
                return_var_name = to_lower(((AST::Name_t*)(x.m_return_var))->m_id);
            } else {
                throw SemanticError("Return variable must be an identifier",
                    x.m_return_var->base.loc);
            }
        } else {
            return_var_name = to_lower(x.m_name);
        }

        // Determine the type of the variable, the type is either specified as
        //     integer function f()
        // or in local variables as
        //     integer :: f
        ASR::asr_t *return_var;
        AST::AttrType_t *return_type = find_return_type(x.m_attributes,
            x.n_attributes, x.base.base.loc);
        if (current_scope->scope.find(return_var_name) == current_scope->scope.end()) {
            // The variable is not defined among local variables, extract the
            // type from "integer function f()" and add the variable.
            if (!return_type) {
                throw SemanticError("Return type not specified",
                        x.base.base.loc);
            }
            ASR::ttype_t *type;
            int a_kind = 4;
            int a_len = -10;
            if (return_type->m_kind != nullptr) {
                if (return_type->n_kind == 1) {
                    visit_expr(*return_type->m_kind->m_value);
                    ASR::expr_t* kind_expr = LFortran::ASRUtils::EXPR(asr);
                    if (return_type->m_type == AST::decl_typeType::TypeCharacter) {
                        a_len = ASRUtils::extract_len(kind_expr, x.base.base.loc);
                    } else {
                        a_kind = ASRUtils::extract_kind(kind_expr, x.base.base.loc);
                    }
                } else {
                    throw SemanticError("Only one kind item supported for now", x.base.base.loc);
                }
            }
            switch (return_type->m_type) {
                case (AST::decl_typeType::TypeInteger) : {
                    type = LFortran::ASRUtils::TYPE(ASR::make_Integer_t(al, x.base.base.loc, a_kind, nullptr, 0));
                    break;
                }
                case (AST::decl_typeType::TypeReal) : {
                    type = LFortran::ASRUtils::TYPE(ASR::make_Real_t(al, x.base.base.loc, a_kind, nullptr, 0));
                    break;
                }
                case (AST::decl_typeType::TypeComplex) : {
                    type = LFortran::ASRUtils::TYPE(ASR::make_Complex_t(al, x.base.base.loc, a_kind, nullptr, 0));
                    break;
                }
                case (AST::decl_typeType::TypeLogical) : {
                    type = LFortran::ASRUtils::TYPE(ASR::make_Logical_t(al, x.base.base.loc, 4, nullptr, 0));
                    break;
                }
                case (AST::decl_typeType::TypeCharacter) : {
                    type = LFortran::ASRUtils::TYPE(ASR::make_Character_t(al, x.base.base.loc, 1, a_len, nullptr, nullptr, 0));
                    break;
                }
                default :
                    throw SemanticError("Return type not supported",
                            x.base.base.loc);
            }
            // Add it as a local variable:
            return_var = ASR::make_Variable_t(al, x.base.base.loc,
                current_scope, s2c(al, return_var_name), LFortran::ASRUtils::intent_return_var, nullptr, nullptr,
                ASR::storage_typeType::Default, type,
                current_procedure_abi_type, ASR::Public, ASR::presenceType::Required,
                false);
            current_scope->scope[return_var_name]
                = ASR::down_cast<ASR::symbol_t>(return_var);
        } else {
            if (return_type) {
                throw SemanticError("Cannot specify the return type twice",
                    x.base.base.loc);
            }
            // Extract the variable from the local scope
            return_var = (ASR::asr_t*) current_scope->scope[return_var_name];
            ASR::down_cast2<ASR::Variable_t>(return_var)->m_intent = LFortran::ASRUtils::intent_return_var;
        }

        ASR::asr_t *return_var_ref = ASR::make_Var_t(al, x.base.base.loc,
            ASR::down_cast<ASR::symbol_t>(return_var));

        // Create and register the function
        std::string sym_name = to_lower(x.m_name);
        if (assgnd_access.count(sym_name)) {
            s_access = assgnd_access[sym_name];
        }

        if (is_interface) {
            deftype = ASR::deftypeType::Interface;
        }

        asr = ASR::make_Function_t(
            al, x.base.base.loc,
            /* a_symtab */ current_scope,
            /* a_name */ s2c(al, to_lower(x.m_name)),
            /* a_args */ args.p,
            /* n_args */ args.size(),
            /* a_body */ nullptr,
            /* n_body */ 0,
            /* a_return_var */ LFortran::ASRUtils::EXPR(return_var_ref),
            current_procedure_abi_type, s_access, deftype, bindc_name);
        if (parent_scope->scope.find(sym_name) != parent_scope->scope.end()) {
            ASR::symbol_t *f1 = parent_scope->scope[sym_name];
            ASR::Function_t *f2 = ASR::down_cast<ASR::Function_t>(f1);
            if (f2->m_abi == ASR::abiType::Interactive) {
                // Previous declaration will be shadowed
            } else {
                throw SemanticError("Function already defined", asr->loc);
            }
        }
        parent_scope->scope[sym_name] = ASR::down_cast<ASR::symbol_t>(asr);
        current_scope = parent_scope;
        current_procedure_args.clear();
        current_procedure_abi_type = ASR::abiType::Source;
    }

    void visit_StrOp(const AST::StrOp_t &x) {
        this->visit_expr(*x.m_left);
        ASR::expr_t *left = LFortran::ASRUtils::EXPR(asr);
        this->visit_expr(*x.m_right);
        ASR::expr_t *right = LFortran::ASRUtils::EXPR(asr);
        CommonVisitorMethods::visit_StrOp(al, x, left, right, asr);
    }

    void visit_UnaryOp(const AST::UnaryOp_t &x) {
        this->visit_expr(*x.m_operand);
        ASR::expr_t *operand = LFortran::ASRUtils::EXPR(asr);
        CommonVisitorMethods::visit_UnaryOp(al, x, operand, asr);
    }

    void visit_BoolOp(const AST::BoolOp_t &x) {
        this->visit_expr(*x.m_left);
        ASR::expr_t *left = LFortran::ASRUtils::EXPR(asr);
        this->visit_expr(*x.m_right);
        ASR::expr_t *right = LFortran::ASRUtils::EXPR(asr);
        CommonVisitorMethods::visit_BoolOp(al, x, left, right, asr);
    }

    void visit_Compare(const AST::Compare_t &x) {
        this->visit_expr(*x.m_left);
        ASR::expr_t *left = LFortran::ASRUtils::EXPR(asr);
        this->visit_expr(*x.m_right);
        ASR::expr_t *right = LFortran::ASRUtils::EXPR(asr);
        CommonVisitorMethods::visit_Compare(al, x, left, right, asr);
    }

    void visit_BinOp(const AST::BinOp_t &x) {
        this->visit_expr(*x.m_left);
        ASR::expr_t *left = LFortran::ASRUtils::EXPR(asr);
        this->visit_expr(*x.m_right);
        ASR::expr_t *right = LFortran::ASRUtils::EXPR(asr);
        CommonVisitorMethods::visit_BinOp(al, x, left, right, asr, binop2str[x.m_op], current_scope);
    }

    void visit_String(const AST::String_t &x) {
        int s_len = strlen(x.m_s);
        ASR::ttype_t *type = LFortran::ASRUtils::TYPE(ASR::make_Character_t(al, x.base.base.loc,
                1, s_len, nullptr, nullptr, 0));
        asr = ASR::make_ConstantString_t(al, x.base.base.loc, x.m_s, type);
    }

    void visit_BOZ(const AST::BOZ_t& x) {
        LFortran::Str boz_content;
        std::string s = x.m_s; 
        boz_content.from_str(al, s.substr(1));
        ASR::bozType boz_type;
        if( s[0] == 'b' || s[0] == 'B' ) {
            boz_type = ASR::bozType::Binary;
        } else if( s[0] == 'z' || s[0] == 'Z' ) {
            boz_type = ASR::bozType::Hex;
        } else if( s[0] == 'o' || s[0] == 'O' ) {
            boz_type = ASR::bozType::Octal;
        } else {
            throw SemanticError(R"""(Only 'b', 'o' and 'z' 
                                are accepted as prefixes of 
                                BOZ literal constants.)""", 
                                x.base.base.loc);
        }
        asr = ASR::make_BOZ_t(al, x.base.base.loc, boz_content.c_str(al),
                                boz_type, nullptr);
    }

    void visit_Logical(const AST::Logical_t &x) {
        ASR::ttype_t *type = LFortran::ASRUtils::TYPE(ASR::make_Logical_t(al, x.base.base.loc,
                4, nullptr, 0));
        asr = ASR::make_ConstantLogical_t(al, x.base.base.loc, x.m_value, type);
    }

    void visit_Complex(const AST::Complex_t &x) {
        this->visit_expr(*x.m_re);
        ASR::expr_t *re = LFortran::ASRUtils::EXPR(asr);
        this->visit_expr(*x.m_im);
        ASR::expr_t *im = LFortran::ASRUtils::EXPR(asr);
        int re_kind = LFortran::ASRUtils::extract_kind_from_ttype_t(LFortran::ASRUtils::expr_type(re));
        int im_kind = LFortran::ASRUtils::extract_kind_from_ttype_t(LFortran::ASRUtils::expr_type(im));
        ASR::ttype_t *type = LFortran::ASRUtils::TYPE(ASR::make_Complex_t(al, x.base.base.loc,
                std::max(re_kind, im_kind), nullptr, 0));
        asr = ASR::make_ConstantComplex_t(al, x.base.base.loc,
                re, im, type);
    }

    void process_dims(Allocator &al, Vec<ASR::dimension_t> &dims,
        AST::dimension_t *m_dim, size_t n_dim) {
        LFORTRAN_ASSERT(dims.size() == 0);
        dims.reserve(al, n_dim);
        for (size_t i=0; i<n_dim; i++) {
            ASR::dimension_t dim;
            dim.loc = m_dim[i].loc;
            if (m_dim[i].m_start) {
                this->visit_expr(*m_dim[i].m_start);
                dim.m_start = LFortran::ASRUtils::EXPR(asr);
            } else {
                dim.m_start = nullptr;
            }
            if (m_dim[i].m_end) {
                this->visit_expr(*m_dim[i].m_end);
                dim.m_end = LFortran::ASRUtils::EXPR(asr);
            } else {
                dim.m_end = nullptr;
            }
            dims.push_back(al, dim);
        }
    }

    void visit_Declaration(const AST::Declaration_t &x) {
        if (x.m_vartype == nullptr &&
                x.n_attributes == 1 &&
                AST::is_a<AST::AttrNamelist_t>(*x.m_attributes[0])) {
            //char *name = down_cast<AttrNamelist_t>(x.m_attributes[0])->m_name;
            throw SemanticError("Namelists not implemented yet", x.base.base.loc);
        }
        for (size_t i=0; i<x.n_attributes; i++) {
            if (AST::is_a<AST::AttrType_t>(*x.m_attributes[i])) {
                throw SemanticError("Type must be declared first",
                    x.base.base.loc);
            };
        }
        if (x.m_vartype == nullptr) {
            // Examples:
            // private
            // public
            // private :: x, y, z
            if (x.n_attributes == 0) {
                throw SemanticError("No attribute specified",
                    x.base.base.loc);
            }
            if (x.n_attributes > 1) {
                throw SemanticError("Only one attribute can be specified if type is missing",
                    x.base.base.loc);
            }
            LFORTRAN_ASSERT(x.n_attributes == 1);
            if (AST::is_a<AST::SimpleAttribute_t>(*x.m_attributes[0])) {
                AST::SimpleAttribute_t *sa =
                    AST::down_cast<AST::SimpleAttribute_t>(x.m_attributes[0]);
                if (x.n_syms == 0) {
                    // Example:
                    // private
                    if (sa->m_attr == AST::simple_attributeType
                            ::AttrPrivate) {
                        dflt_access = ASR::accessType::Private;
                    } else if (sa->m_attr == AST::simple_attributeType
                            ::AttrPublic) {
                        // Do nothing (public access is the default)
                        LFORTRAN_ASSERT(dflt_access == ASR::accessType::Public);
                    } else if (sa->m_attr == AST::simple_attributeType
                            ::AttrSave) {
                        if (in_module) {
                            // Do nothing (all variables implicitly have the
                            // save attribute in a module/main program)
                        } else {
                            throw SemanticError("Save Attribute not "
                                    "supported yet", x.base.base.loc);
                        }
                    } else {
                        throw SemanticError("Attribute declaration not "
                                "supported yet", x.base.base.loc);
                    }
                } else {
                    // Example:
                    // private :: x, y, z
                    for (size_t i=0; i<x.n_syms; i++) {
                        AST::var_sym_t &s = x.m_syms[i];
                        if( s.m_name == nullptr &&
                            s.m_spec->type == AST::decl_attributeType::AttrIntrinsicOperator ) {
                                // Operator Overloading Encountered
                                if( sa->m_attr != AST::simple_attributeType::AttrPublic &&
                                    sa->m_attr != AST::simple_attributeType::AttrPrivate ) {
                                    overloaded_ops[current_scope][s.m_spec] = AST::simple_attributeType::AttrPublic;
                                } else {
                                    overloaded_ops[current_scope][s.m_spec] = sa->m_attr;
                                }
                        } else {
                            std::string sym = to_lower(s.m_name);
                            if (sa->m_attr == AST::simple_attributeType
                                    ::AttrPrivate) {
                                assgnd_access[sym] = ASR::accessType::Private;
                            } else if (sa->m_attr == AST::simple_attributeType
                                    ::AttrPublic) {
                                assgnd_access[sym] = ASR::accessType::Public;
                            } else if (sa->m_attr == AST::simple_attributeType
                                    ::AttrOptional) {
                                assgnd_presence[sym] = ASR::presenceType::Optional;
                            } else {
                                throw SemanticError("Attribute declaration not "
                                        "supported", x.base.base.loc);
                            }
                        }
                    }
                }
            } else {
                throw SemanticError("Attribute declaration not supported",
                    x.base.base.loc);
            }
        } else {
            // Example
            // real(dp), private :: x, y(3), z
            for (size_t i=0; i<x.n_syms; i++) {
                AST::var_sym_t &s = x.m_syms[i];
                std::string sym = to_lower(s.m_name);
                ASR::accessType s_access = dflt_access;
                ASR::presenceType s_presence = dflt_presence;
                bool value_attr = false;
                AST::AttrType_t *sym_type =
                    AST::down_cast<AST::AttrType_t>(x.m_vartype);
                if (assgnd_access.count(sym)) {
                    s_access = assgnd_access[sym];
                }
                if (assgnd_presence.count(sym)) {
                    s_presence = assgnd_presence[sym];
                }
                ASR::storage_typeType storage_type =
                        ASR::storage_typeType::Default;
                bool is_pointer = false;
                if (current_scope->scope.find(sym) !=
                        current_scope->scope.end()) {
                    if (current_scope->parent != nullptr) {
                        // re-declaring a global scope variable is allowed
                        // Otherwise raise an error
                        throw SemanticError("Symbol already declared",
                                x.base.base.loc);
                    }
                }
                ASR::intentType s_intent;
                if (std::find(current_procedure_args.begin(),
                        current_procedure_args.end(), to_lower(s.m_name)) !=
                        current_procedure_args.end()) {
                    s_intent = LFortran::ASRUtils::intent_unspecified;
                } else {
                    s_intent = LFortran::ASRUtils::intent_local;
                }
                Vec<ASR::dimension_t> dims;
                dims.reserve(al, 0);
                if (x.n_attributes > 0) {
                    for (size_t i=0; i < x.n_attributes; i++) {
                        AST::decl_attribute_t *a = x.m_attributes[i];
                        if (AST::is_a<AST::SimpleAttribute_t>(*a)) {
                            AST::SimpleAttribute_t *sa =
                                AST::down_cast<AST::SimpleAttribute_t>(a);
                            if (sa->m_attr == AST::simple_attributeType
                                    ::AttrPrivate) {
                                s_access = ASR::accessType::Private;
                            } else if (sa->m_attr == AST::simple_attributeType
                                    ::AttrPublic) {
                                s_access = ASR::accessType::Public;
                            } else if (sa->m_attr == AST::simple_attributeType
                                    ::AttrParameter) {
                                storage_type = ASR::storage_typeType::Parameter;
                            } else if( sa->m_attr == AST::simple_attributeType
                                    ::AttrAllocatable ) {
                                storage_type = ASR::storage_typeType::Allocatable;
                            } else if (sa->m_attr == AST::simple_attributeType
                                    ::AttrPointer) {
                                is_pointer = true;
                            } else if (sa->m_attr == AST::simple_attributeType
                                    ::AttrOptional) {
                                s_presence = ASR::presenceType::Optional;
                            } else if (sa->m_attr == AST::simple_attributeType
                                    ::AttrTarget) {
                                // Do nothing for now
                            } else if (sa->m_attr == AST::simple_attributeType
                                    ::AttrAllocatable) {
                                // TODO
                            } else if (sa->m_attr == AST::simple_attributeType
                                    ::AttrValue) {
                                value_attr = true;
                            } else {
                                throw SemanticError("Attribute type not implemented yet",
                                        x.base.base.loc);
                            }
                        } else if (AST::is_a<AST::AttrIntent_t>(*a)) {
                            AST::AttrIntent_t *ai =
                                AST::down_cast<AST::AttrIntent_t>(a);
                            switch (ai->m_intent) {
                                case (AST::attr_intentType::In) : {
                                    s_intent = LFortran::ASRUtils::intent_in;
                                    break;
                                }
                                case (AST::attr_intentType::Out) : {
                                    s_intent = LFortran::ASRUtils::intent_out;
                                    break;
                                }
                                case (AST::attr_intentType::InOut) : {
                                    s_intent = LFortran::ASRUtils::intent_inout;
                                    break;
                                }
                                default : {
                                    s_intent = LFortran::ASRUtils::intent_unspecified;
                                    break;
                                }
                            }
                        } else if (AST::is_a<AST::AttrDimension_t>(*a)) {
                            AST::AttrDimension_t *ad =
                                AST::down_cast<AST::AttrDimension_t>(a);
                            if (dims.size() > 0) {
                                throw SemanticError("Dimensions specified twice",
                                        x.base.base.loc);
                            }
                            process_dims(al, dims, ad->m_dim, ad->n_dim);
                        } else {
                            throw SemanticError("Attribute type not implemented yet",
                                    x.base.base.loc);
                        }
                    }
                }
                if (s.n_dim > 0) {
                    if (dims.size() > 0) {
                        throw SemanticError("Cannot specify dimensions both ways",
                                x.base.base.loc);
                    }
                    process_dims(al, dims, s.m_dim, s.n_dim);
                }
                ASR::ttype_t *type;
                int a_kind = 4;
                if (sym_type->m_type != AST::decl_typeType::TypeCharacter &&
                    sym_type->m_kind != nullptr &&
                    sym_type->m_kind->m_value != nullptr) {
                    visit_expr(*sym_type->m_kind->m_value);
                    ASR::expr_t* kind_expr = LFortran::ASRUtils::EXPR(asr);
                    a_kind = ASRUtils::extract_kind(kind_expr, x.base.base.loc);
                }
                if (sym_type->m_type == AST::decl_typeType::TypeReal) {
                    if (is_pointer) {
                        type = LFortran::ASRUtils::TYPE(ASR::make_RealPointer_t(al, x.base.base.loc,
                            a_kind, dims.p, dims.size()));
                    } else {
                        type = LFortran::ASRUtils::TYPE(ASR::make_Real_t(al, x.base.base.loc,
                            a_kind, dims.p, dims.size()));
                    }
                } else if (sym_type->m_type == AST::decl_typeType::TypeDoublePrecision) {
                    a_kind = 8;
                    if (is_pointer) {
                        type = LFortran::ASRUtils::TYPE(ASR::make_RealPointer_t(al, x.base.base.loc,
                            a_kind, dims.p, dims.size()));
                    } else {
                        type = LFortran::ASRUtils::TYPE(ASR::make_Real_t(al, x.base.base.loc,
                            a_kind, dims.p, dims.size()));
                    }
                } else if (sym_type->m_type == AST::decl_typeType::TypeInteger) {
                    if (is_pointer) {
                        type = LFortran::ASRUtils::TYPE(ASR::make_IntegerPointer_t(al, x.base.base.loc, a_kind, dims.p, dims.size()));
                    } else {
                        type = LFortran::ASRUtils::TYPE(ASR::make_Integer_t(al, x.base.base.loc, a_kind, dims.p, dims.size()));
                    }
                } else if (sym_type->m_type == AST::decl_typeType::TypeLogical) {
                    type = LFortran::ASRUtils::TYPE(ASR::make_Logical_t(al, x.base.base.loc, 4,
                        dims.p, dims.size()));
                } else if (sym_type->m_type == AST::decl_typeType::TypeComplex) {
                    if( is_pointer ) {
                        type = LFortran::ASRUtils::TYPE(ASR::make_ComplexPointer_t(al, x.base.base.loc, a_kind,
                                    dims.p, dims.size()));
                    } else {
                        type = LFortran::ASRUtils::TYPE(ASR::make_Complex_t(al, x.base.base.loc, a_kind,
                                    dims.p, dims.size()));
                    }
                } else if (sym_type->m_type == AST::decl_typeType::TypeCharacter) {
                    int a_len = -10;
                    ASR::expr_t *len_expr = nullptr;
                    // TODO: take into account m_kind->m_id and all kind items
                    if (sym_type->m_kind != nullptr) {
                        switch (sym_type->m_kind->m_type) {
                            case (AST::kind_item_typeType::Value) : {
                                LFORTRAN_ASSERT(sym_type->m_kind->m_value != nullptr);
                                visit_expr(*sym_type->m_kind->m_value);
                                ASR::expr_t* len_expr0 = LFortran::ASRUtils::EXPR(asr);
                                a_len = ASRUtils::extract_len(len_expr0, x.base.base.loc);
                                if (a_len == -3) {
                                    len_expr = len_expr0;
                                }
                                break;
                            }
                            case (AST::kind_item_typeType::Star) : {
                                LFORTRAN_ASSERT(sym_type->m_kind->m_value == nullptr);
                                a_len = -1;
                                break;
                            }
                            case (AST::kind_item_typeType::Colon) : {
                                LFORTRAN_ASSERT(sym_type->m_kind->m_value == nullptr);
                                a_len = -2;
                                break;
                            }
                        }
                    } else {
                        a_len = 1; // The default len of "character :: x" is 1
                    }
                    LFORTRAN_ASSERT(a_len != -10)
                    type = LFortran::ASRUtils::TYPE(ASR::make_Character_t(al, x.base.base.loc, 1, a_len, len_expr,
                        dims.p, dims.size()));
                } else if (sym_type->m_type == AST::decl_typeType::TypeType) {
                    LFORTRAN_ASSERT(sym_type->m_name);
                    std::string derived_type_name = to_lower(sym_type->m_name);
                    ASR::symbol_t *v = current_scope->resolve_symbol(derived_type_name);
                    if (!v) {
                        throw SemanticError("Derived type '"
                            + derived_type_name + "' not declared", x.base.base.loc);
                    }
                    type = LFortran::ASRUtils::TYPE(ASR::make_Derived_t(al, x.base.base.loc, v,
                        dims.p, dims.size()));
                } else if (sym_type->m_type == AST::decl_typeType::TypeClass) {
                    LFORTRAN_ASSERT(sym_type->m_name);
                    std::string derived_type_name = to_lower(sym_type->m_name);
                    ASR::symbol_t *v = current_scope->resolve_symbol(derived_type_name);
                    if (!v) {
                        throw SemanticError("Derived type '"
                            + derived_type_name + "' not declared", x.base.base.loc);
                    }
                    type = LFortran::ASRUtils::TYPE(ASR::make_Class_t(al,
                        x.base.base.loc, v, dims.p, dims.size()));
                } else {
                    throw SemanticError("Type not implemented yet.",
                         x.base.base.loc);
                }
                ASR::expr_t* init_expr = nullptr;
                ASR::expr_t* value = nullptr;
                if (s.m_initializer != nullptr) {
                    this->visit_expr(*s.m_initializer);
                    init_expr = LFortran::ASRUtils::EXPR(asr);
                    ASR::ttype_t *init_type = LFortran::ASRUtils::expr_type(init_expr);
                    ImplicitCastRules::set_converted_value(al, x.base.base.loc, &init_expr, init_type, type);
                    LFORTRAN_ASSERT(init_expr != nullptr);
                    if (storage_type == ASR::storage_typeType::Parameter) {
                        value = ASRUtils::expr_value(init_expr);
                        if (sym_type->m_type == AST::decl_typeType::TypeCharacter && value != nullptr) {
                            ASR::Character_t *lhs_type = ASR::down_cast<ASR::Character_t>(type);
                            ASR::Character_t *rhs_type = ASR::down_cast<ASR::Character_t>(ASRUtils::expr_type(value));
                            int lhs_len = lhs_type->m_len;
                            int rhs_len = rhs_type->m_len;
                            if (rhs_len >= 0) {
                                if (lhs_len == -1) {
                                    // The RHS len is known at compile time
                                    // and the LHS is inferred length
                                    lhs_len = rhs_len;
                                } else if (lhs_len >= 0) {
                                    if (lhs_len != rhs_len) {
                                        // Note: this might be valid, perhaps
                                        // change this to a warning
                                        throw SemanticError("The LHS character len="
                                            + std::to_string(lhs_len)
                                            + " and the RHS character len="
                                            + std::to_string(rhs_len)
                                            + " are not equal.", x.base.base.loc);
                                    }
                                } else {
                                    LFORTRAN_ASSERT(lhs_len == -2)
                                    throw SemanticError("The LHS character len must not be allocatable in a parameter declaration",
                                        x.base.base.loc);
                                }
                            } else {
                                throw SemanticError("The RHS character len must be known at compile time",
                                    x.base.base.loc);
                            }
                            LFORTRAN_ASSERT(lhs_len == rhs_len)
                            LFORTRAN_ASSERT(lhs_len >= 0)
                            lhs_type->m_len = lhs_len;
                        }
                    }
                }
                ASR::asr_t *v = ASR::make_Variable_t(al, x.base.base.loc, current_scope,
                        s2c(al, to_lower(s.m_name)), s_intent, init_expr, value, storage_type, type,
                        current_procedure_abi_type, s_access, s_presence,
                        value_attr);
                current_scope->scope[sym] = ASR::down_cast<ASR::symbol_t>(v);
                if( is_derived_type ) {
                    data_member_names.push_back(al, s2c(al, to_lower(s.m_name)));
                }
            } // for m_syms
        }
    }

    Vec<ASR::expr_t*> visit_expr_list(AST::fnarg_t *ast_list, size_t n) {
        Vec<ASR::expr_t*> asr_list;
        asr_list.reserve(al, n);
        ASR::expr_t *expr = nullptr;
        for (size_t i=0; i<n; i++) {
            expr = nullptr;
            if( ast_list[i].m_end != nullptr ) {
                visit_expr(*ast_list[i].m_end);
                expr = LFortran::ASRUtils::EXPR(asr);
            }
            asr_list.push_back(al, expr);
        }
        return asr_list;
    }

    void visit_FuncCallOrArray(const AST::FuncCallOrArray_t &x) {
        SymbolTable *scope = current_scope;
        std::string var_name = to_lower(x.m_func);
        ASR::symbol_t *v = current_scope->resolve_symbol(var_name);
        if (!v) {
            std::string remote_sym = var_name;
            if (intrinsic_procedures.find(remote_sym)
                        != intrinsic_procedures.end()) {
                std::string module_name = intrinsic_procedures[remote_sym];

                bool shift_scope = false;
                if (current_scope->parent->parent) {
                    current_scope = current_scope->parent;
                    shift_scope = true;
                }
                ASR::Module_t *m = LFortran::ASRUtils::load_module(al, current_scope->parent,
                    module_name, x.base.base.loc, true);
                if (shift_scope) current_scope = scope;

                ASR::symbol_t *t = m->m_symtab->resolve_symbol(remote_sym);
                if (!t) {
                    throw SemanticError("The symbol '" + remote_sym
                        + "' not found in the module '" + module_name + "'",
                        x.base.base.loc);
                }

                ASR::Function_t *mfn = ASR::down_cast<ASR::Function_t>(t);
                ASR::asr_t *fn = ASR::make_ExternalSymbol_t(
                    al, mfn->base.base.loc,
                    /* a_symtab */ current_scope,
                    /* a_name */ mfn->m_name,
                    (ASR::symbol_t*)mfn,
                    m->m_name, nullptr, 0, mfn->m_name,
                    ASR::accessType::Private
                    );
                std::string sym = to_lower(mfn->m_name);
                current_scope->scope[sym] = ASR::down_cast<ASR::symbol_t>(fn);
                v = ASR::down_cast<ASR::symbol_t>(fn);
                // Add the module `m` to current module dependencies
                if (!present(current_module_dependencies, m->m_name)) {
                    current_module_dependencies.push_back(al, m->m_name);
                }
            } else {
                throw SemanticError("Function '" + var_name + "' not found"
                    " or not implemented yet (if it is intrinsic)",
                    x.base.base.loc);
            }
        }
        Vec<ASR::expr_t*> args = visit_expr_list(x.m_args, x.n_args);
        const ASR::symbol_t *s = ASRUtils::symbol_get_past_external(v);
        if (ASR::is_a<ASR::Variable_t>(*s)) {
            // This happens for things like:
            // integer :: Y(5)
            // real :: X(Y(1)+1)
            ASR::ttype_t *type = ASR::down_cast<ASR::Variable_t>(s)->m_type;
            Vec<ASR::array_index_t> indices;
            // FIXME: convert args to indices:
            indices.p = nullptr;
            indices.n = 0;
            asr = ASR::make_ArrayRef_t(al, x.base.base.loc, v,
                indices.p, indices.size(), type, nullptr);
            return;
        } else if (ASR::is_a<ASR::Function_t>(*s)) {
            // pass
        } else {
            throw SemanticError("Expected a function call or an array", x.base.base.loc);
        }
        ASR::ttype_t *type = ASRUtils::EXPR2VAR(ASR::down_cast<ASR::Function_t>(s)->m_return_var)->m_type;
        // Add value where possible
        ASR::expr_t *value = nullptr;
        switch(args.n) {
            case 1: { // Single argument intrinsics
                if (var_name=="kind") {
                    // TODO: Refactor to allow early return
                    // kind_num --> value {4, 8, etc.}
                    int64_t kind_num = 4; // Default
                    ASR::expr_t* kind_expr = args[0];
                    // TODO: Check that the expression reduces to a valid constant expression (10.1.12)
                    switch( kind_expr->type ) {
                        case ASR::exprType::ConstantInteger: {
                            kind_num = ASR::down_cast<ASR::Integer_t>(ASR::down_cast<ASR::ConstantInteger_t>(kind_expr)->m_type)->m_kind;
                            break;
                        }
                        case ASR::exprType::ConstantReal:{
                            kind_num = ASR::down_cast<ASR::Real_t>(ASR::down_cast<ASR::ConstantReal_t>(kind_expr)->m_type)->m_kind;
                            break;
                        }
                        case ASR::exprType::ConstantLogical:{
                            kind_num = ASR::down_cast<ASR::Logical_t>(ASR::down_cast<ASR::ConstantLogical_t>(kind_expr)->m_type)->m_kind;
                            break;
                        }
                        case ASR::exprType::Var : {
                            kind_num = ASRUtils::extract_kind(kind_expr, x.base.base.loc);
                            break;
                        }
                    default: {
                        std::string msg = R"""(Only Integer literals or expressions which reduce to constant Integer are accepted as kind parameters.)""";
                        throw SemanticError(msg, x.base.base.loc);
                        break;
                    }
                    }
                    ASR::ttype_t *type = LFortran::ASRUtils::TYPE(
                            ASR::make_Integer_t(al, x.base.base.loc,
                                4, nullptr, 0));
                    value = ASR::down_cast<ASR::expr_t>(ASR::make_ConstantInteger_t(al, x.base.base.loc, kind_num,
                        type));
                }
                else if (var_name=="tiny") {
                    // We assume the input is valid
                    // ASR::expr_t* tiny_expr = args[0];
                    ASR::ttype_t* tiny_type = LFortran::ASRUtils::expr_type(args[0]);
                    // TODO: Arrays of reals are a valid argument for tiny
                    if (LFortran::ASRUtils::is_array(tiny_type)){
                        throw SemanticError("Array values not implemented yet",
                                            x.base.base.loc);
                    }
                    // TODO: Figure out how to deal with higher precision later
                    if (ASR::is_a<LFortran::ASR::Real_t>(*tiny_type)) {
                        // We don't actually need the value yet, it is enough to know it is a double
                        // but it might provide further information later (precision)
                        // double tiny_val = ASR::down_cast<ASR::ConstantReal_t>(LFortran::ASRUtils::expr_value(tiny_expr))->m_r;
                        int tiny_kind = LFortran::ASRUtils::extract_kind_from_ttype_t(tiny_type);
                        if (tiny_kind == 4){
                            float low_val = std::numeric_limits<float>::min();
                            value = ASR::down_cast<ASR::expr_t>(ASR::make_ConstantReal_t(al, x.base.base.loc,
                                                                                         low_val, // value
                                                                                         tiny_type));
                        } else {
                            double low_val = std::numeric_limits<double>::min();
                            value = ASR::down_cast<ASR::expr_t>(ASR::make_ConstantReal_t(al, x.base.base.loc,
                                                                                         low_val, // value
                                                                                         tiny_type));
                                }
                    }
                    else {
                        throw SemanticError("Argument for tiny must be Real",
                                            x.base.base.loc);
                    }
                }
                else if (var_name=="real") {
                    asr = CommonVisitorMethods::comptime_intrinsic_real(args[0], nullptr, al, x.base.base.loc);
                    return;
                }
                else if (var_name=="floor") {
                    // TODO: Implement optional kind; J3/18-007r1 --> FLOOR(A, [KIND])
                    // TODO: Rip out switch to work with optional arguments
                    ASR::expr_t* func_expr = args[0];
                    ASR::ttype_t* func_type = LFortran::ASRUtils::expr_type(func_expr);
                    int func_kind = ASRUtils::extract_kind_from_ttype_t(func_type);
                    int64_t ival {0};
                    if (LFortran::ASR::is_a<LFortran::ASR::Real_t>(*func_type)) {
                        if (func_kind == 4){
                            float rv = ASR::down_cast<ASR::ConstantReal_t>(
                                LFortran::ASRUtils::expr_value(func_expr))->m_r;
                            if (rv<0) {
                                // negative number
                                // floor -> integer(|x|+1)
                                ival = static_cast<int64_t>(rv-1);
                            } else {
                                    // positive, floor -> integer(x)
                                    ival = static_cast<int64_t>(rv);
                                }
                                value = ASR::down_cast<ASR::expr_t>(ASR::make_ConstantInteger_t(al, x.base.base.loc, ival,func_type));
                            } else {
                                double rv = ASR::down_cast<ASR::ConstantReal_t>(LFortran::ASRUtils::expr_value(func_expr))->m_r;
                                int64_t ival = static_cast<int64_t>(rv);
                                if (rv<0) {
                                    // negative number
                                    // floor -> integer(x+1)
                                    ival = static_cast<int64_t>(rv+1);
                                } else {
                                    // positive, floor -> integer(x)
                                    ival = static_cast<int64_t>(rv);
                                }
                                value = ASR::down_cast<ASR::expr_t>(ASR::make_ConstantInteger_t(al, x.base.base.loc, ival,func_type));
                        }
                    } else {
                        throw SemanticError("floor must have one real argument", x.base.base.loc);
                    }
                }
                else if (var_name=="int") {
                    ASR::expr_t* int_expr = args[0];
                    ASR::ttype_t* int_type = LFortran::ASRUtils::expr_type(int_expr);
                    int int_kind = ASRUtils::extract_kind_from_ttype_t(int_type);
                    if( int_type != nullptr ) {
                        if (LFortran::ASR::is_a<LFortran::ASR::Integer_t>(*int_type)) {
                            if (int_kind == 4){
                                int64_t ival = ASR::down_cast<ASR::ConstantInteger_t>(LFortran::ASRUtils::expr_value(int_expr))->m_n;
                                value = ASR::down_cast<ASR::expr_t>(ASR::make_ConstantInteger_t(al, x.base.base.loc, ival, int_type));
                            } else {
                                int64_t ival = ASR::down_cast<ASR::ConstantInteger_t>(LFortran::ASRUtils::expr_value(int_expr))->m_n;
                                value = ASR::down_cast<ASR::expr_t>(ASR::make_ConstantInteger_t(al, x.base.base.loc, ival, int_type));
                            }
                        }
                        else if (LFortran::ASR::is_a<LFortran::ASR::Real_t>(*int_type)) {
                            if (int_kind == 4){
                                float rv = ASR::down_cast<ASR::ConstantReal_t>(
                                    LFortran::ASRUtils::expr_value(int_expr))->m_r;
                                int64_t ival = static_cast<int64_t>(rv);
                                value = ASR::down_cast<ASR::expr_t>(ASR::make_ConstantInteger_t(al, x.base.base.loc, ival, int_type));
                            } else {
                                double rv = ASR::down_cast<ASR::ConstantReal_t>(LFortran::ASRUtils::expr_value(int_expr))->m_r;
                                int64_t ival = static_cast<int64_t>(rv);
                                value = ASR::down_cast<ASR::expr_t>(ASR::make_ConstantInteger_t(al, x.base.base.loc, ival, int_type));
                            }
                        } else {
                            throw SemanticError("int must have only one argument", x.base.base.loc);
                        }
                    }
                }
                else if (var_name=="char") {
                    ASR::expr_t* real_expr = args[0];
                    ASR::ttype_t* real_type = LFortran::ASRUtils::expr_type(real_expr);
                    if (LFortran::ASR::is_a<LFortran::ASR::Integer_t>(*real_type)) {
                        int64_t c = ASR::down_cast<ASR::ConstantInteger_t>(
                            LFortran::ASRUtils::expr_value(real_expr))->m_n;
                        ASR::ttype_t* str_type =
                            LFortran::ASRUtils::TYPE(ASR::make_Character_t(al,
                            x.base.base.loc, 1, 1, nullptr, nullptr, 0));
                        if (! (c >= 0 && c <= 127) ) {
                            throw SemanticError("The argument 'x' in char(x) must be in the range 0 <= x <= 127.", x.base.base.loc);
                        }
                        char cc = c;
                        std::string svalue;
                        svalue += cc;
                        Str s;
                        s.from_str_view(svalue);
                        char *str_val = s.c_str(al);
                        value = ASR::down_cast<ASR::expr_t>(
                            ASR::make_ConstantString_t(al, x.base.base.loc,
                            str_val, str_type));
                    } else {
                        throw SemanticError("char() must have one integer argument", x.base.base.loc);
                    }
                }
                else if (var_name=="selected_int_kind") {
                    ASR::expr_t* real_expr = args[0];
                    ASR::ttype_t* real_type = LFortran::ASRUtils::expr_type(real_expr);
                    if (LFortran::ASR::is_a<LFortran::ASR::Integer_t>(*real_type)) {
                        int64_t R = ASR::down_cast<ASR::ConstantInteger_t>(
                            LFortran::ASRUtils::expr_value(real_expr))->m_n;
                        int a_kind = 4;
                        if (R < 10) {
                            a_kind = 4;
                        } else {
                            a_kind = 8;
                        }
                        value = ASR::down_cast<ASR::expr_t>(
                            ASR::make_ConstantInteger_t(al, x.base.base.loc,
                            a_kind, real_type));
                    } else {
                        throw SemanticError("integer_int_kind() must have one integer argument", x.base.base.loc);
                    }
                }
                else if (var_name=="selected_real_kind") {
                    // TODO: Be more standards compliant 16.9.170
                    // e.g. selected_real_kind(6, 70)
                    ASR::expr_t* real_expr = args[0];
                    ASR::ttype_t* real_type = LFortran::ASRUtils::expr_type(real_expr);
                    if (LFortran::ASR::is_a<LFortran::ASR::Integer_t>(*real_type)) {
                        int64_t R = ASR::down_cast<ASR::ConstantInteger_t>(
                            LFortran::ASRUtils::expr_value(real_expr))->m_n;
                        int a_kind = 4;
                        if (R < 7) {
                            a_kind = 4;
                        } else {
                            a_kind = 8;
                        }
                        value = ASR::down_cast<ASR::expr_t>(
                            ASR::make_ConstantInteger_t(al, x.base.base.loc,
                            a_kind, real_type));
                    } else {
                        throw SemanticError("integer_real_kind() must have one integer argument", x.base.base.loc);
                    }
                }
                break;
            }
            case 2: {
                if (var_name=="real") {
                    asr = CommonVisitorMethods::comptime_intrinsic_real(args[0], args[1], al, x.base.base.loc);
                    return;
                } else {
                    throw SemanticError("Function '" + var_name + "' with " + std::to_string(args.n) +
                            " arguments not supported yet",
                            x.base.base.loc);
                }
                break;
            }
            default:  { // Not implemented
                throw SemanticError("Function '" + var_name + "' with " + std::to_string(args.n) +
                        " arguments not supported yet",
                        x.base.base.loc);
            }
        }
        asr = ASR::make_FunctionCall_t(al, x.base.base.loc, v, nullptr,
            args.p, args.size(), nullptr, 0, type, value, nullptr);
    }

    void visit_DerivedType(const AST::DerivedType_t &x) {
        SymbolTable *parent_scope = current_scope;
        current_scope = al.make_new<SymbolTable>(parent_scope);
        data_member_names.reserve(al, 0);
        is_derived_type = true;
        dt_name = to_lower(x.m_name);
        AST::AttrExtends_t *attr_extend = nullptr;
        for( size_t i = 0; i < x.n_attrtype; i++ ) {
            switch( x.m_attrtype[i]->type ) {
                case AST::decl_attributeType::AttrExtends: {
                    if( attr_extend != nullptr ) {
                        throw SemanticError("DerivedType can only extend one another DerivedType",
                                            x.base.base.loc);
                    }
                    attr_extend = (AST::AttrExtends_t*)(&(x.m_attrtype[i]->base));
                    break;
                }
                default:
                    break;
            }
        }
        for (size_t i=0; i<x.n_items; i++) {
            this->visit_unit_decl2(*x.m_items[i]);
        }
        for (size_t i=0; i<x.n_contains; i++) {
            visit_procedure_decl(*x.m_contains[i]);
        }
        std::string sym_name = to_lower(x.m_name);
        if (current_scope->scope.find(sym_name) != current_scope->scope.end()) {
            throw SemanticError("DerivedType already defined", x.base.base.loc);
        }
        ASR::symbol_t* parent_sym = nullptr;
        if( attr_extend != nullptr ) {
            std::string parent_sym_name = to_lower(attr_extend->m_name);
            if( parent_scope->scope.find(parent_sym_name) == parent_scope->scope.end() ) {
                throw SemanticError(parent_sym_name + " is not defined.", x.base.base.loc);
            }
            parent_sym = parent_scope->scope[parent_sym_name];
        }
        asr = ASR::make_DerivedType_t(al, x.base.base.loc, current_scope,
                s2c(al, to_lower(x.m_name)), data_member_names.p, data_member_names.size(),
                ASR::abiType::Source, dflt_access, parent_sym);
        parent_scope->scope[sym_name] = ASR::down_cast<ASR::symbol_t>(asr);
        current_scope = parent_scope;
        is_derived_type = false;
    }

    void visit_InterfaceProc(const AST::InterfaceProc_t &x) {
        is_interface = true;
        visit_program_unit(*x.m_proc);
        is_interface = false;
        return;
    }

    void visit_DerivedTypeProc(const AST::DerivedTypeProc_t &x) {
        for (size_t i = 0; i < x.n_symbols; i++) {
            AST::UseSymbol_t *use_sym = AST::down_cast<AST::UseSymbol_t>(
                x.m_symbols[i]);
            if (use_sym->m_rename) {
                class_procedures[dt_name][to_lower(use_sym->m_rename)] = to_lower(use_sym->m_sym);
            } else {
                class_procedures[dt_name][to_lower(use_sym->m_sym)] = to_lower(use_sym->m_sym);
            }
        }
    }

    void fill_interface_proc_names(const AST::Interface_t& x,
                                    std::vector<std::string>& proc_names) {
        for (size_t i = 0; i < x.n_items; i++) {
            AST::interface_item_t *item = x.m_items[i];
            if (AST::is_a<AST::InterfaceModuleProcedure_t>(*item)) {
                AST::InterfaceModuleProcedure_t *proc
                    = AST::down_cast<AST::InterfaceModuleProcedure_t>(item);
                for (size_t i = 0; i < proc->n_names; i++) {
                    /* Check signatures of procedures
                    * to ensure there are no two procedures
                    * with same signatures.
                    */
                    char *proc_name = proc->m_names[i];
                    proc_names.push_back(std::string(proc_name));
                }
            } else {
                throw SemanticError("Interface procedure type not imlemented yet", item->base.loc);
            }
        }
    }

    void visit_Interface(const AST::Interface_t &x) {
        if (AST::is_a<AST::InterfaceHeaderName_t>(*x.m_header)) {
            std::string generic_name = to_lower(AST::down_cast<AST::InterfaceHeaderName_t>(x.m_header)->m_name);
            std::vector<std::string> proc_names;
            fill_interface_proc_names(x, proc_names);
            generic_procedures[std::string(generic_name)] = proc_names;
        } else if (AST::is_a<AST::InterfaceHeader_t>(*x.m_header)) {
            std::vector<std::string> proc_names;
            for (size_t i = 0; i < x.n_items; i++) {
                visit_interface_item(*x.m_items[i]);
            }
        } else if (AST::is_a<AST::InterfaceHeaderOperator_t>(*x.m_header)) {
            AST::intrinsicopType opType = AST::down_cast<AST::InterfaceHeaderOperator_t>(x.m_header)->m_op;
            std::vector<std::string> proc_names;
            fill_interface_proc_names(x, proc_names);
            overloaded_op_procs[opType] = proc_names;
        } else {
            throw SemanticError("Interface type not imlemented yet", x.base.base.loc);
        }
    }

    void add_overloaded_procedures() {
        for (auto &proc : overloaded_op_procs) {
            Location loc;
            loc.first_line = 1;
            loc.last_line = 1;
            loc.first_column = 1;
            loc.last_column = 1;
            Str s;
            s.from_str_view(intrinsic2str[proc.first]);
            char *generic_name = s.c_str(al);
            Vec<ASR::symbol_t*> symbols;
            symbols.reserve(al, proc.second.size());
            for (auto &pname : proc.second) {
                ASR::symbol_t *x;
                Str s;
                s.from_str_view(pname);
                char *name = s.c_str(al);
                x = resolve_symbol(loc, name);
                symbols.push_back(al, x);
            }
            ASR::asr_t *v = ASR::make_CustomOperator_t(al, loc, current_scope,
                                generic_name, symbols.p, symbols.size(), ASR::Public);
            current_scope->scope[intrinsic2str[proc.first]] = ASR::down_cast<ASR::symbol_t>(v);
        }
        overloaded_op_procs.clear();
    }

    void add_generic_procedures() {
        for (auto &proc : generic_procedures) {
            Location loc;
            loc.first_line = 1;
            loc.last_line = 1;
            loc.first_column = 1;
            loc.last_column = 1;
            Str s;
            s.from_str_view(proc.first);
            char *generic_name = s.c_str(al);
            Vec<ASR::symbol_t*> symbols;
            symbols.reserve(al, proc.second.size());
            for (auto &pname : proc.second) {
                ASR::symbol_t *x;
                Str s;
                s.from_str_view(pname);
                char *name = s.c_str(al);
                x = resolve_symbol(loc, name);
                symbols.push_back(al, x);
            }
            ASR::asr_t *v = ASR::make_GenericProcedure_t(al, loc,
                current_scope,
                generic_name, symbols.p, symbols.size(), ASR::Public);
            current_scope->scope[proc.first] = ASR::down_cast<ASR::symbol_t>(v);
        }
    }

    void add_class_procedures() {
        for (auto &proc : class_procedures) {
            Location loc;
            loc.first_line = 1;
            loc.last_line = 1;
            loc.first_column = 1;
            loc.last_column = 1;
            ASR::DerivedType_t *clss = ASR::down_cast<ASR::DerivedType_t>(
                current_scope->scope[proc.first]);
            for (auto &pname : proc.second) {
                ASR::symbol_t *proc_sym = current_scope->scope[pname.second];
                Str s;
                s.from_str_view(pname.first);
                char *name = s.c_str(al);
                s.from_str_view(pname.second);
                char *proc_name = s.c_str(al);
                ASR::asr_t *v = ASR::make_ClassProcedure_t(al, loc,
                    clss->m_symtab, name, proc_name, proc_sym,
                    ASR::abiType::Source);
                ASR::symbol_t *cls_proc_sym = ASR::down_cast<ASR::symbol_t>(v);
                clss->m_symtab->scope[pname.first] = cls_proc_sym;
            }
        }
    }

    void visit_Use(const AST::Use_t &x) {
        std::string msym = to_lower(x.m_module);
        Str msym_c; msym_c.from_str_view(msym);
        char *msym_cc = msym_c.c_str(al);
        if (!present(current_module_dependencies, msym_cc)) {
            current_module_dependencies.push_back(al, msym_cc);
        }
        ASR::symbol_t *t = current_scope->parent->resolve_symbol(msym);
        if (!t) {
            t = (ASR::symbol_t*)LFortran::ASRUtils::load_module(al, current_scope->parent,
                msym, x.base.base.loc, false);
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
                    ASR::asr_t *sub = ASR::make_ExternalSymbol_t(
                        al, msub->base.base.loc,
                        /* a_symtab */ current_scope,
                        /* a_name */ msub->m_name,
                        (ASR::symbol_t*)msub,
                        m->m_name, nullptr, 0, msub->m_name,
                        dflt_access
                        );
                    std::string sym = to_lower(msub->m_name);
                    current_scope->scope[sym] = ASR::down_cast<ASR::symbol_t>(sub);
                } else if (ASR::is_a<ASR::Function_t>(*item.second)) {
                    ASR::Function_t *mfn = ASR::down_cast<ASR::Function_t>(item.second);
                    ASR::asr_t *fn = ASR::make_ExternalSymbol_t(
                        al, mfn->base.base.loc,
                        /* a_symtab */ current_scope,
                        /* a_name */ mfn->m_name,
                        (ASR::symbol_t*)mfn,
                        m->m_name, nullptr, 0, mfn->m_name,
                        dflt_access
                        );
                    std::string sym = to_lower(mfn->m_name);
                    current_scope->scope[sym] = ASR::down_cast<ASR::symbol_t>(fn);
                } else if (ASR::is_a<ASR::GenericProcedure_t>(*item.second)) {
                    ASR::GenericProcedure_t *gp = ASR::down_cast<
                        ASR::GenericProcedure_t>(item.second);
                    ASR::asr_t *ep = ASR::make_ExternalSymbol_t(
                        al, gp->base.base.loc,
                        current_scope,
                        /* a_name */ gp->m_name,
                        (ASR::symbol_t*)gp,
                        m->m_name, nullptr, 0, gp->m_name,
                        dflt_access
                        );
                    std::string sym = to_lower(gp->m_name);
                    current_scope->scope[sym] = ASR::down_cast<ASR::symbol_t>(ep);
                }  else if (ASR::is_a<ASR::CustomOperator_t>(*item.second)) {
                    ASR::CustomOperator_t *gp = ASR::down_cast<
                        ASR::CustomOperator_t>(item.second);
                    ASR::asr_t *ep = ASR::make_ExternalSymbol_t(
                        al, gp->base.base.loc,
                        current_scope,
                        /* a_name */ gp->m_name,
                        (ASR::symbol_t*)gp,
                        m->m_name, nullptr, 0, gp->m_name,
                        dflt_access
                        );
                    std::string sym = to_lower(gp->m_name);
                    current_scope->scope[sym] = ASR::down_cast<ASR::symbol_t>(ep);
                } else if (ASR::is_a<ASR::Variable_t>(*item.second)) {
                    ASR::Variable_t *mvar = ASR::down_cast<ASR::Variable_t>(item.second);
                    ASR::asr_t *var = ASR::make_ExternalSymbol_t(
                        al, mvar->base.base.loc,
                        /* a_symtab */ current_scope,
                        /* a_name */ mvar->m_name,
                        (ASR::symbol_t*)mvar,
                        m->m_name, nullptr, 0, mvar->m_name,
                        dflt_access
                        );
                    std::string sym = to_lower(mvar->m_name);
                    current_scope->scope[sym] = ASR::down_cast<ASR::symbol_t>(var);
                } else if (ASR::is_a<ASR::ExternalSymbol_t>(*item.second)) {
                    // We have to "repack" the ExternalSymbol so that it lives in the
                    // local symbol table
                    ASR::ExternalSymbol_t *es0 = ASR::down_cast<ASR::ExternalSymbol_t>(item.second);
                    ASR::asr_t *es = ASR::make_ExternalSymbol_t(
                        al, es0->base.base.loc,
                        /* a_symtab */ current_scope,
                        /* a_name */ es0->m_name,
                        es0->m_external,
                        es0->m_module_name, nullptr, 0,
                        es0->m_original_name,
                        dflt_access
                        );
                    std::string sym = to_lower(es0->m_original_name);
                    current_scope->scope[sym] = ASR::down_cast<ASR::symbol_t>(es);
                } else {
                    throw LFortranException("'" + item.first + "' is not supported yet for declaring with use.");
                }
            }
        } else {
            // Only import individual symbols from the module, e.g.:
            //     use a, only: x, y, z
            for (size_t i = 0; i < x.n_symbols; i++) {
                std::string remote_sym = to_lower(AST::down_cast<AST::UseSymbol_t>(x.m_symbols[i])->m_sym);
                std::string local_sym;
                if (AST::down_cast<AST::UseSymbol_t>(x.m_symbols[i])->m_rename) {
                    local_sym = to_lower(AST::down_cast<AST::UseSymbol_t>(x.m_symbols[i])->m_rename);
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
                    // an ExternalSymbol that points to
                    // `msub` via the `external` field.
                    Str name;
                    name.from_str(al, local_sym);
                    ASR::asr_t *sub = ASR::make_ExternalSymbol_t(
                        al, msub->base.base.loc,
                        /* a_symtab */ current_scope,
                        /* a_name */ name.c_str(al),
                        (ASR::symbol_t*)msub,
                        m->m_name, nullptr, 0, msub->m_name,
                        dflt_access
                        );
                    current_scope->scope[local_sym] = ASR::down_cast<ASR::symbol_t>(sub);
                } else if (ASR::is_a<ASR::GenericProcedure_t>(*t)) {
                    if (current_scope->scope.find(local_sym) != current_scope->scope.end()) {
                        throw SemanticError("Symbol already defined",
                            x.base.base.loc);
                    }
                    ASR::GenericProcedure_t *gp = ASR::down_cast<ASR::GenericProcedure_t>(t);
                    Str name;
                    name.from_str(al, local_sym);
                    char *cname = name.c_str(al);
                    ASR::asr_t *ep = ASR::make_ExternalSymbol_t(
                        al, t->base.loc,
                        current_scope,
                        /* a_name */ cname,
                        t,
                        m->m_name, nullptr, 0, gp->m_name,
                        dflt_access
                        );
                    current_scope->scope[local_sym] = ASR::down_cast<ASR::symbol_t>(ep);
                } else if (ASR::is_a<ASR::ExternalSymbol_t>(*t)) {
                    if (current_scope->scope.find(local_sym) != current_scope->scope.end()) {
                        throw SemanticError("Symbol already defined",
                            x.base.base.loc);
                    }
                    // Repack ExternalSymbol to point directly to the original symbol
                    ASR::ExternalSymbol_t *es = ASR::down_cast<ASR::ExternalSymbol_t>(t);
                    ASR::asr_t *ep = ASR::make_ExternalSymbol_t(
                        al, es->base.base.loc,
                        current_scope,
                        /* a_name */ es->m_name,
                        es->m_external,
                        es->m_module_name, es->m_scope_names, es->n_scope_names, es->m_original_name,
                        es->m_access
                        );
                    current_scope->scope[local_sym] = ASR::down_cast<ASR::symbol_t>(ep);
                } else if (ASR::is_a<ASR::Function_t>(*t)) {
                    if (current_scope->scope.find(local_sym) != current_scope->scope.end()) {
                        throw SemanticError("Function already defined",
                            x.base.base.loc);
                    }
                    ASR::Function_t *mfn = ASR::down_cast<ASR::Function_t>(t);
                    // `mfn` is the Function in a module. Now we construct
                    // an ExternalSymbol that points to it.
                    Str name;
                    name.from_str(al, local_sym);
                    char *cname = name.c_str(al);
                    ASR::asr_t *fn = ASR::make_ExternalSymbol_t(
                        al, mfn->base.base.loc,
                        /* a_symtab */ current_scope,
                        /* a_name */ cname,
                        (ASR::symbol_t*)mfn,
                        m->m_name, nullptr, 0, mfn->m_name,
                        dflt_access
                        );
                    current_scope->scope[local_sym] = ASR::down_cast<ASR::symbol_t>(fn);
                } else if (ASR::is_a<ASR::Variable_t>(*t)) {
                    if (current_scope->scope.find(local_sym) != current_scope->scope.end()) {
                        throw SemanticError("Variable already defined",
                            x.base.base.loc);
                    }
                    ASR::Variable_t *mv = ASR::down_cast<ASR::Variable_t>(t);
                    // `mv` is the Variable in a module. Now we construct
                    // an ExternalSymbol that points to it.
                    Str name;
                    name.from_str(al, local_sym);
                    char *cname = name.c_str(al);
                    ASR::asr_t *v = ASR::make_ExternalSymbol_t(
                        al, mv->base.base.loc,
                        /* a_symtab */ current_scope,
                        /* a_name */ cname,
                        (ASR::symbol_t*)mv,
                        m->m_name, nullptr, 0, mv->m_name,
                        dflt_access
                        );
                    current_scope->scope[local_sym] = ASR::down_cast<ASR::symbol_t>(v);
                } else if( ASR::is_a<ASR::DerivedType_t>(*t) ) {
                    if (current_scope->scope.find(local_sym) != current_scope->scope.end()) {
                        throw SemanticError("Derived type already defined",
                            x.base.base.loc);
                    }
                    ASR::DerivedType_t *mv = ASR::down_cast<ASR::DerivedType_t>(t);
                    // `mv` is the Variable in a module. Now we construct
                    // an ExternalSymbol that points to it.
                    Str name;
                    name.from_str(al, local_sym);
                    char *cname = name.c_str(al);
                    ASR::asr_t *v = ASR::make_ExternalSymbol_t(
                        al, mv->base.base.loc,
                        /* a_symtab */ current_scope,
                        /* a_name */ cname,
                        (ASR::symbol_t*)mv,
                        m->m_name, nullptr, 0, mv->m_name,
                        dflt_access
                        );
                    current_scope->scope[local_sym] = ASR::down_cast<ASR::symbol_t>(v);
                } else {
                    throw LFortranException("Only Subroutines, Functions, Variables and Derived supported in 'use'");
                }
            }
        }
    }

    void visit_Real(const AST::Real_t &x) {
        double r = ASRUtils::extract_real(x.m_n);
        char* s_kind;
        int r_kind = ASRUtils::extract_kind_str(x.m_n, s_kind);
        if (r_kind == 0) {
            std::string var_name = to_lower(s_kind);
            ASR::symbol_t *v = current_scope->resolve_symbol(var_name);
            if (v) {
                const ASR::symbol_t *v3 = LFortran::ASRUtils::symbol_get_past_external(v);
                if (ASR::is_a<ASR::Variable_t>(*v3)) {
                    ASR::Variable_t *v2 = ASR::down_cast<ASR::Variable_t>(v3);
                    if (v2->m_value) {
                        if (ASR::is_a<ASR::ConstantInteger_t>(*v2->m_value)) {
                            r_kind = ASR::down_cast<ASR::ConstantInteger_t>(v2->m_value)->m_n;
                        } else {
                            throw SemanticError("Variable '" + var_name + "' is constant but not an integer",
                                x.base.base.loc);
                        }
                    } else {
                        throw SemanticError("Variable '" + var_name + "' is not constant",
                            x.base.base.loc);
                    }
                } else {
                    throw SemanticError("Symbol '" + var_name + "' is not a variable",
                        x.base.base.loc);
                }
            } else {
                throw SemanticError("Variable '" + var_name + "' not declared",
                    x.base.base.loc);
            }
        }
        ASR::ttype_t *type = LFortran::ASRUtils::TYPE(ASR::make_Real_t(al, x.base.base.loc,
                r_kind, nullptr, 0));
        asr = ASR::make_ConstantReal_t(al, x.base.base.loc, r, type);
    }

    ASR::asr_t* resolve_variable(const Location &loc, const std::string &var_name) {
        SymbolTable *scope = current_scope;
        ASR::symbol_t *v = scope->resolve_symbol(var_name);
        if (!v) {
            throw SemanticError("Variable '" + var_name + "' not declared", loc);
        }
        return ASR::make_Var_t(al, loc, v);
    }

    void visit_Name(const AST::Name_t &x) {
        asr = resolve_variable(x.base.base.loc, to_lower(x.m_id));
    }

    void visit_Num(const AST::Num_t &x) {
        int ikind = 4;
        if (x.m_kind) {
            ikind = std::atoi(x.m_kind);
            if (ikind == 0) {
                std::string var_name = to_lower(x.m_kind);
                ASR::symbol_t *v = current_scope->resolve_symbol(var_name);
                if (v) {
                    const ASR::symbol_t *v3 = LFortran::ASRUtils::symbol_get_past_external(v);
                    if (ASR::is_a<ASR::Variable_t>(*v3)) {
                        ASR::Variable_t *v2 = ASR::down_cast<ASR::Variable_t>(v3);
                        if (v2->m_value) {
                            if (ASR::is_a<ASR::ConstantInteger_t>(*v2->m_value)) {
                                ikind = ASR::down_cast<ASR::ConstantInteger_t>(v2->m_value)->m_n;
                            } else {
                                throw SemanticError("Variable '" + var_name + "' is constant but not an integer",
                                    x.base.base.loc);
                            }
                        } else {
                            throw SemanticError("Variable '" + var_name + "' is not constant",
                                x.base.base.loc);
                        }
                    } else {
                        throw SemanticError("Symbol '" + var_name + "' is not a variable",
                            x.base.base.loc);
                    }
                } else {
                    throw SemanticError("Variable '" + var_name + "' not declared",
                        x.base.base.loc);
                }
            }
        }
        ASR::ttype_t *type = LFortran::ASRUtils::TYPE(ASR::make_Integer_t(al, x.base.base.loc,
                ikind, nullptr, 0));
        if (BigInt::is_int_ptr(x.m_n)) {
            throw SemanticError("Integer constants larger than 2^62-1 are not implemented yet", x.base.base.loc);
        } else {
            LFORTRAN_ASSERT(!BigInt::is_int_ptr(x.m_n));
            asr = ASR::make_ConstantInteger_t(al, x.base.base.loc, x.m_n, type);
        }
    }

    void visit_Parenthesis(const AST::Parenthesis_t &x) {
        visit_expr(*x.m_operand);
    }

};

ASR::asr_t *symbol_table_visitor(Allocator &al, AST::TranslationUnit_t &ast,
        SymbolTable *symbol_table)
{
    SymbolTableVisitor v(al, symbol_table);
    v.visit_TranslationUnit(ast);
    ASR::asr_t *unit = v.asr;
    return unit;
}

} // namespace LFortran
