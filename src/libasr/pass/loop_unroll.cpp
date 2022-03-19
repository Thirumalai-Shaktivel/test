#include <libasr/asr.h>
#include <libasr/containers.h>
#include <libasr/exception.h>
#include <libasr/asr_utils.h>
#include <libasr/asr_verify.h>
#include <libasr/pass/loop_unroll.h>
#include <libasr/pass/pass_utils.h>

#include <vector>
#include <map>
#include <utility>
#include <cmath>


namespace LFortran {

using ASR::down_cast;
using ASR::is_a;

class LoopUnrollVisitor : public PassUtils::PassVisitor<LoopUnrollVisitor>
{
private:

    std::string rl_path;

    size_t unroll_factor;

    ASR::ExprStmtDuplicator node_duplicator;

public:

    LoopUnrollVisitor(Allocator &al_, const std::string& rl_path_,
                      size_t unroll_factor_) :
    PassVisitor(al_, nullptr), rl_path(rl_path_),
    unroll_factor(unroll_factor_), node_duplicator(al_)
    {
        pass_result.reserve(al, 1);
    }

    void visit_DoLoop(const ASR::DoLoop_t& x) {
        ASR::DoLoop_t& xx = const_cast<ASR::DoLoop_t&>(x);
        ASR::do_loop_head_t x_head = x.m_head;
        ASR::expr_t* x_start = ASRUtils::expr_value(x_head.m_start);
        ASR::expr_t* x_end = ASRUtils::expr_value(x_head.m_end);
        ASR::expr_t* x_inc = ASRUtils::expr_value(x_head.m_increment);
        ASR::expr_t* itr_var = x_head.m_v;
        size_t _start, _end, _inc;
        if( !ASRUtils::is_value_constant(x_start, _start) ||
            !ASRUtils::is_value_constant(x_end, _end) ||
            !ASRUtils::is_value_constant(x_inc, _inc) ) {
            return ;
        }
        size_t loop_size = std::ceilf( ((float) (_end - _start + 1)) / ((float) _inc) );
        size_t unroll_factor_ = std::min(unroll_factor_, loop_size);
        Vec<ASR::stmt_t*> unrolled_loop;
        unrolled_loop.reserve(al, x.n_body * unroll_factor_);
        for( size_t i = 0; i < x.n_body; i++ ) {
            unrolled_loop.push_back(al, x.m_body[i]);
        }

        for( size_t i = 0; i < unroll_factor_; i++ ) {
            ASR::ttype_t *int32_type = LFortran::ASRUtils::TYPE(ASR::make_Integer_t(al, x_head.m_v->base.loc, 4, nullptr, 0));
            ASR::expr_t* itr_var_inc = ASRUtils::EXPR(ASR::make_ConstantInteger_t(al, x_head.m_v->base.loc, i + 1,
                                                      int32_type));
            ASR::expr_t* inc_expr = ASRUtils::EXPR(ASR::make_BinOp_t(al, x_head.m_v->base.loc, itr_var,
                                        ASR::binopType::Add, itr_var_inc, ASRUtils::expr_type(itr_var),
                                        nullptr, nullptr));
            ASR::stmt_t* update_itr_stmt = ASRUtils::STMT(ASR::make_Assignment_t(al, x_head.m_v->base.loc, itr_var, inc_expr, nullptr));
            unrolled_loop.push_back(al, update_itr_stmt);
            for( size_t i = 0; i < x.n_body; i++ ) {
                node_duplicator.success = false;
                ASR::stmt_t* m_body_copy = node_duplicator.duplicate_stmt(x.m_body[i]);
                if( !node_duplicator.success ) {
                    return ;
                }
                visit_stmt(*m_body_copy);
                unrolled_loop.push_back(al, m_body_copy);
            }
        }

        xx.m_head.m_increment =
    }

};

void pass_loop_unroll(Allocator &al, ASR::TranslationUnit_t &unit,
                      const std::string& rl_path,
                      size_t unroll_factor) {
    LoopUnrollVisitor v(al, rl_path, unroll_factor);
    LFORTRAN_ASSERT(asr_verify(unit));
}


} // namespace LFortran
