#ifndef LFORTRAN_PASS_UTILS_H
#define LFORTRAN_PASS_UTILS_H

#include <libasr/asr.h>
#include <libasr/containers.h>

namespace LFortran {

    namespace PassUtils {

        bool is_array(ASR::expr_t* x);

        void get_dim_rank(ASR::ttype_t* x_type, ASR::dimension_t*& m_dims, int& n_dims);

        ASR::ttype_t* set_dim_rank(ASR::ttype_t* x_type, ASR::dimension_t*& m_dims, int& n_dims,
                                    bool create_new=false, Allocator* al=nullptr);

        int get_rank(ASR::expr_t* x);

        ASR::expr_t* create_array_ref(ASR::expr_t* arr_expr, Vec<ASR::expr_t*>& idx_vars, Allocator& al);

        ASR::expr_t* create_array_ref(ASR::symbol_t* arr, Vec<ASR::expr_t*>& idx_vars, Allocator& al,
                                      const Location& loc, ASR::ttype_t* _type);

        void create_idx_vars(Vec<ASR::expr_t*>& idx_vars, int n_dims, const Location& loc,
                             Allocator& al, SymbolTable*& current_scope, std::string suffix="_k");

        ASR::expr_t* get_bound(ASR::expr_t* arr_expr, int dim, std::string bound,
                                Allocator& al, ASR::TranslationUnit_t& unit,
                                const std::string &rl_path,
                                SymbolTable*& current_scope);


        ASR::stmt_t* get_flipsign(ASR::expr_t* arg0, ASR::expr_t* arg1,
                                  Allocator& al, ASR::TranslationUnit_t& unit,
                                  const std::string &rl_path,
                                  SymbolTable*& current_scope,
                                  const std::function<void (const std::string &, const Location &)> err);

        ASR::expr_t* to_int32(ASR::expr_t* x, ASR::ttype_t* int32type, Allocator& al);

        bool is_slice_present(const ASR::ArrayRef_t& x);

        bool is_slice_present(const ASR::expr_t* x);

        ASR::expr_t* create_auxiliary_variable_for_expr(ASR::expr_t* expr, std::string& name,
            Allocator& al, SymbolTable*& current_scope, ASR::stmt_t*& assign_stmt);

        ASR::expr_t* create_auxiliary_variable(Location& loc, std::string& name,
            Allocator& al, SymbolTable*& current_scope, ASR::ttype_t* var_type);

        ASR::expr_t* get_fma(ASR::expr_t* arg0, ASR::expr_t* arg1, ASR::expr_t* arg2,
                             Allocator& al, ASR::TranslationUnit_t& unit, std::string& rl_path,
                             SymbolTable*& current_scope,Location& loc,
                             const std::function<void (const std::string &, const Location &)> err);

        ASR::expr_t* get_sign_from_value(ASR::expr_t* arg0, ASR::expr_t* arg1,
                                         Allocator& al, ASR::TranslationUnit_t& unit, std::string& rl_path,
                                         SymbolTable*& current_scope, Location& loc,
                                         const std::function<void (const std::string &, const Location &)> err);

        template <class Derived>
        class PassVisitor: public ASR::BaseWalkVisitor<Derived> {

            private:

                Derived& self() { return static_cast<Derived&>(*this); }

            public:

                bool asr_changed, retain_original_stmt;
                Allocator& al;
                Vec<ASR::stmt_t*> pass_result;
                SymbolTable* current_scope;

                PassVisitor(Allocator& al_, SymbolTable* current_scope_): al{al_},
                current_scope{current_scope_} {
                    pass_result.n = 0;
                }

                void transform_stmts(ASR::stmt_t **&m_body, size_t &n_body) {
                    Vec<ASR::stmt_t*> body;
                    body.reserve(al, n_body);
                    for (size_t i=0; i<n_body; i++) {
                        // Not necessary after we check it after each visit_stmt in every
                        // visitor method:
                        pass_result.n = 0;
                        retain_original_stmt = false;
                        self().visit_stmt(*m_body[i]);
                        if (pass_result.size() > 0) {
                            asr_changed = true;
                            for (size_t j=0; j < pass_result.size(); j++) {
                                body.push_back(al, pass_result[j]);
                            }
                            if( retain_original_stmt ) {
                                body.push_back(al, m_body[i]);
                                retain_original_stmt = false;
                            }
                            pass_result.n = 0;
                        } else {
                            body.push_back(al, m_body[i]);
                        }
                    }
                    m_body = body.p;
                    n_body = body.size();
                }

                void visit_Program(const ASR::Program_t &x) {
                    // FIXME: this is a hack, we need to pass in a non-const `x`,
                    // which requires to generate a TransformVisitor.
                    ASR::Program_t &xx = const_cast<ASR::Program_t&>(x);
                    current_scope = xx.m_symtab;
                    transform_stmts(xx.m_body, xx.n_body);

                    // Transform nested functions and subroutines
                    for (auto &item : x.m_symtab->scope) {
                        if (ASR::is_a<ASR::Subroutine_t>(*item.second)) {
                            ASR::Subroutine_t *s = ASR::down_cast<ASR::Subroutine_t>(item.second);
                            self().visit_Subroutine(*s);
                        }
                        if (ASR::is_a<ASR::Function_t>(*item.second)) {
                            ASR::Function_t *s = ASR::down_cast<ASR::Function_t>(item.second);
                            self().visit_Function(*s);
                        }
                    }
                }

                void visit_Subroutine(const ASR::Subroutine_t &x) {
                    // FIXME: this is a hack, we need to pass in a non-const `x`,
                    // which requires to generate a TransformVisitor.
                    ASR::Subroutine_t &xx = const_cast<ASR::Subroutine_t&>(x);
                    current_scope = xx.m_symtab;
                    transform_stmts(xx.m_body, xx.n_body);
                }

                void visit_Function(const ASR::Function_t &x) {
                    // FIXME: this is a hack, we need to pass in a non-const `x`,
                    // which requires to generate a TransformVisitor.
                    ASR::Function_t &xx = const_cast<ASR::Function_t&>(x);
                    current_scope = xx.m_symtab;
                    transform_stmts(xx.m_body, xx.n_body);
                }

        };

        template <class Derived>
        class SkipOptimizationSubroutineVisitor: public PassVisitor<Derived> {

            public:

                SkipOptimizationSubroutineVisitor(Allocator& al_): PassVisitor<Derived>(al_, nullptr) {
                }

                void visit_Subroutine(const ASR::Subroutine_t &x) {
                    if( ASRUtils::is_intrinsic_optimization<ASR::Subroutine_t>(&x) ) {
                        return ;
                    }
                    PassUtils::PassVisitor<Derived>::visit_Subroutine(x);
                }

        };

        template <class Derived>
        class SkipOptimizationFunctionVisitor: public PassVisitor<Derived> {

            public:

                SkipOptimizationFunctionVisitor(Allocator& al_): PassVisitor<Derived>(al_, nullptr) {
                }

                void visit_Function(const ASR::Function_t &x) {
                    if( ASRUtils::is_intrinsic_optimization<ASR::Function_t>(&x) ) {
                        return ;
                    }
                    PassUtils::PassVisitor<Derived>::visit_Function(x);
                }

        };

        class NodeDuplicator {
            private:

                Allocator& al;

            public:

                bool success;

                NodeDuplicator(Allocator& al_) : al(al_), success(false) {}

                ASR::stmt_t* duplicate_stmt(ASR::stmt_t* x) {
                    if( !x ) {
                        return nullptr;
                    }

                    switch( x->type ) {
                        case ASR::stmtType::Assignment: {
                            return duplicate_Assignment(ASR::down_cast<ASR::Assignment_t>(x));
                        }
                        case ASR::stmtType::If: {
                            return duplicate_If(ASR::down_cast<ASR::If_t>(x));
                        }
                        default: {
                            LFORTRAN_ASSERT_MSG(false, "Duplication of " + std::to_string(x->type) + " statement is not supported yet.");
                        }
                    }
                }

                ASR::expr_t* duplicate_expr(ASR::expr_t* x) {
                    if( !x ) {
                        return nullptr;
                    }

                    switch( x->type ) {
                        case ASR::exprType::BinOp: {
                            return duplicate_BinOp(ASR::down_cast<ASR::BinOp_t>(x));
                        }
                        case ASR::exprType::Var: {
                            return duplicate_Var(ASR::down_cast<ASR::Var_t>(x));
                        }
                        case ASR::exprType::BoolOp: {
                            return duplicate_BoolOp(ASR::down_cast<ASR::BoolOp_t>(x));
                        }
                        case ASR::exprType::Compare: {
                            return duplicate_Compare(ASR::down_cast<ASR::Compare_t>(x));
                        }
                        case ASR::exprType::UnaryOp: {
                            return duplicate_UnaryOp(ASR::down_cast<ASR::UnaryOp_t>(x));
                        }
                        case ASR::exprType::FunctionCall: {
                            success = false;
                            return nullptr;
                        }
                        case ASR::exprType::ConstantInteger: {

                        }
                        case ASR::exprType::ConstantReal: {
                            return x;
                        }
                        default: {
                            LFORTRAN_ASSERT_MSG(false, "Duplication of " + std::to_string(x->type) + " expression is not supported yet.");
                        }
                    }
                }

                ASR::expr_t* duplicate_Var(ASR::Var_t* x) {
                    return ASRUtils::EXPR(ASR::make_Var_t(al, x->base.base.loc, x->m_v));
                }

                ASR::expr_t* duplicate_FunctionCall(ASR::FunctionCall_t* x) {
                    if( !ASR::is_a<ASR::Function_t>(*(x->m_name)) ) {
                        success = false;
                        return nullptr;
                    }
                    Vec<ASR::call_arg_t> m_args;
                    m_args.reserve(al, x->n_args);
                    for( size_t i = 0; i < x->n_args; i++ ) {
                        ASR::call_arg_t call_arg;
                        call_arg.m_value = duplicate_expr(x->m_args[i].m_value);
                        call_arg.loc = x->m_args[i].loc;
                        m_args.push_back(al, call_arg);
                    }
                    ASR::expr_t *m_value = nullptr, *m_dt = nullptr;
                    m_value = duplicate_expr(x->m_value);
                    m_dt = duplicate_expr(x->m_dt);
                    return ASRUtils::EXPR(ASR::make_FunctionCall_t(al, x->base.base.loc,
                            x->m_name, x->m_original_name, m_args.p, x->n_args, x->m_type,
                            m_value, m_dt));
                }

                ASR::expr_t* duplicate_Compare(ASR::Compare_t* x) {
                    ASR::expr_t *m_left = nullptr, *m_right = nullptr;
                    ASR::expr_t *m_value = nullptr, *m_overloaded = nullptr;
                    m_left = duplicate_expr(x->m_left);
                    m_right = duplicate_expr(x->m_right);
                    m_value = duplicate_expr(x->m_value);
                    m_overloaded = duplicate_expr(x->m_overloaded);
                    return ASRUtils::EXPR(ASR::make_Compare_t(al, x->base.base.loc,
                                m_left, x->m_op, m_right, x->m_type, m_value, m_overloaded));
                }

                ASR::expr_t* duplicate_UnaryOp(ASR::UnaryOp_t* x) {
                    ASR::expr_t *m_operand = nullptr;
                    ASR::expr_t *m_value = nullptr;
                    m_operand = duplicate_expr(x->m_operand);
                    m_value = duplicate_expr(x->m_value);
                    return ASRUtils::EXPR(ASR::make_UnaryOp_t(al, x->base.base.loc,
                                x->m_op, m_operand, x->m_type, m_value));
                }

                ASR::expr_t* duplicate_BoolOp(ASR::BoolOp_t* x) {
                    ASR::expr_t *m_left = nullptr, *m_right = nullptr;
                    ASR::expr_t *m_value = nullptr;
                    m_left = duplicate_expr(x->m_left);
                    m_right = duplicate_expr(x->m_right);
                    m_value = duplicate_expr(x->m_value);
                    return ASRUtils::EXPR(ASR::make_BoolOp_t(al, x->base.base.loc,
                                m_left, x->m_op, m_right, x->m_type, m_value));
                }

                ASR::expr_t* duplicate_BinOp(ASR::BinOp_t* x) {
                    ASR::expr_t *m_left = nullptr, *m_right = nullptr;
                    ASR::expr_t *m_value = nullptr, *m_overloaded = nullptr;
                    m_left = duplicate_expr(x->m_left);
                    m_right = duplicate_expr(x->m_right);
                    m_value = duplicate_expr(x->m_value);
                    m_overloaded = duplicate_expr(x->m_overloaded);
                    return ASRUtils::EXPR(ASR::make_BinOp_t(al, x->base.base.loc,
                                m_left, x->m_op, m_right, x->m_type, m_value, m_overloaded));
                }

                ASR::stmt_t* duplicate_Assignment(ASR::Assignment_t* x) {
                    ASR::expr_t *m_target = nullptr, *m_value = nullptr;
                    ASR::stmt_t *m_overloaded = nullptr;
                    m_target = duplicate_expr(x->m_target);
                    m_value = duplicate_expr(x->m_value);
                    m_overloaded = duplicate_stmt(x->m_overloaded);
                    return ASRUtils::STMT(ASR::make_Assignment_t(
                        al, x->base.base.loc, m_target, m_value, m_overloaded));
                }

                ASR::stmt_t* duplicate_If(ASR::If_t* x) {
                    ASR::expr_t *m_test = nullptr;
                    Vec<ASR::stmt_t*> m_body, m_orelse;
                    m_body.reserve(al, x->n_body);
                    m_orelse.reserve(al, x->n_orelse);
                    m_test = duplicate_expr(x->m_test);
                    for( size_t i = 0; i < x->n_body; i++ ) {
                        m_body.push_back(al, duplicate_stmt(x->m_body[i]));
                    }
                    for( size_t i = 0; i < x->n_orelse; i++ ) {
                        m_orelse.push_back(al, duplicate_stmt(x->m_orelse[i]));
                    }
                    return ASRUtils::STMT(ASR::make_If_t(al, x->base.base.loc,
                            m_test, m_body.p, x->n_body, m_orelse.p, x->n_orelse));
                }

        };

    }

} // namespace LFortran

#endif // LFORTRAN_PASS_UTILS_H
